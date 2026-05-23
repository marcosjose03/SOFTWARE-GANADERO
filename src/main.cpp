#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QDir>
#include <fstream>

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
#include "application/session/SessionManager.h"
#include "presentation/viewmodels/AppViewModel.h"

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    int    argc = __argc;
    char** argv = __argv;
#else
int main(int argc, char* argv[]) {
#endif

    QCoreApplication::setOrganizationName("SoftwareGanadero");
    QCoreApplication::setApplicationName("SoftwareGanadero");

    QGuiApplication app(argc, argv);

    // ── Rutas de datos ────────────────────────────────────────────────────
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);

    std::string dbPath   = (dataDir + "/ganadero.db").toStdString();
    std::string lmdbPath = (dataDir + "/ganadero_lmdb").toStdString();

    // ── Infraestructura ───────────────────────────────────────────────────
    auto sqliteDb = std::make_shared<Infrastructure::SQLiteDatabase>(dbPath);
    sqliteDb->initSchema();
    auto lmdbDb = std::make_shared<Infrastructure::LmdbDatabase>(lmdbPath);

    // ── Repositorios ──────────────────────────────────────────────────────
    auto usuarioRepo    = std::make_shared<Infrastructure::SqliteUsuarioRepository>(sqliteDb);
    auto fincaRepo      = std::make_shared<Infrastructure::SqliteFincaRepository>(sqliteDb);
    auto ganadoRepo     = std::make_shared<Infrastructure::SqliteGanadoRepository>(sqliteDb);
    auto produccionRepo = std::make_shared<Infrastructure::LmdbProduccionRepository>(lmdbDb);

    // ── Use Cases ─────────────────────────────────────────────────────────
    auto createUsuarioUC  = std::make_shared<Application::CreateUsuarioUseCase>(usuarioRepo);
    auto loginUC          = std::make_shared<Application::LoginUseCase>(usuarioRepo);
    auto checkEmailUC = std::make_shared<Application::CheckEmailExistsUseCase>(usuarioRepo);
    auto getUsuarioByIdUC = std::make_shared<Application::GetUsuarioByIdUseCase>(usuarioRepo);
    auto createFincaUC    = std::make_shared<Application::CreateFincaUseCase>(fincaRepo);
    auto getAllFincasUC    = std::make_shared<Application::GetAllFincasUseCase>(fincaRepo);
    auto getFincaByIdUC   = std::make_shared<Application::GetFincaByIdUseCase>(fincaRepo);
    auto updateFincaUC    = std::make_shared<Application::UpdateFincaUseCase>(fincaRepo);
    auto deleteFincaUC    = std::make_shared<Application::DeleteFincaUseCase>(fincaRepo);
    auto createGanadoUC   = std::make_shared<Application::CreateGanadoUseCase>(ganadoRepo, produccionRepo);
    auto getAllGanadoUC    = std::make_shared<Application::GetAllGanadoUseCase>(ganadoRepo);
    auto getGanadoByIdUC  = std::make_shared<Application::GetGanadoByIdUseCase>(ganadoRepo);
    auto updateGanadoUC   = std::make_shared<Application::UpdateGanadoUseCase>(ganadoRepo);
    auto deleteGanadoUC   = std::make_shared<Application::DeleteGanadoUseCase>(ganadoRepo, produccionRepo);
    auto getByFincaUC     = std::make_shared<Application::GetGanadoByFincaUseCase>(ganadoRepo);
    auto getProduccionUC  = std::make_shared<Application::GetProduccionUseCase>(produccionRepo);
    auto updatePrenezUC   = std::make_shared<Application::UpdatePrenezUseCase>(produccionRepo);
    auto updateOrdenoUC   = std::make_shared<Application::UpdateOrdenoUseCase>(produccionRepo);
    auto addPartoUC       = std::make_shared<Application::AddPartoUseCase>(produccionRepo, ganadoRepo);
    auto updatePartoUC    = std::make_shared<Application::UpdatePartoUseCase>(produccionRepo);
    auto deletePartoUC    = std::make_shared<Application::DeletePartoUseCase>(produccionRepo);
    auto addLecheUC       = std::make_shared<Application::AddRegistroLecheUseCase>(produccionRepo);
    auto updateLecheUC    = std::make_shared<Application::UpdateRegistroLecheUseCase>(produccionRepo);
    auto deleteLecheUC    = std::make_shared<Application::DeleteRegistroLecheUseCase>(produccionRepo);
    auto addCarneUC       = std::make_shared<Application::AddRegistroCarneUseCase>(produccionRepo);
    auto updateCarneUC    = std::make_shared<Application::UpdateRegistroCarneUseCase>(produccionRepo);
    auto deleteCarneUC    = std::make_shared<Application::DeleteRegistroCarneUseCase>(produccionRepo);

    // ── Sesión ────────────────────────────────────────────────────────────
    auto session = std::make_shared<Application::SessionManager>();

    // Intentar restaurar sesión previa
    std::string savedUserId;
    std::ifstream sessionFile(Application::SessionManager::sessionFilePath());
    if (sessionFile.is_open()) {
        sessionFile >> savedUserId;
        sessionFile.close();
    }

    if (!savedUserId.empty()) {
        auto usuario = getUsuarioByIdUC->execute(savedUserId);
        if (usuario.has_value())
            session->loadSession(savedUserId, *usuario);
    }

    // ── AppViewModel ──────────────────────────────────────────────────────
    auto appViewModel = std::make_shared<Presentation::AppViewModel>(
        session,
        loginUC, createUsuarioUC, checkEmailUC,
        createFincaUC, getAllFincasUC, getFincaByIdUC, updateFincaUC, deleteFincaUC,
        createGanadoUC, getAllGanadoUC, getGanadoByIdUC, updateGanadoUC, deleteGanadoUC,
        getByFincaUC,
        getProduccionUC, updatePrenezUC, updateOrdenoUC,
        addPartoUC, updatePartoUC, deletePartoUC,
        addLecheUC, updateLecheUC, deleteLecheUC,
        addCarneUC, updateCarneUC, deleteCarneUC
    );

    // ── QML ───────────────────────────────────────────────────────────────
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appViewModel", appViewModel.get());

    const QUrl url(QStringLiteral("qrc:/SoftwareGanadero/ui/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );

    engine.load(url);
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}