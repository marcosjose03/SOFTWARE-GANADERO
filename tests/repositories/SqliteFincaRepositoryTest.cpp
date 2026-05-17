#include <gtest/gtest.h>
#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "infrastructure/repositories/SqliteFincaRepository.h"

using namespace Infrastructure;
using namespace Domain;

class SqliteFincaRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        db = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();

        // Finca necesita un usuario existente por foreign key
        usuarioRepo = std::make_shared<SqliteUsuarioRepository>(db);
        usuarioRepo->insert(Usuario{
            .id     = "user-001",
            .nombre = "Dueño",
            .email  = "dueno@test.com"
        });

        repo = std::make_shared<SqliteFincaRepository>(db);
    }

    std::shared_ptr<SQLiteDatabase>          db;
    std::shared_ptr<SqliteUsuarioRepository> usuarioRepo;
    std::shared_ptr<SqliteFincaRepository>   repo;

    Finca makeFinca(const std::string& id = "finca-001") {
        return Finca{
            .id         = id,
            .nombre     = "La Esperanza",
            .nHectareas = 150,
            .nPotreros  = 8,
            .capacidad  = 200,
            .idUsuario  = "user-001"
        };
    }
};

TEST_F(SqliteFincaRepositoryTest, InsertAndGetById) {
    ASSERT_TRUE(repo->insert(makeFinca()));

    auto result = repo->getById("finca-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->nombre,     "La Esperanza");
    EXPECT_EQ(result->nHectareas, 150);
    EXPECT_EQ(result->nPotreros,  8);
    EXPECT_EQ(result->capacidad,  200);
    EXPECT_EQ(result->idUsuario,  "user-001");
}

TEST_F(SqliteFincaRepositoryTest, GetByIdNotFound) {
    EXPECT_FALSE(repo->getById("no-existe").has_value());
}

TEST_F(SqliteFincaRepositoryTest, GetAll) {
    repo->insert(makeFinca("f1"));
    repo->insert(makeFinca("f2"));

    EXPECT_EQ(repo->getAll().size(), 2u);
}

TEST_F(SqliteFincaRepositoryTest, Update) {
    repo->insert(makeFinca());

    Finca updated{
        .id         = "finca-001",
        .nombre     = "La Nueva Esperanza",
        .nHectareas = 200,
        .nPotreros  = 10,
        .capacidad  = 300,
        .idUsuario  = "user-001"
    };
    ASSERT_TRUE(repo->update(updated));

    auto result = repo->getById("finca-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->nombre,     "La Nueva Esperanza");
    EXPECT_EQ(result->nHectareas, 200);
}

TEST_F(SqliteFincaRepositoryTest, DeleteById) {
    repo->insert(makeFinca());
    ASSERT_TRUE(repo->deleteById("finca-001"));
    EXPECT_FALSE(repo->getById("finca-001").has_value());
}

TEST_F(SqliteFincaRepositoryTest, InsertDuplicateIdFails) {
    repo->insert(makeFinca());
    EXPECT_FALSE(repo->insert(makeFinca()));
}