#pragma once
#include "../../domain/repositories/IFincaRepository.h"
#include "../database/SQLiteDatabase.h"
#include <memory>

namespace Infrastructure {

class SqliteFincaRepository : public Domain::IFincaRepository {
public:
    explicit SqliteFincaRepository(std::shared_ptr<SQLiteDatabase> db);

    [[nodiscard]] std::vector<Domain::Finca>   getAll()                           const override;
    [[nodiscard]] std::optional<Domain::Finca> getById(const std::string& id)     const override;
    bool                                       insert(const Domain::Finca& f)           override;
    bool                                       update(const Domain::Finca& f)           override;
    bool                                       deleteById(const std::string& id)         override;

private:
    std::shared_ptr<SQLiteDatabase> m_db;
    [[nodiscard]] Domain::Finca     rowToFinca(sqlite3_stmt* stmt) const;
};

} // namespace Infrastructure