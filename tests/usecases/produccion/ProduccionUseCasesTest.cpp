#include <gtest/gtest.h>
#include <filesystem>
#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/database/LmdbDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "infrastructure/repositories/SqliteFincaRepository.h"
#include "infrastructure/repositories/SqliteGanadoRepository.h"
#include "infrastructure/repositories/LmdbProduccionRepository.h"
#include "application/usecases/usuario/UsuarioUseCases.h"
#include "application/usecases/finca/FincaUseCases.h"
#include "application/usecases/ganado/GanadoUseCases.h"
#include "application/usecases/produccion/ProduccionUseCases.h"
#include "infrastructure/utils/UuidGenerator.h"

using namespace Infrastructure;
using namespace Application;

class ProduccionUseCasesTest : public ::testing::Test {
protected:
    void SetUp() override {
        db = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();

        testDir = std::filesystem::temp_directory_path() / "ganadero_prod_uc_test";
        std::filesystem::remove_all(testDir);
        lmdb = std::make_shared<LmdbDatabase>(testDir.string());

        auto usuarioRepo = std::make_shared<SqliteUsuarioRepository>(db);
        auto fincaRepo   = std::make_shared<SqliteFincaRepository>(db);
        ganadoRepo       = std::make_shared<SqliteGanadoRepository>(db);
        produccionRepo   = std::make_shared<LmdbProduccionRepository>(lmdb);

        auto u  = CreateUsuarioUseCase(usuarioRepo).execute({"Dueño","d@t.com","pass"});
        auto f  = CreateFincaUseCase(fincaRepo).execute({"Finca",100,5,100,u->id});

        auto animal = CreateGanadoUseCase(ganadoRepo, produccionRepo).execute({
            Domain::Especie::Bovino, "101", u->id, f->id,
            "2020-01-01", Domain::SexoGanado::Hembra,
            Domain::EstadoGanado::Activo, "Holstein Negro",
            std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt
        });
        idGanado = animal->id;

        getUC            = std::make_shared<GetProduccionUseCase>(produccionRepo);
        prenezUC         = std::make_shared<UpdatePrenezUseCase>(produccionRepo);
        ordenoUC         = std::make_shared<UpdateOrdenoUseCase>(produccionRepo);
        addPartoUC       = std::make_shared<AddPartoUseCase>(produccionRepo, ganadoRepo);
        updatePartoUC    = std::make_shared<UpdatePartoUseCase>(produccionRepo);
        deletePartoUC    = std::make_shared<DeletePartoUseCase>(produccionRepo);
        addLecheUC       = std::make_shared<AddRegistroLecheUseCase>(produccionRepo);
        updateLecheUC    = std::make_shared<UpdateRegistroLecheUseCase>(produccionRepo);
        deleteLecheUC    = std::make_shared<DeleteRegistroLecheUseCase>(produccionRepo);
        addCarneUC       = std::make_shared<AddRegistroCarneUseCase>(produccionRepo);
        updateCarneUC    = std::make_shared<UpdateRegistroCarneUseCase>(produccionRepo);
        deleteCarneUC    = std::make_shared<DeleteRegistroCarneUseCase>(produccionRepo);
    }

    void TearDown() override {
    getUC.reset();
    prenezUC.reset();
    ordenoUC.reset();
    addPartoUC.reset();
    updatePartoUC.reset();
    deletePartoUC.reset();
    addLecheUC.reset();
    updateLecheUC.reset();
    deleteLecheUC.reset();
    addCarneUC.reset();
    updateCarneUC.reset();
    deleteCarneUC.reset();
    produccionRepo.reset();
    ganadoRepo.reset();
    lmdb.reset();
    std::filesystem::remove_all(testDir);
}

    std::shared_ptr<SQLiteDatabase>              db;
    std::shared_ptr<LmdbDatabase>                lmdb;
    std::shared_ptr<SqliteGanadoRepository>      ganadoRepo;
    std::shared_ptr<LmdbProduccionRepository>    produccionRepo;
    std::shared_ptr<GetProduccionUseCase>        getUC;
    std::shared_ptr<UpdatePrenezUseCase>         prenezUC;
    std::shared_ptr<UpdateOrdenoUseCase>         ordenoUC;
    std::shared_ptr<AddPartoUseCase>             addPartoUC;
    std::shared_ptr<UpdatePartoUseCase>          updatePartoUC;
    std::shared_ptr<DeletePartoUseCase>          deletePartoUC;
    std::shared_ptr<AddRegistroLecheUseCase>     addLecheUC;
    std::shared_ptr<UpdateRegistroLecheUseCase>  updateLecheUC;
    std::shared_ptr<DeleteRegistroLecheUseCase>  deleteLecheUC;
    std::shared_ptr<AddRegistroCarneUseCase>     addCarneUC;
    std::shared_ptr<UpdateRegistroCarneUseCase>  updateCarneUC;
    std::shared_ptr<DeleteRegistroCarneUseCase>  deleteCarneUC;
    std::filesystem::path                        testDir;
    std::string                                  idGanado;
};

// ─── Get ─────────────────────────────────────────────────────────────────────

TEST_F(ProduccionUseCasesTest, GetRetornaProduccionInicial) {
    auto result = getUC->execute(idGanado);
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result->prenez);
    EXPECT_FALSE(result->ordeno);
    EXPECT_TRUE(result->partos.empty());
    EXPECT_TRUE(result->registroLeche.empty());
    EXPECT_TRUE(result->registroCarne.empty());
}

