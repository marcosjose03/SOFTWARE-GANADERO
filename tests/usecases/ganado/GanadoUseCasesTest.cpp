#include <gtest/gtest.h>
#include <filesystem>
#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/database/LmdbDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "infrastructure/repositories/SqliteFincaRepository.h"
#include "infrastructure/repositories/SqliteGanadoRepository.h"
#include "infrastructure/repositories/LmdbProduccionRepository.h"
#include "application/usecases/usuario/UsuarioUseCases.h"
#include "application/usecases/finca/FincaUseCases.h"
#include "application/usecases/ganado/GanadoUseCases.h"

using namespace Infrastructure;
using namespace Application;

class GanadoUseCasesTest : public ::testing::Test {
protected:
    void SetUp() override {
        db = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();

        testDir = std::filesystem::temp_directory_path() / "ganadero_uc_test";
        std::filesystem::remove_all(testDir);
        lmdb = std::make_shared<LmdbDatabase>(testDir.string());

        auto usuarioRepo = std::make_shared<SqliteUsuarioRepository>(db);
        auto fincaRepo   = std::make_shared<SqliteFincaRepository>(db);
        ganadoRepo       = std::make_shared<SqliteGanadoRepository>(db);
        produccionRepo   = std::make_shared<LmdbProduccionRepository>(lmdb);

        auto u  = CreateUsuarioUseCase(usuarioRepo).execute({"Dueño","d@t.com","pass"});
        auto f  = CreateFincaUseCase(fincaRepo).execute({"Finca",100,5,100,u->id});
        idUsuario = u->id;
        idFinca   = f->id;

        createUC  = std::make_shared<CreateGanadoUseCase>(ganadoRepo, produccionRepo);
        getAllUC   = std::make_shared<GetAllGanadoUseCase>(ganadoRepo);
        getByIdUC = std::make_shared<GetGanadoByIdUseCase>(ganadoRepo);
        updateUC  = std::make_shared<UpdateGanadoUseCase>(ganadoRepo);
        deleteUC  = std::make_shared<DeleteGanadoUseCase>(ganadoRepo, produccionRepo);
    }

    void TearDown() override {
        createUC.reset();
        getAllUC.reset();
        getByIdUC.reset();
        updateUC.reset();
        deleteUC.reset();
        produccionRepo.reset();
        ganadoRepo.reset();
        lmdb.reset();
        std::filesystem::remove_all(testDir);
    }

    std::shared_ptr<SQLiteDatabase>             db;
    std::shared_ptr<LmdbDatabase>               lmdb;
    std::shared_ptr<SqliteGanadoRepository>     ganadoRepo;
    std::shared_ptr<LmdbProduccionRepository>   produccionRepo;
    std::shared_ptr<CreateGanadoUseCase>        createUC;
    std::shared_ptr<GetAllGanadoUseCase>        getAllUC;
    std::shared_ptr<GetGanadoByIdUseCase>       getByIdUC;
    std::shared_ptr<UpdateGanadoUseCase>        updateUC;
    std::shared_ptr<DeleteGanadoUseCase>        deleteUC;
    std::filesystem::path                       testDir;
    std::string                                 idUsuario;
    std::string                                 idFinca;

    CreateGanadoDto makeDto() {
        return CreateGanadoDto{
            .especie       = Domain::Especie::Bovino,
            .identificador = "101",
            .idUsuario     = idUsuario,
            .idFinca       = idFinca,
            .nacimiento    = "2020-05-10",
            .sexo          = Domain::SexoGanado::Hembra,
            .estado        = Domain::EstadoGanado::Activo,
            .raza          = "Holstein Negro",
            .idPadre       = std::nullopt,
            .idMadre       = std::nullopt,
            .chapeta       = "A-101",
            .fechaDestete  = std::nullopt,
            .foto          = std::nullopt
        };
    }
};

TEST_F(GanadoUseCasesTest, CreateRetornaDto) {
    auto result = createUC->execute(makeDto());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->especie,       Domain::Especie::Bovino);
    EXPECT_EQ(result->identificador, "101");
    EXPECT_FALSE(result->id.empty());
}

TEST_F(GanadoUseCasesTest, CreateTambienCreaProduccion) {
    auto result = createUC->execute(makeDto());
    ASSERT_TRUE(result.has_value());
    auto prod = produccionRepo->getById(result->id);
    ASSERT_TRUE(prod.has_value());
    EXPECT_FALSE(prod->prenez);
    EXPECT_FALSE(prod->ordeno);
    EXPECT_TRUE(prod->partos.empty());
}

