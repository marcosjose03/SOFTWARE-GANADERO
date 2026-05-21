#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/database/LmdbDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "infrastructure/repositories/SqliteFincaRepository.h"
#include "infrastructure/repositories/SqliteGanadoRepository.h"
#include "infrastructure/repositories/LmdbProduccionRepository.h"

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    int    argc = __argc;
    char** argv = __argv;
#else
int main(int argc, char* argv[]) {
#endif

    QGuiApplication app(argc, argv);

    auto sqliteDb = std::make_shared<Infrastructure::SQLiteDatabase>("ganadero.db");
    sqliteDb->initSchema();

    auto lmdbDb = std::make_shared<Infrastructure::LmdbDatabase>("ganadero_lmdb");

    auto usuarioRepo    = std::make_shared<Infrastructure::SqliteUsuarioRepository>(sqliteDb);
    auto fincaRepo      = std::make_shared<Infrastructure::SqliteFincaRepository>(sqliteDb);
    auto ganadoRepo     = std::make_shared<Infrastructure::SqliteGanadoRepository>(sqliteDb);
    auto produccionRepo = std::make_shared<Infrastructure::LmdbProduccionRepository>(lmdbDb);

    QQmlApplicationEngine engine;

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