#include <gtest/gtest.h>
#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "infrastructure/repositories/SqliteFincaRepository.h"
#include "infrastructure/repositories/SqliteGanadoRepository.h"

using namespace Infrastructure;
using namespace Domain;

class SqliteGanadoRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        db = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();

        auto usuarioRepo = std::make_shared<SqliteUsuarioRepository>(db);
        usuarioRepo->insert(Usuario{
            .id     = "user-001",
            .nombre = "Dueño",
            .email  = "dueno@test.com"
        });

        auto fincaRepo = std::make_shared<SqliteFincaRepository>(db);
        fincaRepo->insert(Finca{
            .id         = "finca-001",
            .nombre     = "La Esperanza",
            .nHectareas = 100,
            .nPotreros  = 5,
            .capacidad  = 100,
            .idUsuario  = "user-001"
        });

        repo = std::make_shared<SqliteGanadoRepository>(db);
    }

    std::shared_ptr<SQLiteDatabase>         db;
    std::shared_ptr<SqliteGanadoRepository> repo;

    Ganado makeGanado(const std::string& id = "ganado-001") {
        return Ganado{
            .id            = id,
            .especie       = "Bovino",
            .identificador = 101,
            .idUsuario     = "user-001",
            .idFinca       = "finca-001",
            .nacimiento    = "2020-05-10",
            .sexo          = "Hembra",
            .estado        = "Activo",
            .raza          = "Holstein",
            .idPadre       = std::nullopt,
            .idMadre       = std::nullopt,
            .chapeta        = "A-101",
            .fechaDestete  = std::nullopt,
            .foto          = std::nullopt,
            .fechaUltimoParto     = std::nullopt,
            .fechaUltimaPalpacion = std::nullopt,
            .fechaInseminacion    = std::nullopt
        };
    }
};

TEST_F(SqliteGanadoRepositoryTest, InsertAndGetById) {
    ASSERT_TRUE(repo->insert(makeGanado()));

    auto result = repo->getById("ganado-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->especie,       "Bovino");
    EXPECT_EQ(result->identificador, 101);
    EXPECT_EQ(result->sexo,          "Hembra");
    EXPECT_EQ(result->raza,          "Holstein");
}

TEST_F(SqliteGanadoRepositoryTest, GetByIdNotFound) {
    EXPECT_FALSE(repo->getById("no-existe").has_value());
}

TEST_F(SqliteGanadoRepositoryTest, GetAll) {
    repo->insert(makeGanado("g1"));
    repo->insert(makeGanado("g2"));
    repo->insert(makeGanado("g3"));

    EXPECT_EQ(repo->getAll().size(), 3u);
}

TEST_F(SqliteGanadoRepositoryTest, Update) {
    repo->insert(makeGanado());

    auto g        = makeGanado();
    g.estado      = "Vendido";
    g.raza        = "Angus";
    g.fechaDestete = "2020-11-10";

    ASSERT_TRUE(repo->update(g));

    auto result = repo->getById("ganado-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->estado,       "Vendido");
    EXPECT_EQ(result->raza,         "Angus");
    EXPECT_EQ(result->fechaDestete, "2020-11-10");
}

TEST_F(SqliteGanadoRepositoryTest, DeleteById) {
    repo->insert(makeGanado());
    ASSERT_TRUE(repo->deleteById("ganado-001"));
    EXPECT_FALSE(repo->getById("ganado-001").has_value());
}

TEST_F(SqliteGanadoRepositoryTest, OptionalFieldsNullable) {
    auto g  = makeGanado("g-null");
    g.raza  = std::nullopt;
    g.chapeta = std::nullopt;

    ASSERT_TRUE(repo->insert(g));

    auto result = repo->getById("g-null");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result->raza.has_value());
    EXPECT_FALSE(result->chapeta.has_value());
}

TEST_F(SqliteGanadoRepositoryTest, InsertWithFoto) {
    auto g = makeGanado("g-foto");
    g.foto = std::vector<uint8_t>{0x89, 0x50, 0x4E, 0x47}; // PNG header

    ASSERT_TRUE(repo->insert(g));

    auto result = repo->getById("g-foto");
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->foto.has_value());
    EXPECT_EQ(result->foto->size(), 4u);
    EXPECT_EQ((*result->foto)[0],   0x89);
}