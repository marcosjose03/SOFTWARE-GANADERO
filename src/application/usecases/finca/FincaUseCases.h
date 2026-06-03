#pragma once
#include "IFincaUseCases.h"
#include "domain/repositories/IFincaRepository.h"
#include <memory>

namespace Application {

class CreateFincaUseCase : public ICreateFincaUseCase {
public:
    explicit CreateFincaUseCase(std::shared_ptr<Domain::IFincaRepository> repo);
    [[nodiscard]] std::optional<FincaResultDto>
        execute(const CreateFincaDto& dto) override;
private:
    std::shared_ptr<Domain::IFincaRepository> m_repo;
};

class GetAllFincasUseCase : public IGetAllFincasUseCase {
public:
    explicit GetAllFincasUseCase(std::shared_ptr<Domain::IFincaRepository> repo);
    [[nodiscard]] std::vector<FincaResultDto> execute() override;
private:
    std::shared_ptr<Domain::IFincaRepository> m_repo;
};

class GetFincaByIdUseCase : public IGetFincaByIdUseCase {
public:
    explicit GetFincaByIdUseCase(std::shared_ptr<Domain::IFincaRepository> repo);
    [[nodiscard]] std::optional<FincaResultDto>
        execute(const std::string& id) override;
private:
    std::shared_ptr<Domain::IFincaRepository> m_repo;
};

class UpdateFincaUseCase : public IUpdateFincaUseCase {
public:
    explicit UpdateFincaUseCase(std::shared_ptr<Domain::IFincaRepository> repo);
    [[nodiscard]] bool execute(const UpdateFincaDto& dto) override;
private:
    std::shared_ptr<Domain::IFincaRepository> m_repo;
};

class DeleteFincaUseCase : public IDeleteFincaUseCase {
public:
    explicit DeleteFincaUseCase(std::shared_ptr<Domain::IFincaRepository> repo);
    [[nodiscard]] bool execute(const std::string& id) override;
private:
    std::shared_ptr<Domain::IFincaRepository> m_repo;
};

} // namespace Application