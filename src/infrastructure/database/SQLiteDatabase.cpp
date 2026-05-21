#include "SQLiteDatabase.h"
#include <stdexcept>

namespace Infrastructure {

SQLiteDatabase::SQLiteDatabase(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &m_db) != SQLITE_OK) {
        throw std::runtime_error("No se pudo abrir la base de datos: " +
                                 std::string(sqlite3_errmsg(m_db)));
    }
    // Activar WAL y foreign keys
    execute("PRAGMA journal_mode=WAL;");
    execute("PRAGMA foreign_keys=ON;");
}

SQLiteDatabase::~SQLiteDatabase() {
    if (m_db) sqlite3_close(m_db);
}

void SQLiteDatabase::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string err(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error("SQLite error: " + err);
    }
}

void SQLiteDatabase::initSchema() {
    execute(R"(
        CREATE TABLE IF NOT EXISTS usuarios (
            id                  TEXT NOT NULL PRIMARY KEY,
            nombre              TEXT NOT NULL,
            email               TEXT NOT NULL,
            contrasena          TEXT NOT NULL,
            fecha_registro      TEXT,
            fecha_ultimo_backup TEXT
        );
    )");

    execute(R"(
        CREATE TABLE IF NOT EXISTS fincas (
            id          TEXT NOT NULL PRIMARY KEY,
            nombre      TEXT NOT NULL,
            n_hectareas INTEGER NOT NULL,
            n_potreros  INTEGER NOT NULL,
            capacidad   INTEGER NOT NULL,
            id_usuario  TEXT NOT NULL,
            FOREIGN KEY (id_usuario) REFERENCES usuarios(id)
        );
    )");

    execute(R"(
        CREATE TABLE IF NOT EXISTS ganado (
            id                    TEXT NOT NULL PRIMARY KEY,
            especie               TEXT NOT NULL,
            identificador         INTEGER NOT NULL,
            id_usuario            TEXT NOT NULL,
            id_finca              TEXT NOT NULL,
            nacimiento            TEXT NOT NULL,
            sexo                  TEXT NOT NULL,
            estado                TEXT NOT NULL,
            raza                  TEXT,
            id_padre              TEXT REFERENCES ganado(id),
            id_madre              TEXT REFERENCES ganado(id),
            chapeta               TEXT,
            fecha_destete         TEXT,
            foto                  BLOB,
            fecha_ultimo_parto    TEXT,
            fecha_ultima_palpacion TEXT,
            fecha_inseminacion    TEXT,
            FOREIGN KEY (id_usuario) REFERENCES usuarios(id),
            FOREIGN KEY (id_finca)   REFERENCES fincas(id)
        );
    )");
}

} // namespace Infrastructure