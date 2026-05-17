#pragma once
#include "../../domain/repositories/IGanadoRepository.h"
#include "../database/SQLiteDatabase.h"
#include <memory>

namespace Infrastructure {

class SqliteGanadoRepository : public Domain::IGanadoRepository {
public:
    explicit SqliteGanadoRepository(std::shared_ptr<SQLiteDatabase> db);

    [[nodiscard]] std::vector<Domain::Ganado>   getAll()                            const override;
    [[nodiscard]] std::optional<Domain::Ganado> getById(const std::string& id)      const override;
    bool                                        insert(const Domain::Ganado& g)           override;
    bool                                        update(const Domain::Ganado& g)           override;
    bool                                        deleteById(const std::string& id)          override;

private:
    std::shared_ptr<SQLiteDatabase> m_db;
    [[nodiscard]] Domain::Ganado    rowToGanado(sqlite3_stmt* stmt) const;

    void bindOptionalText(sqlite3_stmt* stmt, int col,
                          const std::optional<std::string>& val) const;
};

} // namespace Infrastructure