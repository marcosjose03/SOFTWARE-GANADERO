// ============================================================================
//  benchmark_persistencia.cpp
//  Mide los tiempos de inserción y lectura del stack de persistencia
//  (SQLite para ganado + LMDB para producción) sin depender de Qt.
//
//  Inserta N animales, cada uno con M registros de leche, y cronometra:
//    1) creación de N animales   (SQLite: tabla ganado + Produccion vacía en LMDB)
//    2) inserción de N*M registros de leche (LMDB: read-modify-write por registro)
//    3) lectura completa de la producción de todos los animales (LMDB get + parse)
//
//  El patrón read-modify-write de LmdbProduccionRepository hace que insertar
//  el registro j-ésimo de un animal cueste re-serializar j registros; este
//  programa expone esa degradación O(M) por animal.
//
//  Uso:
//    ./benchmark_persistencia [N] [M]
//    N = número de animales   (por defecto 100)
//    M = registros de leche por animal (por defecto 100)
//
//  Compilación: ver benchmarks/README_BENCHMARK.md
// ============================================================================

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/database/LmdbDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "infrastructure/repositories/SqliteFincaRepository.h"
#include "infrastructure/repositories/SqliteGanadoRepository.h"
#include "infrastructure/repositories/LmdbProduccionRepository.h"
#include "application/usecases/usuario/UsuarioUseCases.h"
#include "application/usecases/finca/FincaUseCases.h"
#include "application/usecases/ganado/GanadoUseCases.h"
#include "application/usecases/produccion/ProduccionUseCases.h"

using Clock = std::chrono::steady_clock;

