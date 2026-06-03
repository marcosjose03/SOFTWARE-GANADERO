#include <gtest/gtest.h>
#include <filesystem>
#include "infrastructure/database/LmdbDatabase.h"
#include "infrastructure/repositories/LmdbProduccionRepository.h"

using namespace Infrastructure;
using namespace Domain;

class LmdbProduccionRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Directorio temporal único por test
        testDir = std::filesystem::temp_directory_path() / "ganadero_test_lmdb";
        std::filesystem::remove_all(testDir);

        db   = std::make_shared<LmdbDatabase>(testDir.string());
        repo = std::make_shared<LmdbProduccionRepository>(db);
    }

    void TearDown() override {
        db.reset();
        repo.reset();
        std::filesystem::remove_all(testDir);
    }

    std::shared_ptr<LmdbDatabase>             db;
    std::shared_ptr<LmdbProduccionRepository> repo;
    std::filesystem::path                     testDir;

    Produccion makeProduccion(const std::string& id = "prod-001") {
        return Produccion{
            .id           = id,
            .prenez       = false,
            .ordeno       = false,
            .partos       = {},
            .registroLeche = {},
            .registroCarne = {}
        };
    }
};

// ─── CRUD principal ──────────────────────────────────────────────────────────

TEST_F(LmdbProduccionRepositoryTest, InsertAndGetById) {
    auto p = makeProduccion();
    ASSERT_TRUE(repo->insert(p));

    auto result = repo->getById("prod-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id,     "prod-001");
    EXPECT_FALSE(result->prenez);
    EXPECT_FALSE(result->ordeno);
}

TEST_F(LmdbProduccionRepositoryTest, GetByIdNotFound) {
    EXPECT_FALSE(repo->getById("no-existe").has_value());
}

TEST_F(LmdbProduccionRepositoryTest, DeleteById) {
    repo->insert(makeProduccion());
    ASSERT_TRUE(repo->deleteById("prod-001"));
    EXPECT_FALSE(repo->getById("prod-001").has_value());
}

// ─── Campos simples ──────────────────────────────────────────────────────────

TEST_F(LmdbProduccionRepositoryTest, UpdatePrenez) {
    repo->insert(makeProduccion());
    ASSERT_TRUE(repo->updatePrenez("prod-001", true));

    auto result = repo->getById("prod-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->prenez);
}

TEST_F(LmdbProduccionRepositoryTest, UpdateOrdeno) {
    repo->insert(makeProduccion());
    ASSERT_TRUE(repo->updateOrdeno("prod-001", true));

    auto result = repo->getById("prod-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->ordeno);
}

TEST_F(LmdbProduccionRepositoryTest, UpdatePrenezNotFound) {
    EXPECT_FALSE(repo->updatePrenez("no-existe", true));
}

// ─── Partos ──────────────────────────────────────────────────────────────────

TEST_F(LmdbProduccionRepositoryTest, AddParto) {
    repo->insert(makeProduccion());
    Parto parto{"parto-001", "2024-03-10"};

    ASSERT_TRUE(repo->addParto("prod-001", parto));

    auto result = repo->getById("prod-001");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->partos.size(), 1u);
    EXPECT_EQ(result->partos[0].id,    "parto-001");
    EXPECT_EQ(result->partos[0].fecha, "2024-03-10");
}

TEST_F(LmdbProduccionRepositoryTest, UpdateParto) {
    repo->insert(makeProduccion());
    repo->addParto("prod-001", {"parto-001", "2024-03-10"});

    ASSERT_TRUE(repo->updateParto("prod-001", {"parto-001", "2024-04-15"}));

    auto result = repo->getById("prod-001");
    EXPECT_EQ(result->partos[0].fecha, "2024-04-15");
}

TEST_F(LmdbProduccionRepositoryTest, DeleteParto) {
    repo->insert(makeProduccion());
    repo->addParto("prod-001", {"parto-001", "2024-03-10"});
    repo->addParto("prod-001", {"parto-002", "2025-01-18"});

    ASSERT_TRUE(repo->deleteParto("prod-001", "parto-001"));

    auto result = repo->getById("prod-001");
    ASSERT_EQ(result->partos.size(), 1u);
    EXPECT_EQ(result->partos[0].id, "parto-002");
}

TEST_F(LmdbProduccionRepositoryTest, DeletePartoNotFound) {
    repo->insert(makeProduccion());
    EXPECT_FALSE(repo->deleteParto("prod-001", "no-existe"));
}

// ─── Registro leche ──────────────────────────────────────────────────────────

TEST_F(LmdbProduccionRepositoryTest, AddRegistroLeche) {
    repo->insert(makeProduccion());
    ASSERT_TRUE(repo->addRegistroLeche("prod-001", {"2024-03-11", 18.5}));

    auto result = repo->getById("prod-001");
    ASSERT_EQ(result->registroLeche.size(), 1u);
    EXPECT_DOUBLE_EQ(result->registroLeche[0].valor, 18.5);
}

TEST_F(LmdbProduccionRepositoryTest, UpdateRegistroLeche) {
    repo->insert(makeProduccion());
    repo->addRegistroLeche("prod-001", {"2024-03-11", 18.5});

    ASSERT_TRUE(repo->updateRegistroLeche("prod-001", "2024-03-11", 21.0));

    auto result = repo->getById("prod-001");
    EXPECT_DOUBLE_EQ(result->registroLeche[0].valor, 21.0);
}

TEST_F(LmdbProduccionRepositoryTest, DeleteRegistroLeche) {
    repo->insert(makeProduccion());
    repo->addRegistroLeche("prod-001", {"2024-03-11", 18.5});
    repo->addRegistroLeche("prod-001", {"2024-03-12", 19.2});

    ASSERT_TRUE(repo->deleteRegistroLeche("prod-001", "2024-03-11"));

    auto result = repo->getById("prod-001");
    ASSERT_EQ(result->registroLeche.size(), 1u);
    EXPECT_EQ(result->registroLeche[0].fecha, "2024-03-12");
}

// ─── Registro carne ──────────────────────────────────────────────────────────

TEST_F(LmdbProduccionRepositoryTest, AddRegistroCarne) {
    repo->insert(makeProduccion());
    ASSERT_TRUE(repo->addRegistroCarne("prod-001", {"2025-02-01", 320.5}));

    auto result = repo->getById("prod-001");
    ASSERT_EQ(result->registroCarne.size(), 1u);
    EXPECT_DOUBLE_EQ(result->registroCarne[0].valor, 320.5);
}

TEST_F(LmdbProduccionRepositoryTest, UpdateRegistroCarne) {
    repo->insert(makeProduccion());
    repo->addRegistroCarne("prod-001", {"2025-02-01", 320.5});

    ASSERT_TRUE(repo->updateRegistroCarne("prod-001", "2025-02-01", 350.0));

    auto result = repo->getById("prod-001");
    EXPECT_DOUBLE_EQ(result->registroCarne[0].valor, 350.0);
}

TEST_F(LmdbProduccionRepositoryTest, DeleteRegistroCarne) {
    repo->insert(makeProduccion());
    repo->addRegistroCarne("prod-001", {"2025-02-01", 320.5});
    repo->addRegistroCarne("prod-001", {"2025-03-01", 340.0});

    ASSERT_TRUE(repo->deleteRegistroCarne("prod-001", "2025-02-01"));

    auto result = repo->getById("prod-001");
    ASSERT_EQ(result->registroCarne.size(), 1u);
    EXPECT_EQ(result->registroCarne[0].fecha, "2025-03-01");
}