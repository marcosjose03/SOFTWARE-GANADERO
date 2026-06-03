#include <gtest/gtest.h>
#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"

using namespace Infrastructure;
using namespace Domain;

// ─── Fixture ────────────────────────────────────────────────────────────────

class SqliteUsuarioRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Base de datos en memoria — se destruye al cerrar
        db   = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();
        repo = std::make_shared<SqliteUsuarioRepository>(db);
    }

    std::shared_ptr<SQLiteDatabase>          db;
    std::shared_ptr<SqliteUsuarioRepository> repo;

    Usuario makeUsuario(const std::string& id = "user-001") {
        return Usuario{
            .id                = id,
            .nombre            = "Juan Pérez",
            .email             = "juan@test.com",
            .contrasena        = "pass123",
            .fechaRegistro     = "2024-01-01",
            .fechaUltimoBackup = std::nullopt
        };
    }
};

// ─── Tests ──────────────────────────────────────────────────────────────────

TEST_F(SqliteUsuarioRepositoryTest, InsertAndGetById) {
    auto u = makeUsuario();
    ASSERT_TRUE(repo->insert(u));

    auto result = repo->getById("user-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id,     "user-001");
    EXPECT_EQ(result->nombre, "Juan Pérez");
    EXPECT_EQ(result->email,  "juan@test.com");
}

TEST_F(SqliteUsuarioRepositoryTest, GetByIdNotFound) {
    auto result = repo->getById("no-existe");
    EXPECT_FALSE(result.has_value());
}

TEST_F(SqliteUsuarioRepositoryTest, GetAll) {
    repo->insert(makeUsuario("u1"));
    repo->insert(makeUsuario("u2"));
    repo->insert(makeUsuario("u3"));

    auto all = repo->getAll();
    EXPECT_EQ(all.size(), 3u);
}

TEST_F(SqliteUsuarioRepositoryTest, GetAllEmpty) {
    auto all = repo->getAll();
    EXPECT_TRUE(all.empty());
}

TEST_F(SqliteUsuarioRepositoryTest, Update) {
    repo->insert(makeUsuario());

    Usuario updated{
        .id                = "user-001",
        .nombre            = "Juan Actualizado",
        .email             = "nuevo@test.com",
        .contrasena        = "pass123",
        .fechaRegistro     = std::optional<std::string>{"2024-01-01"},
        .fechaUltimoBackup = std::optional<std::string>{"2024-06-01"}
    };
    ASSERT_TRUE(repo->update(updated));

    auto result = repo->getById("user-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->nombre,            "Juan Actualizado");
    EXPECT_EQ(result->email,             "nuevo@test.com");
    EXPECT_EQ(result->fechaUltimoBackup, "2024-06-01");
}

TEST_F(SqliteUsuarioRepositoryTest, DeleteById) {
    repo->insert(makeUsuario());
    ASSERT_TRUE(repo->deleteById("user-001"));

    auto result = repo->getById("user-001");
    EXPECT_FALSE(result.has_value());
}

TEST_F(SqliteUsuarioRepositoryTest, DeleteNonExistent) {
    bool ok = repo->deleteById("no-existe");
    // No debe lanzar excepción — simplemente retorna false o true sin filas afectadas
    SUCCEED();
}

TEST_F(SqliteUsuarioRepositoryTest, InsertDuplicateIdFails) {
    repo->insert(makeUsuario());
    bool second = repo->insert(makeUsuario()); // mismo id
    EXPECT_FALSE(second);
}

TEST_F(SqliteUsuarioRepositoryTest, OptionalFieldsNullable) {
    Usuario u{
        .id                = "user-002",
        .nombre            = "Sin fechas",
        .email             = "test@test.com",
        .contrasena        = "pass123",
        .fechaRegistro     = std::nullopt,
        .fechaUltimoBackup = std::nullopt
    };
    ASSERT_TRUE(repo->insert(u));

    auto result = repo->getById("user-002");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result->fechaRegistro.has_value());
    EXPECT_FALSE(result->fechaUltimoBackup.has_value());
}

TEST_F(SqliteUsuarioRepositoryTest, ContrasenaSeGuardaCorrectamente) {
    repo->insert(makeUsuario());
    auto result = repo->getById("user-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->contrasena, "pass123");
}