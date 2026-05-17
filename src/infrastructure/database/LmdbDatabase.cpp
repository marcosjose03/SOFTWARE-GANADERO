#include "LmdbDatabase.h"
#include <filesystem>

namespace Infrastructure {

LmdbDatabase::LmdbDatabase(const std::string& dirPath, size_t mapSizeMB) {
    std::filesystem::create_directories(dirPath);

    if (mdb_env_create(&m_env) != 0)
        throw std::runtime_error("LMDB: no se pudo crear el entorno");

    mdb_env_set_mapsize(m_env, mapSizeMB * 1024UL * 1024UL);

    if (mdb_env_open(m_env, dirPath.c_str(), 0, 0664) != 0)
        throw std::runtime_error("LMDB: no se pudo abrir el directorio: " + dirPath);

    // Abrir la DBI por defecto en una transacción de escritura
    MDB_txn* txn = nullptr;
    if (mdb_txn_begin(m_env, nullptr, 0, &txn) != 0)
        throw std::runtime_error("LMDB: no se pudo iniciar la transacción de apertura");

    if (mdb_dbi_open(txn, nullptr, 0, &m_dbi) != 0) {
        mdb_txn_abort(txn);
        throw std::runtime_error("LMDB: no se pudo abrir la base de datos");
    }

    mdb_txn_commit(txn);
}

LmdbDatabase::~LmdbDatabase() {
    if (m_env) {
        mdb_dbi_close(m_env, m_dbi);
        mdb_env_close(m_env);
    }
}

} // namespace Infrastructure