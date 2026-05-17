#pragma once
#include "../../domain/repositories/IUsuarioRepository.h"
#include "../database/SQLiteDatabase.h"
#include <memory>

namespace Infrastructure {

class SqliteUsuarioRepository : public Domain::IUsuarioRepository {
public:
    explicit SqliteUsuarioRepository(std::shared_ptr<SQLiteDatabase> db);

    [[nodiscard]] std::vector<Domain::Usuario>   getAll()                             const override;
    [[nodiscard]] std::optional<Domain::Usuario> getById(const std::string& id)       const override;
    bool                                         insert(const Domain::Usuario& u)           override;
    bool                                         update(const Domain::Usuario& u)           override;
    bool                                         deleteById(const std::string& id)           override;

private:
    std::shared_ptr<SQLiteDatabase> m_db;
    [[nodiscard]] Domain::Usuario   rowToUsuario(sqlite3_stmt* stmt) const;
};

} // namespace Infrastructure