#pragma once
#include "IProduccionUseCases.h"
#include "domain/repositories/IProduccionRepository.h"
#include "domain/repositories/IGanadoRepository.h"
#include <memory>

namespace Application {

class GetProduccionUseCase : public IGetProduccionUseCase {
public:
    explicit GetProduccionUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] std::optional<ProduccionResultDto>
        execute(const std::string& id) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class UpdatePrenezUseCase : public IUpdatePrenezUseCase {
public:
    explicit UpdatePrenezUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool execute(const std::string& id, bool prenez) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class UpdateOrdenoUseCase : public IUpdateOrdenoUseCase {
public:
    explicit UpdateOrdenoUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool execute(const std::string& id, bool ordeno) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class AddPartoUseCase : public IAddPartoUseCase {
public:
    AddPartoUseCase(
        std::shared_ptr<Domain::IProduccionRepository> produccionRepo,
        std::shared_ptr<Domain::IGanadoRepository>     ganadoRepo);
    [[nodiscard]] bool
        execute(const std::string& id, const PartoDto& parto) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_produccionRepo;
    std::shared_ptr<Domain::IGanadoRepository>     m_ganadoRepo;
};

class UpdatePartoUseCase : public IUpdatePartoUseCase {
public:
    explicit UpdatePartoUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool
        execute(const std::string& id, const PartoDto& parto) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class DeletePartoUseCase : public IDeletePartoUseCase {
public:
    explicit DeletePartoUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool
        execute(const std::string& id, const std::string& partoId) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class AddRegistroLecheUseCase : public IAddRegistroLecheUseCase {
public:
    explicit AddRegistroLecheUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool
        execute(const std::string& id, const RegistroFechaDto& r) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class UpdateRegistroLecheUseCase : public IUpdateRegistroLecheUseCase {
public:
    explicit UpdateRegistroLecheUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool execute(const std::string& id,
                               const std::string& fechaOriginal,
                                 const std::string& fechaNueva,
                               double nuevoValor) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class DeleteRegistroLecheUseCase : public IDeleteRegistroLecheUseCase {
public:
    explicit DeleteRegistroLecheUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool
        execute(const std::string& id, const std::string& fecha) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class AddRegistroCarneUseCase : public IAddRegistroCarneUseCase {
public:
    explicit AddRegistroCarneUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool
        execute(const std::string& id, const RegistroFechaDto& r) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class UpdateRegistroCarneUseCase : public IUpdateRegistroCarneUseCase {
public:
    explicit UpdateRegistroCarneUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool execute(const std::string& id,
                               const std::string& fechaOriginal,
                               const std::string& fechaNueva,
                               double nuevoValor) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

class DeleteRegistroCarneUseCase : public IDeleteRegistroCarneUseCase {
public:
    explicit DeleteRegistroCarneUseCase(
        std::shared_ptr<Domain::IProduccionRepository> repo);
    [[nodiscard]] bool
        execute(const std::string& id, const std::string& fecha) override;
private:
    std::shared_ptr<Domain::IProduccionRepository> m_repo;
};

} // namespace Application