TEST_F(GanadoUseCasesTest, CreateRazaInvalidaParaEspecieFalla) {
    auto dto = makeDto();
    dto.raza = "Murrah";
    EXPECT_FALSE(createUC->execute(dto).has_value());
}

TEST_F(GanadoUseCasesTest, CreateSinRazaEsValido) {
    auto dto = makeDto();
    dto.raza = std::nullopt;
    EXPECT_TRUE(createUC->execute(dto).has_value());
}

TEST_F(GanadoUseCasesTest, GetAll) {
    createUC->execute(makeDto());
    auto dto2 = makeDto();
    dto2.identificador = "102";
    createUC->execute(dto2);
    EXPECT_EQ(getAllUC->execute().size(), 2u);
}

TEST_F(GanadoUseCasesTest, GetById) {
    auto created = createUC->execute(makeDto());
    auto result  = getByIdUC->execute(created->id);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->identificador, "101");
}

TEST_F(GanadoUseCasesTest, GetByIdInexistente) {
    EXPECT_FALSE(getByIdUC->execute("no-existe").has_value());
}

TEST_F(GanadoUseCasesTest, Update) {
    auto created = createUC->execute(makeDto());

    UpdateGanadoDto dto{
        .id            = created->id,
        .especie       = Domain::Especie::Bovino,
        .identificador = "101",
        .idFinca       = idFinca,
        .nacimiento    = "2020-05-10",
        .sexo          = Domain::SexoGanado::Hembra,
        .estado        = Domain::EstadoGanado::Vendido,
        .raza          = "Angus Negro",
        .idPadre       = std::nullopt,
        .idMadre       = std::nullopt,
        .chapeta       = std::nullopt,
        .fechaDestete  = std::nullopt,
        .fechaUltimoParto     = std::nullopt,
        .fechaUltimaPalpacion = std::nullopt,
        .fechaInseminacion    = std::nullopt,
        .foto          = std::nullopt
    };

    ASSERT_TRUE(updateUC->execute(dto));
    auto result = getByIdUC->execute(created->id);
    EXPECT_EQ(result->estado, Domain::EstadoGanado::Vendido);
    EXPECT_EQ(result->raza,   "Angus Negro");
}

// ─── GetGanadoByFinca ────────────────────────────────────────────────────────

TEST_F(GanadoUseCasesTest, GetByFincaRetornaAnimalesDeLaFinca) {
    auto getByFincaUC = std::make_shared<GetGanadoByFincaUseCase>(ganadoRepo);

    createUC->execute(makeDto());
    auto dto2 = makeDto();
    dto2.identificador = "102";
    createUC->execute(dto2);

    auto result = getByFincaUC->execute(idFinca);
    EXPECT_EQ(result.size(), 2u);
    for (const auto& g : result)
        EXPECT_EQ(g.idFinca, idFinca);
}

TEST_F(GanadoUseCasesTest, GetByFincaFincaVaciaRetornaVacio) {
    auto getByFincaUC = std::make_shared<GetGanadoByFincaUseCase>(ganadoRepo);
    auto result = getByFincaUC->execute(idFinca);
    EXPECT_TRUE(result.empty());
}

TEST_F(GanadoUseCasesTest, GetByFincaFincaInexistenteRetornaVacio) {
    auto getByFincaUC = std::make_shared<GetGanadoByFincaUseCase>(ganadoRepo);
    auto result = getByFincaUC->execute("finca-inexistente");
    EXPECT_TRUE(result.empty());
}

TEST_F(GanadoUseCasesTest, DeleteFincaSoloSiEstaVacia) {
    auto getByFincaUC = std::make_shared<GetGanadoByFincaUseCase>(ganadoRepo);

    // Con animales — no debe permitirse el delete
    createUC->execute(makeDto());
    EXPECT_FALSE(getByFincaUC->execute(idFinca).empty());

    // Sin animales — debe permitirse
    deleteUC->execute(ganadoRepo->getAll()[0].id);
    EXPECT_TRUE(getByFincaUC->execute(idFinca).empty());
}

// ─── ValidarProgenitores ──────────────────────────────────────────────────────

TEST_F(GanadoUseCasesTest, ValidarProgenitores_SinProgenitores_OK) {
    auto uc = std::make_shared<Application::ValidarProgenitoresUseCase>(ganadoRepo);
    auto error = uc->execute("2022-01-01", std::nullopt, std::nullopt);
    EXPECT_TRUE(error.empty());
}

