#include <gtest/gtest.h>
#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "application/usecases/usuario/UsuarioUseCases.h"

using namespace Infrastructure;
using namespace Application;

class UsuarioUseCasesTest : public ::testing::Test {
protected:
    void SetUp() override {
        db   = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();
        repo = std::make_shared<SqliteUsuarioRepository>(db);

        createUC  = std::make_shared<CreateUsuarioUseCase>(repo);
        loginUC   = std::make_shared<LoginUseCase>(repo);
        getAllUC  = std::make_shared<GetAllUsuariosUseCase>(repo);
        getByIdUC = std::make_shared<GetUsuarioByIdUseCase>(repo);
        updateUC  = std::make_shared<UpdateUsuarioUseCase>(repo);
        deleteUC  = std::make_shared<DeleteUsuarioUseCase>(repo);
    }

    std::shared_ptr<SQLiteDatabase>           db;
    std::shared_ptr<SqliteUsuarioRepository>  repo;
    std::shared_ptr<CreateUsuarioUseCase>     createUC;
    std::shared_ptr<LoginUseCase>             loginUC;
    std::shared_ptr<GetAllUsuariosUseCase>    getAllUC;
    std::shared_ptr<GetUsuarioByIdUseCase>    getByIdUC;
    std::shared_ptr<UpdateUsuarioUseCase>     updateUC;
    std::shared_ptr<DeleteUsuarioUseCase>     deleteUC;
};

// ─── Create ──────────────────────────────────────────────────────────────────

TEST_F(UsuarioUseCasesTest, CreateRetornaDto) {
    auto result = createUC->execute({"Juan", "juan@test.com", "pass123"});
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->nombre, "Juan");
    EXPECT_EQ(result->email,  "juan@test.com");
    EXPECT_FALSE(result->id.empty());
    EXPECT_TRUE(result->fechaRegistro.has_value());
}

TEST_F(UsuarioUseCasesTest, CreateGeneraUUIDUnico) {
    auto r1 = createUC->execute({"Juan", "juan@test.com", "pass123"});
    auto r2 = createUC->execute({"Ana",  "ana@test.com",  "pass456"});
    ASSERT_TRUE(r1.has_value());
    ASSERT_TRUE(r2.has_value());
    EXPECT_NE(r1->id, r2->id);
}

TEST_F(UsuarioUseCasesTest, CreateFallaSiCamposVacios) {
    EXPECT_FALSE(createUC->execute({"", "juan@test.com", "pass"}).has_value());
    EXPECT_FALSE(createUC->execute({"Juan", "", "pass"}).has_value());
    EXPECT_FALSE(createUC->execute({"Juan", "juan@test.com", ""}).has_value());
}

// ─── Login ───────────────────────────────────────────────────────────────────

TEST_F(UsuarioUseCasesTest, LoginExitoso) {
    createUC->execute({"Juan", "juan@test.com", "pass123"});
    auto result = loginUC->execute({"juan@test.com", "pass123"});
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->email, "juan@test.com");
}

TEST_F(UsuarioUseCasesTest, LoginFallaContrasenaIncorrecta) {
    createUC->execute({"Juan", "juan@test.com", "pass123"});
    auto result = loginUC->execute({"juan@test.com", "wrongpass"});
    EXPECT_FALSE(result.has_value());
}

TEST_F(UsuarioUseCasesTest, LoginFallaEmailInexistente) {
    auto result = loginUC->execute({"noexiste@test.com", "pass123"});
    EXPECT_FALSE(result.has_value());
}

// ─── GetAll ──────────────────────────────────────────────────────────────────

TEST_F(UsuarioUseCasesTest, GetAllRetornaLista) {
    createUC->execute({"Juan", "juan@test.com", "pass1"});
    createUC->execute({"Ana",  "ana@test.com",  "pass2"});
    auto all = getAllUC->execute();
    EXPECT_EQ(all.size(), 2u);
}

