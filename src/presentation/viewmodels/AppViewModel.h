#pragma once
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QStringList>
#include <memory>
#include "application/session/SessionManager.h"
#include "application/usecases/usuario/UsuarioUseCases.h"
#include "application/usecases/finca/FincaUseCases.h"
#include "application/usecases/ganado/GanadoUseCases.h"
#include "application/usecases/produccion/ProduccionUseCases.h"
#include "application/usecases/ganado/IGanadoUseCases.h"

namespace Presentation {

class AppViewModel : public QObject {
    Q_OBJECT

    // Pantalla actual
    Q_PROPERTY(QString currentScreen READ currentScreen
               NOTIFY currentScreenChanged)

    // Datos de sesión expuestos a QML
    Q_PROPERTY(bool    isLoggedIn  READ isLoggedIn  NOTIFY sessionChanged)
    Q_PROPERTY(QString userName    READ userName    NOTIFY sessionChanged)

public:
    explicit AppViewModel(
        std::shared_ptr<Application::SessionManager>           session,
        std::shared_ptr<Application::ILoginUseCase>            loginUC,
        std::shared_ptr<Application::ICreateUsuarioUseCase>    createUsuarioUC,
        std::shared_ptr<Application::ICheckEmailExistsUseCase> checkEmailUC,
        std::shared_ptr<Application::ICreateFincaUseCase>      createFincaUC,
        std::shared_ptr<Application::IGetAllFincasUseCase>     getAllFincasUC,
        std::shared_ptr<Application::IGetFincaByIdUseCase>     getFincaByIdUC,
        std::shared_ptr<Application::IUpdateFincaUseCase>      updateFincaUC,
        std::shared_ptr<Application::IDeleteFincaUseCase>      deleteFincaUC,
        std::shared_ptr<Application::ICreateGanadoUseCase>     createGanadoUC,
        std::shared_ptr<Application::IGetAllGanadoUseCase>     getAllGanadoUC,
        std::shared_ptr<Application::IGetGanadoByIdUseCase>    getGanadoByIdUC,
        std::shared_ptr<Application::IUpdateGanadoUseCase>     updateGanadoUC,
        std::shared_ptr<Application::IDeleteGanadoUseCase>     deleteGanadoUC,
        std::shared_ptr<Application::IGetGanadoByFincaUseCase> getByFincaUC,
        std::shared_ptr<Application::IValidarProgenitoresUseCase>      validarProgenitoresUC,
        std::shared_ptr<Application::IActualizarFechaPartaMadreUseCase> actualizarPartaMadreUC,
        std::shared_ptr<Application::IGetProduccionUseCase>    getProduccionUC,
        std::shared_ptr<Application::IUpdatePrenezUseCase>     updatePrenezUC,
        std::shared_ptr<Application::IUpdateOrdenoUseCase>     updateOrdenoUC,
        std::shared_ptr<Application::IAddPartoUseCase>         addPartoUC,
        std::shared_ptr<Application::IUpdatePartoUseCase>      updatePartoUC,
        std::shared_ptr<Application::IDeletePartoUseCase>      deletePartoUC,
        std::shared_ptr<Application::IAddRegistroLecheUseCase>    addLecheUC,
        std::shared_ptr<Application::IUpdateRegistroLecheUseCase> updateLecheUC,
        std::shared_ptr<Application::IDeleteRegistroLecheUseCase> deleteLecheUC,
        std::shared_ptr<Application::IAddRegistroCarneUseCase>    addCarneUC,
        std::shared_ptr<Application::IUpdateRegistroCarneUseCase> updateCarneUC,
        std::shared_ptr<Application::IDeleteRegistroCarneUseCase> deleteCarneUC,
        QObject* parent = nullptr
    );

    // ── Propiedades ───────────────────────────────────────────────────────
    [[nodiscard]] QString currentScreen() const;
    [[nodiscard]] bool    isLoggedIn()    const;
    [[nodiscard]] QString userName()      const;

    // ── Slots de navegación ───────────────────────────────────────────────
    Q_INVOKABLE void goToLogin();
    Q_INVOKABLE void goToRegister();
    Q_INVOKABLE void goToMenu();
    Q_INVOKABLE void goToInventario();
    Q_INVOKABLE void goToRegistroReproductivo();
    Q_INVOKABLE void goToRegistroLeche();
    Q_INVOKABLE void goToRegistroPeso();

    // ── Slots de sesión ───────────────────────────────────────────────────
    Q_INVOKABLE bool   login(const QString& email, const QString& contrasena);
    Q_INVOKABLE bool   createAccount(const QString& nombre,
                                     const QString& email,
                                     const QString& contrasena);
    Q_INVOKABLE void   logout();

    // ── Slots de finca ────────────────────────────────────────────────────
    Q_INVOKABLE QVariantList getFincas();
    Q_INVOKABLE QVariantMap  getFinca(const QString& id);
    Q_INVOKABLE bool         createFinca(const QString& nombre,
                                         int nHectareas,
                                         int nPotreros,
                                         int capacidad);
    Q_INVOKABLE bool         updateFinca(const QString& id,
                                         const QString& nombre,
                                         int nHectareas,
                                         int nPotreros,
                                         int capacidad);
    Q_INVOKABLE bool         deleteFinca(const QString& id);
    Q_INVOKABLE bool         fincaTieneAnimales(const QString& idFinca);

