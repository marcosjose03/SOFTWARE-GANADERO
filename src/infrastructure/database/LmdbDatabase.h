#pragma once
#include <lmdb.h>
#include <string>
#include <stdexcept>

namespace Infrastructure {

class LmdbDatabase {
public:
    explicit LmdbDatabase(const std::string& dirPath, size_t mapSizeMB = 256);
    ~LmdbDatabase();

    LmdbDatabase(const LmdbDatabase&)            = delete;
    LmdbDatabase& operator=(const LmdbDatabase&) = delete;

    [[nodiscard]] MDB_env* env()   const { return m_env; }
    [[nodiscard]] MDB_dbi  dbi()   const { return m_dbi; }

private:
    MDB_env* m_env = nullptr;
    MDB_dbi  m_dbi = 0;
};

} // namespace Infrastructure