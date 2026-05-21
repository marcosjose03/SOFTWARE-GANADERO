#pragma once
#include "IGanadoUseCases.h"
#include "domain/repositories/IGanadoRepository.h"
#include "domain/repositories/IProduccionRepository.h"
#include <memory>

namespace Application {

class CreateGanadoUseCase : public ICreateGanadoUseCase {
public:
    CreateGanadoUseCase(
        std::shared_ptr<Domain::IGanadoRepository>     ganadoRepo,
        std::shared_ptr<Domain::IProduccionRepository> produccionRepo);
    [[nodiscard]] std::optional<GanadoResultDto>
        execute(const CreateGanadoDto& dto) override;
private:
    std::shared_ptr<Domain::IGanadoRepository>     m_ganadoRepo;
    std::shared_ptr<Domain::IProduccionRepository> m_produccionRepo;
};

class GetAllGanadoUseCase : public IGetAllGanadoUseCase {
public:
    explicit GetAllGanadoUseCase(std::shared_ptr<Domain::IGanadoRepository> repo);
    [[nodiscard]] std::vector<GanadoResultDto> execute() override;
private:
    std::shared_ptr<Domain::IGanadoRepository> m_repo;
};

class GetGanadoByIdUseCase : public IGetGanadoByIdUseCase {
public:
    explicit GetGanadoByIdUseCase(std::shared_ptr<Domain::IGanadoRepository> repo);
    [[nodiscard]] std::optional<GanadoResultDto>
        execute(const std::string& id) override;
private:
    std::shared_ptr<Domain::IGanadoRepository> m_repo;
};

class UpdateGanadoUseCase : public IUpdateGanadoUseCase {
public:
    explicit UpdateGanadoUseCase(std::shared_ptr<Domain::IGanadoRepository> repo);
    [[nodiscard]] bool execute(const UpdateGanadoDto& dto) override;
private:
    std::shared_ptr<Domain::IGanadoRepository> m_repo;
};

class DeleteGanadoUseCase : public IDeleteGanadoUseCase {
public:
    DeleteGanadoUseCase(
        std::shared_ptr<Domain::IGanadoRepository>     ganadoRepo,
        std::shared_ptr<Domain::IProduccionRepository> produccionRepo);
    [[nodiscard]] bool execute(const std::string& id) override;
private:
    std::shared_ptr<Domain::IGanadoRepository>     m_ganadoRepo;
    std::shared_ptr<Domain::IProduccionRepository> m_produccionRepo;
};

} // namespace Application