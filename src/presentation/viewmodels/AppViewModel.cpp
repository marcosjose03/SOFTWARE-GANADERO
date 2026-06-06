#include "AppViewModel.h"
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QStringList>
#include "domain/entities/GanadoEnums.h"
#include "infrastructure/utils/UuidGenerator.h"
#include <QDebug>
#include "application/usecases/usuario/UsuarioUseCases.h"
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <stdexcept>

namespace Presentation {

AppViewModel::AppViewModel(
    std::shared_ptr<Application::SessionManager>                   session,
    std::shared_ptr<Application::ILoginUseCase>                    loginUC,
    std::shared_ptr<Application::ICreateUsuarioUseCase>            createUsuarioUC,
    std::shared_ptr<Application::ICheckEmailExistsUseCase>         checkEmailUC,
    std::shared_ptr<Application::ICreateFincaUseCase>              createFincaUC,
    std::shared_ptr<Application::IGetAllFincasUseCase>             getAllFincasUC,
    std::shared_ptr<Application::IGetFincaByIdUseCase>             getFincaByIdUC,
    std::shared_ptr<Application::IUpdateFincaUseCase>              updateFincaUC,
    std::shared_ptr<Application::IDeleteFincaUseCase>              deleteFincaUC,
    std::shared_ptr<Application::ICreateGanadoUseCase>             createGanadoUC,
    std::shared_ptr<Application::IGetAllGanadoUseCase>             getAllGanadoUC,
    std::shared_ptr<Application::IGetGanadoByIdUseCase>            getGanadoByIdUC,
    std::shared_ptr<Application::IUpdateGanadoUseCase>             updateGanadoUC,
    std::shared_ptr<Application::IDeleteGanadoUseCase>             deleteGanadoUC,
    std::shared_ptr<Application::IGetGanadoByFincaUseCase>         getByFincaUC,
    std::shared_ptr<Application::IValidarProgenitoresUseCase>      validarProgenitoresUC,
    std::shared_ptr<Application::IActualizarFechaPartaMadreUseCase> actualizarPartaMadreUC,
    std::shared_ptr<Application::IGetProduccionUseCase>            getProduccionUC,
    std::shared_ptr<Application::IUpdatePrenezUseCase>             updatePrenezUC,
    std::shared_ptr<Application::IUpdateOrdenoUseCase>             updateOrdenoUC,
    std::shared_ptr<Application::IAddPartoUseCase>                 addPartoUC,
    std::shared_ptr<Application::IUpdatePartoUseCase>              updatePartoUC,
    std::shared_ptr<Application::IDeletePartoUseCase>              deletePartoUC,
    std::shared_ptr<Application::IAddRegistroLecheUseCase>         addLecheUC,
    std::shared_ptr<Application::IUpdateRegistroLecheUseCase>      updateLecheUC,
    std::shared_ptr<Application::IDeleteRegistroLecheUseCase>      deleteLecheUC,
    std::shared_ptr<Application::IAddRegistroCarneUseCase>         addCarneUC,
    std::shared_ptr<Application::IUpdateRegistroCarneUseCase>      updateCarneUC,
    std::shared_ptr<Application::IDeleteRegistroCarneUseCase>      deleteCarneUC,
    QObject* parent)
    : QObject(parent)
    , m_session(std::move(session))
    , m_loginUC(std::move(loginUC))
    , m_createUsuarioUC(std::move(createUsuarioUC))
    , m_checkEmailUC(std::move(checkEmailUC))
    , m_createFincaUC(std::move(createFincaUC))
    , m_getAllFincasUC(std::move(getAllFincasUC))
    , m_getFincaByIdUC(std::move(getFincaByIdUC))
    , m_updateFincaUC(std::move(updateFincaUC))
    , m_deleteFincaUC(std::move(deleteFincaUC))
    , m_createGanadoUC(std::move(createGanadoUC))
    , m_getAllGanadoUC(std::move(getAllGanadoUC))
    , m_getGanadoByIdUC(std::move(getGanadoByIdUC))
    , m_updateGanadoUC(std::move(updateGanadoUC))
    , m_deleteGanadoUC(std::move(deleteGanadoUC))
    , m_getByFincaUC(std::move(getByFincaUC))
    , m_validarProgenitoresUC(std::move(validarProgenitoresUC))
    , m_actualizarPartaMadreUC(std::move(actualizarPartaMadreUC))
    , m_getProduccionUC(std::move(getProduccionUC))
    , m_updatePrenezUC(std::move(updatePrenezUC))
    , m_updateOrdenoUC(std::move(updateOrdenoUC))
    , m_addPartoUC(std::move(addPartoUC))
    , m_updatePartoUC(std::move(updatePartoUC))
    , m_deletePartoUC(std::move(deletePartoUC))
    , m_addLecheUC(std::move(addLecheUC))
    , m_updateLecheUC(std::move(updateLecheUC))
    , m_deleteLecheUC(std::move(deleteLecheUC))
    , m_addCarneUC(std::move(addCarneUC))
    , m_updateCarneUC(std::move(updateCarneUC))
    , m_deleteCarneUC(std::move(deleteCarneUC))
{
    m_currentScreen = m_session->isLoggedIn() ? "menu" : "login";
}

// ─── Navegación ──────────────────────────────────────────────────────────────

void AppViewModel::setScreen(const QString& screen) {
    if (m_currentScreen != screen) {
        m_currentScreen = screen;
        emit currentScreenChanged();
    }
}

QString AppViewModel::currentScreen() const { return m_currentScreen; }
bool    AppViewModel::isLoggedIn()    const { return m_session->isLoggedIn(); }
QString AppViewModel::userName()      const {
    return QString::fromStdString(m_session->userName());
}

void AppViewModel::goToLogin()                 { setScreen("login"); }
void AppViewModel::goToRegister()              { setScreen("register"); }
void AppViewModel::goToMenu()                  { setScreen("menu"); }
void AppViewModel::goToInventario()            { setScreen("inventario"); }
void AppViewModel::goToRegistroReproductivo()  { setScreen("reproductivo"); }
void AppViewModel::goToRegistroLeche()         { setScreen("leche"); }
void AppViewModel::goToRegistroPeso()          { setScreen("peso"); }

// ─── Sesión ───────────────────────────────────────────────────────────────────

bool AppViewModel::login(const QString& email, const QString& contrasena) {
    Application::LoginDto dto{
        email.toStdString(),
        contrasena.toStdString()
    };
    auto result = m_loginUC->execute(dto);
    if (!result) {
        emit errorOccurred("Correo o contraseña incorrectos");
        return false;
    }
    m_session->login(*result);
    emit sessionChanged();
    setScreen("menu");
    return true;
}

bool AppViewModel::createAccount(const QString& nombre,
                                  const QString& email,
                                  const QString& contrasena) {
    if (m_checkEmailUC->execute(email.toStdString())) {
        emit errorOccurred("Este correo ya está registrado");
        return false;
    }

    Application::CreateUsuarioDto dto{
        nombre.toStdString(),
        email.toStdString(),
        contrasena.toStdString()
    };

    auto result = m_createUsuarioUC->execute(dto);
    if (!result) {
        emit errorOccurred("No se pudo crear la cuenta");
        return false;
    }
    m_session->login(*result);
    emit sessionChanged();
    setScreen("menu");
    return true;
}

void AppViewModel::logout() {
    m_session->logout();
    emit sessionChanged();
    setScreen("login");
}

// Helper para resolver identificador desde UUID
static QString resolverIdentificador(
    const std::optional<std::string>& idOpt,
    const std::shared_ptr<Application::IGetGanadoByIdUseCase>& getByIdUC) {
    if (!idOpt || idOpt->empty()) return "";
    auto g = getByIdUC->execute(*idOpt);
    return g.has_value() ? QString::fromStdString(g->identificador) : "";
}

QString AppViewModel::getFotoBase64(const QString& id) {
    auto g = m_getGanadoByIdUC->execute(id.toStdString());
    if (!g || !g->foto.has_value() || g->foto->empty())
        return "";
    QByteArray bytes(
        reinterpret_cast<const char*>(g->foto->data()),
        static_cast<qsizetype>(g->foto->size()));
    return QString::fromLatin1(bytes.toBase64());
}

bool AppViewModel::updateFoto(const QString& id, const QString& base64Data) {
    auto g = m_getGanadoByIdUC->execute(id.toStdString());
    if (!g) return false;

    QByteArray bytes = QByteArray::fromBase64(base64Data.toLatin1());
    QImage img;
    img.loadFromData(bytes);
    if (img.isNull()) return false;

    QImage scaled = img.scaled(255, 255,
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation).copy(0, 0, 255, 255);

    QByteArray out;
    QBuffer buf(&out);
    buf.open(QIODevice::WriteOnly);
    scaled.save(&buf, "PNG");

    auto existing = m_getGanadoByIdUC->execute(id.toStdString());
    if (!existing) return false;

    Application::UpdateGanadoDto dto{
        existing->id,
        existing->especie,
        existing->identificador,
        existing->idFinca,
        existing->nacimiento,
        existing->sexo,
        existing->estado,
        existing->raza,
        existing->idPadre,
        existing->idMadre,
        existing->chapeta,
        existing->fechaDestete,
        existing->fechaUltimoParto,
        existing->fechaUltimaPalpacion,
        existing->fechaInseminacion,
        std::vector<uint8_t>(out.begin(), out.end())
    };
    return m_updateGanadoUC->execute(dto);
}

QString AppViewModel::leerArchivoBase64(const QString& filePath) {
    QString path = filePath;
    // Quitar prefijo file:///
    if (path.startsWith("file:///"))
        path = path.mid(8);
    else if (path.startsWith("file://"))
        path = path.mid(7);

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return "";

    QByteArray bytes = file.readAll();
    file.close();

    // Redimensionar a 255x255
    QImage img;
    img.loadFromData(bytes);
    if (img.isNull()) return "";

    QImage scaled = img.scaled(255, 255,
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation).copy(0, 0, 255, 255);

    QByteArray out;
    QBuffer buf(&out);
    buf.open(QIODevice::WriteOnly);
    scaled.save(&buf, "PNG");

    return QString::fromLatin1(out.toBase64());
}

// ─── Finca ────────────────────────────────────────────────────────────────────

QVariantList AppViewModel::getFincas() {
    QVariantList list;
    for (const auto& f : m_getAllFincasUC->execute()) {
        if (f.idUsuario != m_session->userId()) continue;
        QVariantMap map;
        map["id"]         = QString::fromStdString(f.id);
        map["nombre"]     = QString::fromStdString(f.nombre);
        map["nHectareas"] = f.nHectareas;
        map["nPotreros"]  = f.nPotreros;
        map["capacidad"]  = f.capacidad;
        list.append(map);
    }
    return list;
}

QVariantMap AppViewModel::getFinca(const QString& id) {
    auto f = m_getFincaByIdUC->execute(id.toStdString());
    if (!f) return {};
    QVariantMap map;
    map["id"]         = QString::fromStdString(f->id);
    map["nombre"]     = QString::fromStdString(f->nombre);
    map["nHectareas"] = f->nHectareas;
    map["nPotreros"]  = f->nPotreros;
    map["capacidad"]  = f->capacidad;
    return map;
}

bool AppViewModel::createFinca(const QString& nombre,
                                int nHectareas,
                                int nPotreros,
                                int capacidad) {
    Application::CreateFincaDto dto{
        nombre.toStdString(),
        nHectareas, nPotreros, capacidad,
        m_session->userId()
    };
    auto result = m_createFincaUC->execute(dto);
    if (!result) {
        emit errorOccurred("No se pudo crear la finca");
        return false;
    }
    return true;
}

bool AppViewModel::updateFinca(const QString& id,
                                const QString& nombre,
                                int nHectareas,
                                int nPotreros,
                                int capacidad) {
    Application::UpdateFincaDto dto{
        id.toStdString(),
        nombre.toStdString(),
        nHectareas, nPotreros, capacidad
    };
    bool ok = m_updateFincaUC->execute(dto);
    if (!ok) emit errorOccurred("No se pudo actualizar la finca");
    return ok;
}

bool AppViewModel::deleteFinca(const QString& id) {
    bool ok = m_deleteFincaUC->execute(id.toStdString());
    if (!ok) emit errorOccurred("No se pudo eliminar la finca");
    return ok;
}

bool AppViewModel::fincaTieneAnimales(const QString& idFinca) {
    return !m_getByFincaUC->execute(idFinca.toStdString()).empty();
}

// ─── Ganado ───────────────────────────────────────────────────────────────────

QVariantList AppViewModel::getGanadoByFinca(const QString& idFinca) {
    QVariantList list;
    for (const auto& g : m_getByFincaUC->execute(idFinca.toStdString())) {
        QVariantMap map;
        map["id"]            = QString::fromStdString(g.id);
        map["especie"]       = QString::fromStdString(Domain::especieToString(g.especie));
        map[QStringLiteral("identificador")] = QString::fromStdString(g.identificador);
        map["sexo"]          = QString::fromStdString(Domain::sexoToString(g.sexo));
        map["estado"]        = QString::fromStdString(Domain::estadoToString(g.estado));
        map["raza"]          = g.raza ? QString::fromStdString(*g.raza) : "";
        map["nacimiento"]    = QString::fromStdString(g.nacimiento);
        map["idFinca"]       = QString::fromStdString(g.idFinca);
        map[QStringLiteral("padre")]              = g.idPadre ? QString::fromStdString(*g.idPadre) : "";
        map[QStringLiteral("padreIdentificador")] = resolverIdentificador(g.idPadre, m_getGanadoByIdUC);
        map[QStringLiteral("madre")]              = g.idMadre ? QString::fromStdString(*g.idMadre) : "";
        map[QStringLiteral("madreIdentificador")] = resolverIdentificador(g.idMadre, m_getGanadoByIdUC);
        map["chapeta"]       = g.chapeta ? QString::fromStdString(*g.chapeta) : "";
        map["fechaDestete"]  = g.fechaDestete ? QString::fromStdString(*g.fechaDestete) : "";
        map["fechaUltimoParto"]     = g.fechaUltimoParto
            ? QString::fromStdString(*g.fechaUltimoParto) : "";
        map["fechaUltimaPalpacion"] = g.fechaUltimaPalpacion
            ? QString::fromStdString(*g.fechaUltimaPalpacion) : "";
        map["fechaInseminacion"]    = g.fechaInseminacion
            ? QString::fromStdString(*g.fechaInseminacion) : "";
        list.append(map);
    }
    return list;
}

QVariantMap AppViewModel::getGanado(const QString& id) {
    auto g = m_getGanadoByIdUC->execute(id.toStdString());
    if (!g) return {};
    QVariantMap map;
    map["id"]            = QString::fromStdString(g->id);
    map["especie"]       = QString::fromStdString(Domain::especieToString(g->especie));
    map[QStringLiteral("identificador")] = QString::fromStdString(g->identificador);
    map["sexo"]          = QString::fromStdString(Domain::sexoToString(g->sexo));
    map["estado"]        = QString::fromStdString(Domain::estadoToString(g->estado));
    map["raza"]          = g->raza ? QString::fromStdString(*g->raza) : "";
    map["nacimiento"]    = QString::fromStdString(g->nacimiento);
    map["idFinca"]       = QString::fromStdString(g->idFinca);
    map[QStringLiteral("padre")]              = g->idPadre ? QString::fromStdString(*g->idPadre) : "";
    map[QStringLiteral("padreIdentificador")] = resolverIdentificador(g->idPadre, m_getGanadoByIdUC);
    map[QStringLiteral("madre")]              = g->idMadre ? QString::fromStdString(*g->idMadre) : "";
    map[QStringLiteral("madreIdentificador")] = resolverIdentificador(g->idMadre, m_getGanadoByIdUC);
    map["chapeta"]       = g->chapeta ? QString::fromStdString(*g->chapeta) : "";
    map["fechaDestete"]  = g->fechaDestete
        ? QString::fromStdString(*g->fechaDestete) : "";
    map["fechaUltimoParto"]     = g->fechaUltimoParto
        ? QString::fromStdString(*g->fechaUltimoParto) : "";
    map["fechaUltimaPalpacion"] = g->fechaUltimaPalpacion
        ? QString::fromStdString(*g->fechaUltimaPalpacion) : "";
    map["fechaInseminacion"]    = g->fechaInseminacion
        ? QString::fromStdString(*g->fechaInseminacion) : "";
    return map;
}

bool AppViewModel::createGanado(const QVariantMap& data) {
    try {
        auto optStr = [&](const QString& key) -> std::optional<std::string> {
            QString v = data[key].toString();
            return v.isEmpty() ? std::nullopt : std::optional<std::string>(v.toStdString());
        };

        // Validar progenitores
        auto errorProg = m_validarProgenitoresUC->execute(
            data[QStringLiteral("nacimiento")].toString().toStdString(),
            optStr(QStringLiteral("padre")),
            optStr(QStringLiteral("madre")));
        if (!errorProg.empty()) {
            emit errorOccurred(QString::fromStdString(errorProg));
            return false;
        }

        Application::CreateGanadoDto dto{
            Domain::especieFromString(data[QStringLiteral("especie")].toString().toStdString()),
            data[QStringLiteral("identificador")].toString().toStdString(),
            m_session->userId(),
            data[QStringLiteral("idFinca")].toString().toStdString(),
            data[QStringLiteral("nacimiento")].toString().toStdString(),
            Domain::sexoFromString(data[QStringLiteral("sexo")].toString().toStdString()),
            Domain::estadoFromString(data[QStringLiteral("estado")].toString().toStdString()),
            optStr(QStringLiteral("raza")),
            optStr(QStringLiteral("padre")),
            optStr(QStringLiteral("madre")),
            optStr(QStringLiteral("chapeta")),
            optStr(QStringLiteral("fechaDestete")),
            std::nullopt,
            optStr(QStringLiteral("fechaUltimoParto")),
            optStr(QStringLiteral("fechaUltimaPalpacion")),
            optStr(QStringLiteral("fechaInseminacion"))
        };

        auto result = m_createGanadoUC->execute(dto);
        if (!result) {
            emit errorOccurred("No se pudo registrar el animal");
            return false;
        }

        // Actualizar fecha último parto de la madre si aplica
        m_actualizarPartaMadreUC->execute(
            data[QStringLiteral("nacimiento")].toString().toStdString(),
            optStr(QStringLiteral("madre")));

        return true;
    } catch (const std::invalid_argument& e) {
        emit errorOccurred(QString::fromStdString(e.what()));
        return false;
    } catch (...) {
        emit errorOccurred("Datos del animal inválidos");
        return false;
    }
}

bool AppViewModel::updateGanado(const QVariantMap& data) {
    try {
        auto optStr = [&](const QString& key) -> std::optional<std::string> {
            QString v = data[key].toString();
            return v.isEmpty() ? std::nullopt : std::optional<std::string>(v.toStdString());
        };

        // Validar progenitores
        auto errorProg = m_validarProgenitoresUC->execute(
            data[QStringLiteral("nacimiento")].toString().toStdString(),
            optStr(QStringLiteral("padre")),
            optStr(QStringLiteral("madre")));
        if (!errorProg.empty()) {
            emit errorOccurred(QString::fromStdString(errorProg));
            return false;
        }

        Application::UpdateGanadoDto dto{
            data[QStringLiteral("id")].toString().toStdString(),
            Domain::especieFromString(data[QStringLiteral("especie")].toString().toStdString()),
            data[QStringLiteral("identificador")].toString().toStdString(),
            data[QStringLiteral("idFinca")].toString().toStdString(),
            data[QStringLiteral("nacimiento")].toString().toStdString(),
            Domain::sexoFromString(data[QStringLiteral("sexo")].toString().toStdString()),
            Domain::estadoFromString(data[QStringLiteral("estado")].toString().toStdString()),
            optStr(QStringLiteral("raza")),
            optStr(QStringLiteral("padre")),
            optStr(QStringLiteral("madre")),
            optStr(QStringLiteral("chapeta")),
            optStr(QStringLiteral("fechaDestete")),
            optStr(QStringLiteral("fechaUltimoParto")),
            optStr(QStringLiteral("fechaUltimaPalpacion")),
            optStr(QStringLiteral("fechaInseminacion")),
            std::nullopt
        };

        bool ok = m_updateGanadoUC->execute(dto);
        if (!ok) {
            emit errorOccurred("No se pudo actualizar el animal");
            return false;
        }

        // Actualizar fecha último parto de la madre si aplica
        m_actualizarPartaMadreUC->execute(
            data[QStringLiteral("nacimiento")].toString().toStdString(),
            optStr(QStringLiteral("madre")));

        return true;
    } catch (const std::invalid_argument& e) {
        emit errorOccurred(QString::fromStdString(e.what()));
        return false;
    } catch (...) {
        emit errorOccurred("Datos del animal inválidos");
        return false;
    }
}

bool AppViewModel::deleteGanado(const QString& id) {
    bool ok = m_deleteGanadoUC->execute(id.toStdString());
    if (!ok) emit errorOccurred("No se pudo eliminar el animal");
    return ok;
}

QVariantList AppViewModel::getAllGanado() {
    QVariantList list;
    for (const auto& g : m_getAllGanadoUC->execute()) {
        QVariantMap map;
        map[QStringLiteral("id")]            = QString::fromStdString(g.id);
        map[QStringLiteral("especie")]       = QString::fromStdString(Domain::especieToString(g.especie));
        map[QStringLiteral("identificador")] = QString::fromStdString(g.identificador);
        map[QStringLiteral("sexo")]          = QString::fromStdString(Domain::sexoToString(g.sexo));
        map[QStringLiteral("estado")]        = QString::fromStdString(Domain::estadoToString(g.estado));
        map[QStringLiteral("raza")]          = g.raza ? QString::fromStdString(*g.raza) : "";
        map[QStringLiteral("nacimiento")]    = QString::fromStdString(g.nacimiento);
        map[QStringLiteral("idFinca")]       = QString::fromStdString(g.idFinca);
        map[QStringLiteral("padre")]              = g.idPadre ? QString::fromStdString(*g.idPadre) : "";
        map[QStringLiteral("padreIdentificador")] = resolverIdentificador(g.idPadre, m_getGanadoByIdUC);
        map[QStringLiteral("madre")]              = g.idMadre ? QString::fromStdString(*g.idMadre) : "";
        map[QStringLiteral("madreIdentificador")] = resolverIdentificador(g.idMadre, m_getGanadoByIdUC);
        map[QStringLiteral("chapeta")]       = g.chapeta ? QString::fromStdString(*g.chapeta) : "";
        map[QStringLiteral("fechaDestete")]  = g.fechaDestete
            ? QString::fromStdString(*g.fechaDestete) : "";
        map[QStringLiteral("fechaUltimoParto")]     = g.fechaUltimoParto
            ? QString::fromStdString(*g.fechaUltimoParto) : "";
        map[QStringLiteral("fechaUltimaPalpacion")] = g.fechaUltimaPalpacion
            ? QString::fromStdString(*g.fechaUltimaPalpacion) : "";
        map[QStringLiteral("fechaInseminacion")]    = g.fechaInseminacion
            ? QString::fromStdString(*g.fechaInseminacion) : "";
        
        auto prod = m_getProduccionUC->execute(g.id);
        map[QStringLiteral("ordeno")] = QVariant(prod.has_value() ? prod->ordeno : false);
        map[QStringLiteral("prenez")] = QVariant(prod.has_value() ? prod->prenez : false);

        list.append(map);
    }
    return list;
}

QString AppViewModel::validarProgenitores(const QString& fechaNacimiento,
                                           const QString& idPadre,
                                           const QString& idMadre) {
    auto optStr = [](const QString& s) -> std::optional<std::string> {
        return s.isEmpty() ? std::nullopt : std::optional<std::string>(s.toStdString());
    };
    auto error = m_validarProgenitoresUC->execute(
        fechaNacimiento.toStdString(),
        optStr(idPadre),
        optStr(idMadre));
    return QString::fromStdString(error);
}

// ─── Producción ───────────────────────────────────────────────────────────────

QVariantMap AppViewModel::getProduccion(const QString& id) {
    auto p = m_getProduccionUC->execute(id.toStdString());
    if (!p) return {};

    QVariantMap map;
    map["id"]     = QString::fromStdString(p->id);
    map["prenez"] = p->prenez;
    map["ordeno"] = p->ordeno;

    QVariantList partos;
    for (const auto& parto : p->partos) {
        QVariantMap pm;
        pm["id"]    = QString::fromStdString(parto.id);
        pm["fecha"] = QString::fromStdString(parto.fecha);
        partos.append(pm);
    }
    map["partos"] = partos;

    QVariantList leche;
    for (const auto& r : p->registroLeche) {
        QVariantMap rm;
        rm["fecha"] = QString::fromStdString(r.fecha);
        rm["valor"] = r.valor;
        leche.append(rm);
    }
    map["registroLeche"] = leche;

    QVariantList carne;
    for (const auto& r : p->registroCarne) {
        QVariantMap rm;
        rm["fecha"] = QString::fromStdString(r.fecha);
        rm["valor"] = r.valor;
        carne.append(rm);
    }
    map["registroCarne"] = carne;

    return map;
}

bool AppViewModel::updatePrenez(const QString& id, bool prenez) {
    return m_updatePrenezUC->execute(id.toStdString(), prenez);
}

bool AppViewModel::updateOrdeno(const QString& id, bool ordeno) {
    return m_updateOrdenoUC->execute(id.toStdString(), ordeno);
}

bool AppViewModel::addParto(const QString& id,
                             const QString& partoId,
                             const QString& fecha) {
    Application::PartoDto dto{partoId.toStdString(), fecha.toStdString()};
    return m_addPartoUC->execute(id.toStdString(), dto);
}

bool AppViewModel::updateParto(const QString& id,
                                const QString& partoId,
                                const QString& fecha) {
    Application::PartoDto dto{partoId.toStdString(), fecha.toStdString()};
    return m_updatePartoUC->execute(id.toStdString(), dto);
}

bool AppViewModel::deleteParto(const QString& id, const QString& partoId) {
    return m_deletePartoUC->execute(id.toStdString(), partoId.toStdString());
}

bool AppViewModel::addRegistroLeche(const QString& id,
                                     const QString& fecha,
                                     double valor) {
    return m_addLecheUC->execute(id.toStdString(), {fecha.toStdString(), valor});
}

bool AppViewModel::updateRegistroLeche(const QString& id,
                                        const QString& fechaOriginal,
                                        const QString& fechaNueva,
                                        double nuevoValor) {
    return m_updateLecheUC->execute(
        id.toStdString(),
        fechaOriginal.toStdString(),
        fechaNueva.toStdString(),
        nuevoValor);
}

bool AppViewModel::deleteRegistroLeche(const QString& id, const QString& fecha) {
    return m_deleteLecheUC->execute(id.toStdString(), fecha.toStdString());
}

bool AppViewModel::addRegistroCarne(const QString& id,
                                     const QString& fecha,
                                     double valor) {
    return m_addCarneUC->execute(id.toStdString(), {fecha.toStdString(), valor});
}

bool AppViewModel::updateRegistroCarne(const QString& id,
                                        const QString& fechaOriginal,
                                        const QString& fechaNueva,
                                        double nuevoValor) {
    return m_updateCarneUC->execute(
        id.toStdString(),
        fechaOriginal.toStdString(),
        fechaNueva.toStdString(),
        nuevoValor);
}

bool AppViewModel::deleteRegistroCarne(const QString& id, const QString& fecha) {
    return m_deleteCarneUC->execute(id.toStdString(), fecha.toStdString());
}

// ─── Enums ────────────────────────────────────────────────────────────────────

QStringList AppViewModel::getEspecies() {
    return {"Bovino", "Caprino", "Bufalino"};
}

QStringList AppViewModel::getRazasPorEspecie(const QString& especie) {
    qDebug() << "getRazasPorEspecie llamado con:" << especie;
    try {
        auto e     = Domain::especieFromString(especie.toStdString());
        auto razas = Domain::razasPorEspecie(e);
        qDebug() << "Razas encontradas:" << razas.size();
        QStringList list;
        for (const auto& r : razas)
            list.append(QString::fromStdString(r));
        return list;
    } catch (...) {
        qDebug() << "Excepcion en getRazasPorEspecie para:" << especie;
        return {};
    }
}

QStringList AppViewModel::getSexos() {
    return {"Macho", "Hembra"};
}

QStringList AppViewModel::getEstados() {
    return {"Activo", "Muerto", "Vendido", "Enfermo"};
}

} // namespace Presentation