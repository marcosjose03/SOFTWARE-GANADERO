#include "SessionManager.h"
#include <QStandardPaths>
#include <QString>
#include <fstream>

namespace Application {

std::string SessionManager::sessionFilePath() {
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return (dataDir + "/session.dat").toStdString();
}

void SessionManager::login(const UsuarioResultDto& usuario) {
    m_usuario = usuario;
    persistSession(usuario.id);
}

void SessionManager::logout() {
    m_usuario.reset();
    clearPersistedSession();
}

bool SessionManager::loadSession(const std::string& userId,
                                  const UsuarioResultDto& usuario) {
    if (userId.empty()) return false;
    m_usuario = usuario;
    return true;
}

bool SessionManager::isLoggedIn() const {
    return m_usuario.has_value();
}

std::string SessionManager::userId() const {
    return m_usuario ? m_usuario->id : "";
}

std::string SessionManager::userName() const {
    return m_usuario ? m_usuario->nombre : "";
}

std::string SessionManager::userEmail() const {
    return m_usuario ? m_usuario->email : "";
}

void SessionManager::persistSession(const std::string& userId) {
    std::ofstream file(sessionFilePath());
    if (file.is_open())
        file << userId;
}

void SessionManager::clearPersistedSession() {
    // Usar ofstream para truncar el archivo en lugar de borrarlo
    // Evita el error de archivo en uso en Windows
    std::ofstream file(sessionFilePath(), std::ios::trunc);
    // El archivo queda vacío — al leerlo el próximo arranque
    // savedUserId estará vacío y se irá al login
}

} // namespace Application