static double ms(Clock::time_point a, Clock::time_point b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

int main(int argc, char* argv[]) {
    const int N = (argc > 1) ? std::stoi(argv[1]) : 100;   // animales
    const int M = (argc > 2) ? std::stoi(argv[2]) : 100;   // registros de leche por animal

    std::cout << "=== Benchmark de persistencia (SQLite + LMDB) ===\n";
    std::cout << "N (animales)              : " << N << "\n";
    std::cout << "M (registros/animal)      : " << M << "\n";
    std::cout << "Total registros de leche  : " << (long long)N * M << "\n\n";

    // ── 1. Infraestructura ──────────────────────────────────────────────────
    // SQLite en archivo temporal (para incluir el costo real de disco;
    // usa ":memory:" si solo quieres medir CPU sin I/O de SQLite).
    namespace fs = std::filesystem;
    fs::path tmpBase = fs::temp_directory_path() / "ganadero_bench";
    fs::remove_all(tmpBase);
    fs::create_directories(tmpBase);

    std::string dbPath   = (tmpBase / "ganadero.db").string();
    std::string lmdbPath = (tmpBase / "lmdb").string();

    auto sqliteDb = std::make_shared<Infrastructure::SQLiteDatabase>(dbPath);
    sqliteDb->initSchema();
    // mapSize amplio para soportar N*M grandes (MB). Ajusta si insertas millones.
    auto lmdbDb = std::make_shared<Infrastructure::LmdbDatabase>(lmdbPath, 2048);

    // ── 2. Repositorios ─────────────────────────────────────────────────────
    auto usuarioRepo    = std::make_shared<Infrastructure::SqliteUsuarioRepository>(sqliteDb);
    auto fincaRepo      = std::make_shared<Infrastructure::SqliteFincaRepository>(sqliteDb);
    auto ganadoRepo     = std::make_shared<Infrastructure::SqliteGanadoRepository>(sqliteDb);
    auto produccionRepo = std::make_shared<Infrastructure::LmdbProduccionRepository>(lmdbDb);

    // ── 3. Casos de uso ─────────────────────────────────────────────────────
    Application::CreateUsuarioUseCase crearUsuario(usuarioRepo);
    Application::CreateFincaUseCase   crearFinca(fincaRepo);
    auto crearGanado    = std::make_shared<Application::CreateGanadoUseCase>(ganadoRepo, produccionRepo);
    auto addLeche       = std::make_shared<Application::AddRegistroLecheUseCase>(produccionRepo);
    auto getProduccion  = std::make_shared<Application::GetProduccionUseCase>(produccionRepo);

    // ── Setup: 1 usuario + 1 finca ──────────────────────────────────────────
    auto usuario = crearUsuario.execute({"BenchUser", "bench@test.com", "pass"});
    if (!usuario) { std::cerr << "No se pudo crear el usuario\n"; return 1; }
    auto finca = crearFinca.execute({"Finca Bench", 1000, 20, 5000, usuario->id});
    if (!finca) { std::cerr << "No se pudo crear la finca\n"; return 1; }

    std::vector<std::string> animalIds;
    animalIds.reserve(N);

    // ── FASE 1: crear N animales ────────────────────────────────────────────
    auto t0 = Clock::now();
    for (int i = 0; i < N; ++i) {
        Application::CreateGanadoDto dto{};
        dto.especie       = Domain::Especie::Bovino;
        dto.identificador = "A" + std::to_string(i);
        dto.idUsuario     = usuario->id;
        dto.idFinca       = finca->id;
        dto.nacimiento    = "2024-01-01";
        dto.sexo          = Domain::SexoGanado::Hembra;
        dto.estado        = Domain::EstadoGanado::Activo;
        // resto de campos opcionales quedan en std::nullopt

        auto res = crearGanado->execute(dto);
        if (!res) { std::cerr << "Fallo al crear animal " << i << "\n"; return 1; }
        animalIds.push_back(res->id);
    }
    auto t1 = Clock::now();

    // ── FASE 2: insertar M registros de leche por animal ────────────────────
    for (int j = 0; j < M; ++j) {
        for (int i = 0; i < N; ++i) {
            // fecha sintética; valor variable
            std::string fecha = "2024-01-" + std::to_string((j % 28) + 1);
            addLeche->execute(animalIds[i], Application::RegistroFechaDto{fecha, 10.0 + (j % 10)});
        }
    }
    auto t2 = Clock::now();

    // ── FASE 3: leer la producción completa de cada animal ──────────────────
    long long registrosLeidos = 0;
    for (const auto& id : animalIds) {
        auto p = getProduccion->execute(id);
        if (p) registrosLeidos += (long long)p->registroLeche.size();
    }
    auto t3 = Clock::now();

    // ── Resultados ──────────────────────────────────────────────────────────
    double fase1 = ms(t0, t1);
    double fase2 = ms(t1, t2);
    double fase3 = ms(t2, t3);
    long long totalRegistros = (long long)N * M;

    std::cout << "----------------------------------------------------------\n";
    std::cout << "FASE 1  crear " << N << " animales        : "
              << fase1 << " ms  ("
              << (fase1 / N) << " ms/animal)\n";
    std::cout << "FASE 2  insertar " << totalRegistros << " registros leche : "
              << fase2 << " ms  ("
              << (fase2 / (totalRegistros ? totalRegistros : 1)) << " ms/registro)\n";
    std::cout << "FASE 3  leer produccion de " << N << " animales : "
              << fase3 << " ms  ("
              << (fase3 / N) << " ms/animal)\n";
    std::cout << "----------------------------------------------------------\n";
    std::cout << "Registros de leche leidos (verificacion): " << registrosLeidos
              << " (esperado " << totalRegistros << ")\n";

    // Tamaño en disco de la base LMDB resultante
    std::error_code ec;
    auto lmdbData = fs::path(lmdbPath) / "data.mdb";
    if (fs::exists(lmdbData, ec)) {
        std::cout << "Tamaño data.mdb (LMDB) : "
                  << (fs::file_size(lmdbData, ec) / 1024) << " KB\n";
    }
    auto sqliteSize = fs::exists(dbPath, ec) ? fs::file_size(dbPath, ec) : 0;
    std::cout << "Tamaño ganadero.db (SQLite) : " << (sqliteSize / 1024) << " KB\n";

    // Limpieza
    fs::remove_all(tmpBase);
    return 0;
}
