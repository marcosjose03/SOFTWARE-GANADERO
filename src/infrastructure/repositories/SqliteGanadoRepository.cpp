#include "SqliteGanadoRepository.h"
#include <sqlite3.h>
#include <stdexcept>

namespace Infrastructure {

SqliteGanadoRepository::SqliteGanadoRepository(std::shared_ptr<SQLiteDatabase> db)
    : m_db(std::move(db)) {}

// ─── Helpers ────────────────────────────────────────────────────────────────

void SqliteGanadoRepository::bindOptionalText(sqlite3_stmt* stmt, int col,
                                              const std::optional<std::string>& val) const {
    if (val)
        sqlite3_bind_text(stmt, col, val->c_str(), -1, SQLITE_STATIC);
    else
        sqlite3_bind_null(stmt, col);
}

Domain::Ganado SqliteGanadoRepository::rowToGanado(sqlite3_stmt* stmt) const {
    Domain::Ganado g;
    auto col = [&](int i) -> std::string {
        auto* txt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
        return txt ? txt : "";
    };
    auto optCol = [&](int i) -> std::optional<std::string> {
        if (sqlite3_column_type(stmt, i) == SQLITE_NULL) return std::nullopt;
        return col(i);
    };

    g.id            = col(0);
    g.especie       = col(1);
    g.identificador = sqlite3_column_int(stmt, 2);
    g.idUsuario     = col(3);
    g.idFinca       = col(4);
    g.nacimiento    = col(5);
    g.sexo          = col(6);
    g.estado        = col(7);
    g.raza          = optCol(8);
    g.idPadre       = optCol(9);
    g.idMadre       = optCol(10);
    g.chapeta       = optCol(11);
    g.fechaDestete  = optCol(12);

    // foto (BLOB) — columna 13
    if (sqlite3_column_type(stmt, 13) != SQLITE_NULL) {
        auto* blob = static_cast<const uint8_t*>(sqlite3_column_blob(stmt, 13));
        int   size = sqlite3_column_bytes(stmt, 13);
        g.foto = std::vector<uint8_t>(blob, blob + size);
    }

    g.fechaUltimoParto      = optCol(14);
    g.fechaUltimaPalpacion  = optCol(15);
    g.fechaInseminacion     = optCol(16);

    return g;
}

// ─── getAll ─────────────────────────────────────────────────────────────────

std::vector<Domain::Ganado> SqliteGanadoRepository::getAll() const {
    std::vector<Domain::Ganado> result;
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "SELECT id, especie, identificador, id_usuario, id_finca, nacimiento, sexo, estado, "
        "raza, id_padre, id_madre, chapeta, fecha_destete, foto, "
        "fecha_ultimo_parto, fecha_ultima_palpacion, fecha_inseminacion "
        "FROM ganado;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("getAll ganado: prepare falló");
    while (sqlite3_step(stmt) == SQLITE_ROW)
        result.push_back(rowToGanado(stmt));
    sqlite3_finalize(stmt);
    return result;
}

// ─── getById ────────────────────────────────────────────────────────────────

std::optional<Domain::Ganado> SqliteGanadoRepository::getById(const std::string& id) const {
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "SELECT id, especie, identificador, id_usuario, id_finca, nacimiento, sexo, estado, "
        "raza, id_padre, id_madre, chapeta, fecha_destete, foto, "
        "fecha_ultimo_parto, fecha_ultima_palpacion, fecha_inseminacion "
        "FROM ganado WHERE id = ?;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("getById ganado: prepare falló");
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    std::optional<Domain::Ganado> result;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        result = rowToGanado(stmt);
    sqlite3_finalize(stmt);
    return result;
}

// ─── insert ─────────────────────────────────────────────────────────────────

bool SqliteGanadoRepository::insert(const Domain::Ganado& g) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO ganado (id, especie, identificador, id_usuario, id_finca, nacimiento, "
        "sexo, estado, raza, id_padre, id_madre, chapeta, fecha_destete, foto, "
        "fecha_ultimo_parto, fecha_ultima_palpacion, fecha_inseminacion) "
        "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt,  1, g.id.c_str(),          -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  2, g.especie.c_str(),      -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt,  3, g.identificador);
    sqlite3_bind_text(stmt,  4, g.idUsuario.c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  5, g.idFinca.c_str(),      -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  6, g.nacimiento.c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  7, g.sexo.c_str(),         -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  8, g.estado.c_str(),       -1, SQLITE_STATIC);
    bindOptionalText (stmt,  9, g.raza);
    bindOptionalText (stmt, 10, g.idPadre);
    bindOptionalText (stmt, 11, g.idMadre);
    bindOptionalText (stmt, 12, g.chapeta);
    bindOptionalText (stmt, 13, g.fechaDestete);

    if (g.foto)
        sqlite3_bind_blob(stmt, 14, g.foto->data(), static_cast<int>(g.foto->size()), SQLITE_STATIC);
    else
        sqlite3_bind_null(stmt, 14);

    bindOptionalText(stmt, 15, g.fechaUltimoParto);
    bindOptionalText(stmt, 16, g.fechaUltimaPalpacion);
    bindOptionalText(stmt, 17, g.fechaInseminacion);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

// ─── update ─────────────────────────────────────────────────────────────────

bool SqliteGanadoRepository::update(const Domain::Ganado& g) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "UPDATE ganado SET especie=?, identificador=?, id_usuario=?, id_finca=?, "
        "nacimiento=?, sexo=?, estado=?, raza=?, id_padre=?, id_madre=?, chapeta=?, "
        "fecha_destete=?, foto=?, fecha_ultimo_parto=?, fecha_ultima_palpacion=?, "
        "fecha_inseminacion=? WHERE id=?;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt,  1, g.especie.c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt,  2, g.identificador);
    sqlite3_bind_text(stmt,  3, g.idUsuario.c_str(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  4, g.idFinca.c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  5, g.nacimiento.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  6, g.sexo.c_str(),       -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  7, g.estado.c_str(),     -1, SQLITE_STATIC);
    bindOptionalText (stmt,  8, g.raza);
    bindOptionalText (stmt,  9, g.idPadre);
    bindOptionalText (stmt, 10, g.idMadre);
    bindOptionalText (stmt, 11, g.chapeta);
    bindOptionalText (stmt, 12, g.fechaDestete);

    if (g.foto)
        sqlite3_bind_blob(stmt, 13, g.foto->data(), static_cast<int>(g.foto->size()), SQLITE_STATIC);
    else
        sqlite3_bind_null(stmt, 13);

    bindOptionalText(stmt, 14, g.fechaUltimoParto);
    bindOptionalText(stmt, 15, g.fechaUltimaPalpacion);
    bindOptionalText(stmt, 16, g.fechaInseminacion);
    sqlite3_bind_text(stmt, 17, g.id.c_str(), -1, SQLITE_STATIC);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

// ─── deleteById ─────────────────────────────────────────────────────────────

bool SqliteGanadoRepository::deleteById(const std::string& id) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "DELETE FROM ganado WHERE id = ?;";
    if (sqlite3_prepare_v2(m_db->handle(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

} // namespace Infrastructure