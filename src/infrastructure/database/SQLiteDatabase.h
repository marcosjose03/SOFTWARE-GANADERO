#pragma once
#include <sqlite3.h>
#include <string>
#include <stdexcept>

namespace Infrastructure {

class SQLiteDatabase {
public:
    explicit SQLiteDatabase(const std::string& dbPath);
    ~SQLiteDatabase();

    // No copiable
    SQLiteDatabase(const SQLiteDatabase&)            = delete;
    SQLiteDatabase& operator=(const SQLiteDatabase&) = delete;

    [[nodiscard]] sqlite3* handle() const { return m_db; }

    void initSchema();

private:
    sqlite3* m_db = nullptr;

    void execute(const std::string& sql);
};

} // namespace Infrastructure