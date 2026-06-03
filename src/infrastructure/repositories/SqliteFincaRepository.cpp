#include "SqliteFincaRepository.h"
#include <sqlite3.h>
#include <stdexcept>

namespace Infrastructure {

SqliteFincaRepository::SqliteFincaRepository(std::shared_ptr<SQLiteDatabase> db)
    : m_db(std::move(db)) {}

Domain::Finca SqliteFincaRepository::rowToFinca(sqlite3_stmt* stmt) const {
    Domain::Finca f;
    auto col = [&](int i) -> std::string {
        auto* txt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
        return txt ? txt : "";
    };
    f.id         = col(0);
    f.nombre     = col(1);
    f.nHectareas = sqlite3_column_int(stmt, 2);
    f.nPotreros  = sqlite3_column_int(stmt, 3);
    f.capacidad  = sqlite3_column_int(stmt, 4);
    f.idUsuario  = col(5);
    return f;
}

std::vector<Domain::Finca> SqliteFincaRepository::getAll() const {
    std::vector<Domain::Finca> result;
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT id, nombre, n_hectareas, n_potreros, capacidad, id_usuario "
                      "FROM fincas;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("getAll fincas: prepare falló");
    while (sqlite3_step(stmt) == SQLITE_ROW)
        result.push_back(rowToFinca(stmt));
    sqlite3_finalize(stmt);
    return result;
}

std::optional<Domain::Finca> SqliteFincaRepository::getById(const std::string& id) const {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT id, nombre, n_hectareas, n_potreros, capacidad, id_usuario "
                      "FROM fincas WHERE id = ?;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("getById fincas: prepare falló");
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    std::optional<Domain::Finca> result;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        result = rowToFinca(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool SqliteFincaRepository::insert(const Domain::Finca& f) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO fincas (id, nombre, n_hectareas, n_potreros, capacidad, id_usuario) "
        "VALUES (?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;
    sqlite3_bind_text(stmt, 1, f.id.c_str(),        -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, f.nombre.c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 3, f.nHectareas);
    sqlite3_bind_int (stmt, 4, f.nPotreros);
    sqlite3_bind_int (stmt, 5, f.capacidad);
    sqlite3_bind_text(stmt, 6, f.idUsuario.c_str(), -1, SQLITE_STATIC);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool SqliteFincaRepository::update(const Domain::Finca& f) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "UPDATE fincas SET nombre=?, n_hectareas=?, n_potreros=?, capacidad=?, id_usuario=? "
        "WHERE id=?;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;
    sqlite3_bind_text(stmt, 1, f.nombre.c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 2, f.nHectareas);
    sqlite3_bind_int (stmt, 3, f.nPotreros);
    sqlite3_bind_int (stmt, 4, f.capacidad);
    sqlite3_bind_text(stmt, 5, f.idUsuario.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, f.id.c_str(),        -1, SQLITE_STATIC);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool SqliteFincaRepository::deleteById(const std::string& id) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "DELETE FROM fincas WHERE id = ?;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

} // namespace Infrastructure