TEST_F(GanadoUseCasesTest, ValidarProgenitores_PadreAnterior_OK) {
    auto padre = createUC->execute({
        Domain::Especie::Bovino, "p001", idUsuario, idFinca,
        "2018-01-01", Domain::SexoGanado::Macho,
        Domain::EstadoGanado::Activo, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt
    });
    auto uc = std::make_shared<Application::ValidarProgenitoresUseCase>(ganadoRepo);
    auto error = uc->execute("2022-01-01", padre->id, std::nullopt);
    EXPECT_TRUE(error.empty());
}

TEST_F(GanadoUseCasesTest, ValidarProgenitores_AnimalAntesQuePadre_Falla) {
    auto padre = createUC->execute({
        Domain::Especie::Bovino, "p001", idUsuario, idFinca,
        "2020-01-01", Domain::SexoGanado::Macho,
        Domain::EstadoGanado::Activo, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt
    });
    auto uc = std::make_shared<Application::ValidarProgenitoresUseCase>(ganadoRepo);
    auto error = uc->execute("2019-01-01", padre->id, std::nullopt);
    EXPECT_FALSE(error.empty());
}

TEST_F(GanadoUseCasesTest, ValidarProgenitores_AnimalAntesQueMadre_Falla) {
    auto madre = createUC->execute({
        Domain::Especie::Bovino, "m001", idUsuario, idFinca,
        "2020-06-01", Domain::SexoGanado::Hembra,
        Domain::EstadoGanado::Activo, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt
    });
    auto uc = std::make_shared<Application::ValidarProgenitoresUseCase>(ganadoRepo);
    auto error = uc->execute("2020-01-01", std::nullopt, madre->id);
    EXPECT_FALSE(error.empty());
}

// ─── ActualizarFechaPartaMadre ────────────────────────────────────────────────

TEST_F(GanadoUseCasesTest, ActualizarPartaMadre_CampoVacio_Actualiza) {
    auto madre = createUC->execute({
        Domain::Especie::Bovino, "m001", idUsuario, idFinca,
        "2018-01-01", Domain::SexoGanado::Hembra,
        Domain::EstadoGanado::Activo, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt
    });
    auto uc = std::make_shared<Application::ActualizarFechaPartaMadreUseCase>(
        ganadoRepo, produccionRepo);
    uc->execute("2022-05-01", madre->id);
    auto madreActualizada = ganadoRepo->getById(madre->id);
    ASSERT_TRUE(madreActualizada.has_value());
    EXPECT_EQ(madreActualizada->fechaUltimoParto, "2022-05-01");
}

TEST_F(GanadoUseCasesTest, ActualizarPartaMadre_NacimientoMasReciente_Actualiza) {
    auto madre = createUC->execute({
        Domain::Especie::Bovino, "m001", idUsuario, idFinca,
        "2018-01-01", Domain::SexoGanado::Hembra,
        Domain::EstadoGanado::Activo, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        "2021-01-01", std::nullopt, std::nullopt
    });
    auto uc = std::make_shared<Application::ActualizarFechaPartaMadreUseCase>(
        ganadoRepo, produccionRepo);
    uc->execute("2023-03-01", madre->id);
    auto madreActualizada = ganadoRepo->getById(madre->id);
    EXPECT_EQ(madreActualizada->fechaUltimoParto, "2023-03-01");
}

TEST_F(GanadoUseCasesTest, ActualizarPartaMadre_PartaMasReciente_NoActualiza) {
    auto madre = createUC->execute({
        Domain::Especie::Bovino, "m001", idUsuario, idFinca,
        "2018-01-01", Domain::SexoGanado::Hembra,
        Domain::EstadoGanado::Activo, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        "2024-01-01", std::nullopt, std::nullopt
    });
    auto uc = std::make_shared<Application::ActualizarFechaPartaMadreUseCase>(
        ganadoRepo, produccionRepo);
    uc->execute("2022-03-01", madre->id);
    auto madreActualizada = ganadoRepo->getById(madre->id);
    EXPECT_EQ(madreActualizada->fechaUltimoParto, "2024-01-01");
}

TEST_F(GanadoUseCasesTest, ActualizarPartaMadre_FechasIguales_NoActualiza) {
    auto madre = createUC->execute({
        Domain::Especie::Bovino, "m001", idUsuario, idFinca,
        "2018-01-01", Domain::SexoGanado::Hembra,
        Domain::EstadoGanado::Activo, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        "2022-05-01", std::nullopt, std::nullopt
    });
    auto uc = std::make_shared<Application::ActualizarFechaPartaMadreUseCase>(
        ganadoRepo, produccionRepo);
    uc->execute("2022-05-01", madre->id);
    auto madreActualizada = ganadoRepo->getById(madre->id);
    EXPECT_EQ(madreActualizada->fechaUltimoParto, "2022-05-01");
}