TEST_F(UsuarioUseCasesTest, GetAllVacio) {
    EXPECT_TRUE(getAllUC->execute().empty());
}

// ─── GetById ─────────────────────────────────────────────────────────────────

TEST_F(UsuarioUseCasesTest, GetByIdExistente) {
    auto created = createUC->execute({"Juan", "juan@test.com", "pass123"});
    auto result  = getByIdUC->execute(created->id);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id,     created->id);
    EXPECT_EQ(result->nombre, "Juan");
}

TEST_F(UsuarioUseCasesTest, GetByIdInexistente) {
    EXPECT_FALSE(getByIdUC->execute("no-existe").has_value());
}

// ─── Update ──────────────────────────────────────────────────────────────────

TEST_F(UsuarioUseCasesTest, UpdateExitoso) {
    auto created = createUC->execute({"Juan", "juan@test.com", "pass123"});
    bool ok = updateUC->execute({created->id, "Juan Nuevo", "nuevo@test.com", "newpass"});
    ASSERT_TRUE(ok);
    auto result = getByIdUC->execute(created->id);
    EXPECT_EQ(result->nombre, "Juan Nuevo");
    EXPECT_EQ(result->email,  "nuevo@test.com");
}

TEST_F(UsuarioUseCasesTest, UpdateFallaIdInexistente) {
    EXPECT_FALSE(updateUC->execute({"no-existe", "Nombre", "email@test.com", "pass"}));
}

TEST_F(UsuarioUseCasesTest, UpdateFallaCamposVacios) {
    auto created = createUC->execute({"Juan", "juan@test.com", "pass123"});
    EXPECT_FALSE(updateUC->execute({created->id, "", "email@test.com", "pass"}));
    EXPECT_FALSE(updateUC->execute({created->id, "Juan", "", "pass"}));
    EXPECT_FALSE(updateUC->execute({created->id, "Juan", "email@test.com", ""}));
}

// ─── Delete ──────────────────────────────────────────────────────────────────

TEST_F(UsuarioUseCasesTest, DeleteExitoso) {
    auto created = createUC->execute({"Juan", "juan@test.com", "pass123"});
    ASSERT_TRUE(deleteUC->execute(created->id));
    EXPECT_FALSE(getByIdUC->execute(created->id).has_value());
}

TEST_F(UsuarioUseCasesTest, DeleteInexistente) {
    deleteUC->execute("no-existe");
    SUCCEED();
}

// ─── CheckEmailExists ────────────────────────────────────────────────────────

class CheckEmailExistsUseCaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        db   = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();
        repo = std::make_shared<SqliteUsuarioRepository>(db);
        createUC = std::make_shared<CreateUsuarioUseCase>(repo);
        checkUC  = std::make_shared<CheckEmailExistsUseCase>(repo);
    }

    std::shared_ptr<SQLiteDatabase>           db;
    std::shared_ptr<SqliteUsuarioRepository>  repo;
    std::shared_ptr<CreateUsuarioUseCase>     createUC;
    std::shared_ptr<CheckEmailExistsUseCase>  checkUC;
};

TEST_F(CheckEmailExistsUseCaseTest, EmailExistente) {
    createUC->execute({"Juan", "juan@test.com", "pass123"});
    EXPECT_TRUE(checkUC->execute("juan@test.com"));
}

TEST_F(CheckEmailExistsUseCaseTest, EmailInexistente) {
    EXPECT_FALSE(checkUC->execute("noexiste@test.com"));
}

TEST_F(CheckEmailExistsUseCaseTest, EmailExistenteTrasDosUsuarios) {
    createUC->execute({"Juan", "juan@test.com",  "pass1"});
    createUC->execute({"Ana",  "ana@test.com",   "pass2"});
    EXPECT_TRUE(checkUC->execute("ana@test.com"));
    EXPECT_FALSE(checkUC->execute("otro@test.com"));
}