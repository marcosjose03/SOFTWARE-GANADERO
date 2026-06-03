#include <gtest/gtest.h>
#include "infrastructure/database/SQLiteDatabase.h"
#include "infrastructure/repositories/SqliteUsuarioRepository.h"
#include "infrastructure/repositories/SqliteFincaRepository.h"
#include "application/usecases/usuario/UsuarioUseCases.h"
#include "application/usecases/finca/FincaUseCases.h"

using namespace Infrastructure;
using namespace Application;

class FincaUseCasesTest : public ::testing::Test {
protected:
    void SetUp() override {
        db = std::make_shared<SQLiteDatabase>(":memory:");
        db->initSchema();

        auto usuarioRepo = std::make_shared<SqliteUsuarioRepository>(db);
        createUsuarioUC  = std::make_shared<CreateUsuarioUseCase>(usuarioRepo);

        auto fincaRepo = std::make_shared<SqliteFincaRepository>(db);
        createUC  = std::make_shared<CreateFincaUseCase>(fincaRepo);
        getAllUC  = std::make_shared<GetAllFincasUseCase>(fincaRepo);
        getByIdUC = std::make_shared<GetFincaByIdUseCase>(fincaRepo);
        updateUC  = std::make_shared<UpdateFincaUseCase>(fincaRepo);
        deleteUC  = std::make_shared<DeleteFincaUseCase>(fincaRepo);

        auto u = createUsuarioUC->execute({"Dueño", "dueno@test.com", "pass"});
        idUsuario = u->id;
    }

    std::shared_ptr<SQLiteDatabase>         db;
    std::shared_ptr<CreateUsuarioUseCase>   createUsuarioUC;
    std::shared_ptr<CreateFincaUseCase>     createUC;
    std::shared_ptr<GetAllFincasUseCase>    getAllUC;
    std::shared_ptr<GetFincaByIdUseCase>    getByIdUC;
    std::shared_ptr<UpdateFincaUseCase>     updateUC;
    std::shared_ptr<DeleteFincaUseCase>     deleteUC;
    std::string                             idUsuario;

    CreateFincaDto makeDto() {
        return CreateFincaDto{
            .nombre     = "La Esperanza",
            .nHectareas = 150,
            .nPotreros  = 8,
            .capacidad  = 200,
            .idUsuario  = idUsuario
        };
    }
};

TEST_F(FincaUseCasesTest, CreateRetornaDto) {
    auto result = createUC->execute(makeDto());
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->nombre,     "La Esperanza");
    EXPECT_EQ(result->nHectareas, 150);
    EXPECT_EQ(result->idUsuario,  idUsuario);
    EXPECT_FALSE(result->id.empty());
}

TEST_F(FincaUseCasesTest, CreateGeneraUUIDUnico) {
    auto r1 = createUC->execute(makeDto());
    auto r2 = createUC->execute(makeDto());
    EXPECT_NE(r1->id, r2->id);
}

TEST_F(FincaUseCasesTest, CreateFallaValoresInvalidos) {
    auto dto = makeDto();
    dto.nombre = "";
    EXPECT_FALSE(createUC->execute(dto).has_value());

    dto = makeDto();
    dto.nHectareas = 0;
    EXPECT_FALSE(createUC->execute(dto).has_value());
}

TEST_F(FincaUseCasesTest, GetAll) {
    createUC->execute(makeDto());
    createUC->execute(makeDto());
    EXPECT_EQ(getAllUC->execute().size(), 2u);
}

TEST_F(FincaUseCasesTest, GetById) {
    auto created = createUC->execute(makeDto());
    auto result  = getByIdUC->execute(created->id);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->nombre, "La Esperanza");
}

TEST_F(FincaUseCasesTest, GetByIdInexistente) {
    EXPECT_FALSE(getByIdUC->execute("no-existe").has_value());
}

TEST_F(FincaUseCasesTest, Update) {
    auto created = createUC->execute(makeDto());
    bool ok = updateUC->execute({
        created->id, "Nueva Esperanza", 200, 10, 300
    });
    ASSERT_TRUE(ok);
    auto result = getByIdUC->execute(created->id);
    EXPECT_EQ(result->nombre,     "Nueva Esperanza");
    EXPECT_EQ(result->nHectareas, 200);
}

TEST_F(FincaUseCasesTest, UpdateFallaIdInexistente) {
    EXPECT_FALSE(updateUC->execute({"no-existe", "Nombre", 100, 5, 100}));
}

TEST_F(FincaUseCasesTest, Delete) {
    auto created = createUC->execute(makeDto());
    ASSERT_TRUE(deleteUC->execute(created->id));
    EXPECT_FALSE(getByIdUC->execute(created->id).has_value());
}