    // ── Slots de ganado ───────────────────────────────────────────────────
    Q_INVOKABLE QVariantList getGanadoByFinca(const QString& idFinca);
    Q_INVOKABLE QVariantMap  getGanado(const QString& id);
    Q_INVOKABLE bool         createGanado(const QVariantMap& data);
    Q_INVOKABLE bool         updateGanado(const QVariantMap& data);
    Q_INVOKABLE bool         deleteGanado(const QString& id);
    Q_INVOKABLE QVariantList getAllGanado();
    Q_INVOKABLE QString validarProgenitores(const QString& fechaNacimiento,
                                             const QString& idPadre,
                                             const QString& idMadre);
    Q_INVOKABLE QString getFotoBase64(const QString& id);
    Q_INVOKABLE bool    updateFoto(const QString& id, const QString& base64Data);
    Q_INVOKABLE QString leerArchivoBase64(const QString& filePath);

    // ── Slots de producción ───────────────────────────────────────────────
    Q_INVOKABLE QVariantMap  getProduccion(const QString& id);
    Q_INVOKABLE bool         updatePrenez(const QString& id, bool prenez);
    Q_INVOKABLE bool         updateOrdeno(const QString& id, bool ordeno);
    Q_INVOKABLE bool         addParto(const QString& id,
                                      const QString& partoId,
                                      const QString& fecha);
    Q_INVOKABLE bool         updateParto(const QString& id,
                                         const QString& partoId,
                                         const QString& fecha);
    Q_INVOKABLE bool         deleteParto(const QString& id,
                                         const QString& partoId);
    Q_INVOKABLE bool         addRegistroLeche(const QString& id,
                                              const QString& fecha,
                                              double valor);
    Q_INVOKABLE bool         updateRegistroLeche(const QString& id,
                                                 const QString& fecha,
                                                 double nuevoValor);
    Q_INVOKABLE bool         deleteRegistroLeche(const QString& id,
                                                 const QString& fecha);
    Q_INVOKABLE bool         addRegistroCarne(const QString& id,
                                              const QString& fecha,
                                              double valor);
    Q_INVOKABLE bool         updateRegistroCarne(const QString& id,
                                                 const QString& fecha,
                                                 double nuevoValor);
    Q_INVOKABLE bool         deleteRegistroCarne(const QString& id,
                                                 const QString& fecha);

    // ── Enums para QML ────────────────────────────────────────────────────
    Q_INVOKABLE QStringList getEspecies();
    Q_INVOKABLE QStringList getRazasPorEspecie(const QString& especie);
    Q_INVOKABLE QStringList getSexos();
    Q_INVOKABLE QStringList getEstados();
    

signals:
    void currentScreenChanged();
    void sessionChanged();
    void errorOccurred(const QString& message);

private:
    std::shared_ptr<Application::SessionManager>           m_session;
    std::shared_ptr<Application::ILoginUseCase>            m_loginUC;
    std::shared_ptr<Application::ICreateUsuarioUseCase>    m_createUsuarioUC;
    std::shared_ptr<Application::ICheckEmailExistsUseCase> m_checkEmailUC;
    std::shared_ptr<Application::ICreateFincaUseCase>      m_createFincaUC;
    std::shared_ptr<Application::IGetAllFincasUseCase>     m_getAllFincasUC;
    std::shared_ptr<Application::IGetFincaByIdUseCase>     m_getFincaByIdUC;
    std::shared_ptr<Application::IUpdateFincaUseCase>      m_updateFincaUC;
    std::shared_ptr<Application::IDeleteFincaUseCase>      m_deleteFincaUC;
    std::shared_ptr<Application::ICreateGanadoUseCase>     m_createGanadoUC;
    std::shared_ptr<Application::IGetAllGanadoUseCase>     m_getAllGanadoUC;
    std::shared_ptr<Application::IGetGanadoByIdUseCase>    m_getGanadoByIdUC;
    std::shared_ptr<Application::IUpdateGanadoUseCase>     m_updateGanadoUC;
    std::shared_ptr<Application::IDeleteGanadoUseCase>     m_deleteGanadoUC;
    std::shared_ptr<Application::IGetGanadoByFincaUseCase> m_getByFincaUC;
    std::shared_ptr<Application::IValidarProgenitoresUseCase>       m_validarProgenitoresUC;
    std::shared_ptr<Application::IActualizarFechaPartaMadreUseCase> m_actualizarPartaMadreUC;
    std::shared_ptr<Application::IGetProduccionUseCase>    m_getProduccionUC;
    std::shared_ptr<Application::IUpdatePrenezUseCase>     m_updatePrenezUC;
    std::shared_ptr<Application::IUpdateOrdenoUseCase>     m_updateOrdenoUC;
    std::shared_ptr<Application::IAddPartoUseCase>         m_addPartoUC;
    std::shared_ptr<Application::IUpdatePartoUseCase>      m_updatePartoUC;
    std::shared_ptr<Application::IDeletePartoUseCase>      m_deletePartoUC;
    std::shared_ptr<Application::IAddRegistroLecheUseCase>    m_addLecheUC;
    std::shared_ptr<Application::IUpdateRegistroLecheUseCase> m_updateLecheUC;
    std::shared_ptr<Application::IDeleteRegistroLecheUseCase> m_deleteLecheUC;
    std::shared_ptr<Application::IAddRegistroCarneUseCase>    m_addCarneUC;
    std::shared_ptr<Application::IUpdateRegistroCarneUseCase> m_updateCarneUC;
    std::shared_ptr<Application::IDeleteRegistroCarneUseCase> m_deleteCarneUC;

    QString m_currentScreen;

    void setScreen(const QString& screen);
};

} // namespace Presentation