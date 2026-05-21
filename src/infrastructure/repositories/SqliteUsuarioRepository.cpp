#include "SqliteUsuarioRepository.h"
#include <sqlite3.h>
#include <stdexcept>

namespace Infrastructure {

SqliteUsuarioRepository::SqliteUsuarioRepository(std::shared_ptr<SQLiteDatabase> db)
    : m_db(std::move(db)) {}

// ─── Helpers ────────────────────────────────────────────────────────────────

Domain::Usuario SqliteUsuarioRepository::rowToUsuario(sqlite3_stmt* stmt) const {
    Domain::Usuario u;
    auto col = [&](int i) -> std::string {
        auto* txt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
        return txt ? txt : "";
    };
    u.id         = col(0);
    u.nombre     = col(1);
    u.email      = col(2);
    u.contrasena = col(3);
    if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) u.fechaRegistro     = col(4);
    if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) u.fechaUltimoBackup = col(5);
    return u;
}

// ─── getAll ─────────────────────────────────────────────────────────────────

std::vector<Domain::Usuario> SqliteUsuarioRepository::getAll() const {
    std::vector<Domain::Usuario> result;
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "SELECT id, nombre, email, contrasena, fecha_registro, fecha_ultimo_backup "
                      "FROM usuarios;";

    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("getAll usuarios: prepare falló");

    while (sqlite3_step(stmt) == SQLITE_ROW)
        result.push_back(rowToUsuario(stmt));

    sqlite3_finalize(stmt);
    return result;
}

// ─── getById ────────────────────────────────────────────────────────────────

std::optional<Domain::Usuario> SqliteUsuarioRepository::getById(const std::string& id) const {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT id, nombre, email, contrasena, fecha_registro, fecha_ultimo_backup "
                      "FROM usuarios WHERE id = ?;";

    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("getById usuarios: prepare falló");

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);

    std::optional<Domain::Usuario> result;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        result = rowToUsuario(stmt);

    sqlite3_finalize(stmt);
    return result;
}

// ─── insert ─────────────────────────────────────────────────────────────────

bool SqliteUsuarioRepository::insert(const Domain::Usuario& u) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO usuarios (id, nombre, email, contrasena, fecha_registro, fecha_ultimo_backup) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, u.id.c_str(),     -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, u.nombre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, u.email.c_str(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, u.contrasena.c_str(), -1, SQLITE_STATIC);
    if (u.fechaRegistro)
        sqlite3_bind_text(stmt, 5, u.fechaRegistro->c_str(), -1, SQLITE_STATIC);
    else
        sqlite3_bind_null(stmt, 5);

    if (u.fechaUltimoBackup)
        sqlite3_bind_text(stmt, 6, u.fechaUltimoBackup->c_str(), -1, SQLITE_STATIC);
    else
        sqlite3_bind_null(stmt, 6);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

// ─── update ─────────────────────────────────────────────────────────────────

bool SqliteUsuarioRepository::update(const Domain::Usuario& u) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "UPDATE usuarios SET nombre=?, email=?, contrasena=?, "
        "fecha_registro=?, fecha_ultimo_backup=? WHERE id=?;";

    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, u.nombre.c_str(),     -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, u.email.c_str(),      -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, u.contrasena.c_str(), -1, SQLITE_STATIC);

    if (u.fechaRegistro)
        sqlite3_bind_text(stmt, 4, u.fechaRegistro->c_str(), -1, SQLITE_STATIC);
    else
        sqlite3_bind_null(stmt, 4);

    if (u.fechaUltimoBackup)
        sqlite3_bind_text(stmt, 5, u.fechaUltimoBackup->c_str(), -1, SQLITE_STATIC);
    else
        sqlite3_bind_null(stmt, 5);

    sqlite3_bind_text(stmt, 6, u.id.c_str(), -1, SQLITE_STATIC);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

// ─── deleteById ─────────────────────────────────────────────────────────────

bool SqliteUsuarioRepository::deleteById(const std::string& id) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "DELETE FROM usuarios WHERE id = ?;";

    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

} // namespace Infrastructure