TEST_F(ProduccionUseCasesTest, GetInexistente) {
    EXPECT_FALSE(getUC->execute("no-existe").has_value());
}

// ─── Prenez / Ordeno ─────────────────────────────────────────────────────────

TEST_F(ProduccionUseCasesTest, UpdatePrenez) {
    ASSERT_TRUE(prenezUC->execute(idGanado, true));
    EXPECT_TRUE(getUC->execute(idGanado)->prenez);
    ASSERT_TRUE(prenezUC->execute(idGanado, false));
    EXPECT_FALSE(getUC->execute(idGanado)->prenez);
}

TEST_F(ProduccionUseCasesTest, UpdateOrdeno) {
    ASSERT_TRUE(ordenoUC->execute(idGanado, true));
    EXPECT_TRUE(getUC->execute(idGanado)->ordeno);
}

// ─── Partos ──────────────────────────────────────────────────────────────────

TEST_F(ProduccionUseCasesTest, AddPartoActualizaFechaEnSQLite) {
    std::string partoId = UuidGenerator::generate();
    ASSERT_TRUE(addPartoUC->execute(idGanado, {partoId, "2024-03-10"}));

    auto prod = getUC->execute(idGanado);
    ASSERT_EQ(prod->partos.size(), 1u);
    EXPECT_EQ(prod->partos[0].fecha, "2024-03-10");

    // Verificar que fecha_ultimo_parto se actualizó en SQLite
    auto ganado = ganadoRepo->getById(idGanado);
    ASSERT_TRUE(ganado.has_value());
    EXPECT_EQ(ganado->fechaUltimoParto, "2024-03-10");
}

TEST_F(ProduccionUseCasesTest, UpdateParto) {
    std::string partoId = UuidGenerator::generate();
    addPartoUC->execute(idGanado, {partoId, "2024-03-10"});
    ASSERT_TRUE(updatePartoUC->execute(idGanado, {partoId, "2024-04-15"}));
    EXPECT_EQ(getUC->execute(idGanado)->partos[0].fecha, "2024-04-15");
}

TEST_F(ProduccionUseCasesTest, DeleteParto) {
    std::string p1 = UuidGenerator::generate();
    std::string p2 = UuidGenerator::generate();
    addPartoUC->execute(idGanado, {p1, "2024-03-10"});
    addPartoUC->execute(idGanado, {p2, "2025-01-18"});
    ASSERT_TRUE(deletePartoUC->execute(idGanado, p1));
    auto prod = getUC->execute(idGanado);
    ASSERT_EQ(prod->partos.size(), 1u);
    EXPECT_EQ(prod->partos[0].id, p2);
}

// ─── Registro leche ──────────────────────────────────────────────────────────

TEST_F(ProduccionUseCasesTest, AddRegistroLeche) {
    ASSERT_TRUE(addLecheUC->execute(idGanado, {"2024-03-11", 18.5}));
    auto prod = getUC->execute(idGanado);
    ASSERT_EQ(prod->registroLeche.size(), 1u);
    EXPECT_DOUBLE_EQ(prod->registroLeche[0].valor, 18.5);
}

TEST_F(ProduccionUseCasesTest, UpdateRegistroLeche) {
    addLecheUC->execute(idGanado, {"2024-03-11", 18.5});
    ASSERT_TRUE(updateLecheUC->execute(idGanado, "2024-03-11", "2024-03-11", 22.0));
    EXPECT_DOUBLE_EQ(getUC->execute(idGanado)->registroLeche[0].valor, 22.0);
}

TEST_F(ProduccionUseCasesTest, DeleteRegistroLeche) {
    addLecheUC->execute(idGanado, {"2024-03-11", 18.5});
    addLecheUC->execute(idGanado, {"2024-03-12", 19.2});
    ASSERT_TRUE(deleteLecheUC->execute(idGanado, "2024-03-11"));
    auto prod = getUC->execute(idGanado);
    ASSERT_EQ(prod->registroLeche.size(), 1u);
    EXPECT_EQ(prod->registroLeche[0].fecha, "2024-03-12");
}

// ─── Registro carne ──────────────────────────────────────────────────────────

TEST_F(ProduccionUseCasesTest, AddRegistroCarne) {
    ASSERT_TRUE(addCarneUC->execute(idGanado, {"2025-02-01", 320.5}));
    auto prod = getUC->execute(idGanado);
    ASSERT_EQ(prod->registroCarne.size(), 1u);
    EXPECT_DOUBLE_EQ(prod->registroCarne[0].valor, 320.5);
}

TEST_F(ProduccionUseCasesTest, UpdateRegistroCarne) {
    addCarneUC->execute(idGanado, {"2025-02-01", 320.5});
    ASSERT_TRUE(updateCarneUC->execute(idGanado, "2025-02-01", 350.0));
    EXPECT_DOUBLE_EQ(getUC->execute(idGanado)->registroCarne[0].valor, 350.0);
}

TEST_F(ProduccionUseCasesTest, DeleteRegistroCarne) {
    addCarneUC->execute(idGanado, {"2025-02-01", 320.5});
    addCarneUC->execute(idGanado, {"2025-03-01", 340.0});
    ASSERT_TRUE(deleteCarneUC->execute(idGanado, "2025-02-01"));
    auto prod = getUC->execute(idGanado);
    ASSERT_EQ(prod->registroCarne.size(), 1u);
    EXPECT_EQ(prod->registroCarne[0].fecha, "2025-03-01");
}