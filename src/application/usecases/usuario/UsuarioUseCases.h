#pragma once
#include "IUsuarioUseCases.h"
#include "domain/repositories/IUsuarioRepository.h"
#include <memory>

namespace Application {

class CreateUsuarioUseCase : public ICreateUsuarioUseCase {
public:
    explicit CreateUsuarioUseCase(std::shared_ptr<Domain::IUsuarioRepository> repo);
    [[nodiscard]] std::optional<UsuarioResultDto>
        execute(const CreateUsuarioDto& dto) override;
private:
    std::shared_ptr<Domain::IUsuarioRepository> m_repo;
    [[nodiscard]] UsuarioResultDto toDto(const Domain::Usuario& u) const;
};

class LoginUseCase : public ILoginUseCase {
public:
    explicit LoginUseCase(std::shared_ptr<Domain::IUsuarioRepository> repo);
    [[nodiscard]] std::optional<UsuarioResultDto>
        execute(const LoginDto& dto) override;
private:
    std::shared_ptr<Domain::IUsuarioRepository> m_repo;
    [[nodiscard]] UsuarioResultDto toDto(const Domain::Usuario& u) const;
};

class GetAllUsuariosUseCase : public IGetAllUsuariosUseCase {
public:
    explicit GetAllUsuariosUseCase(std::shared_ptr<Domain::IUsuarioRepository> repo);
    [[nodiscard]] std::vector<UsuarioResultDto> execute() override;
private:
    std::shared_ptr<Domain::IUsuarioRepository> m_repo;
    [[nodiscard]] UsuarioResultDto toDto(const Domain::Usuario& u) const;
};

class GetUsuarioByIdUseCase : public IGetUsuarioByIdUseCase {
public:
    explicit GetUsuarioByIdUseCase(std::shared_ptr<Domain::IUsuarioRepository> repo);
    [[nodiscard]] std::optional<UsuarioResultDto>
        execute(const std::string& id) override;
private:
    std::shared_ptr<Domain::IUsuarioRepository> m_repo;
    [[nodiscard]] UsuarioResultDto toDto(const Domain::Usuario& u) const;
};

class UpdateUsuarioUseCase : public IUpdateUsuarioUseCase {
public:
    explicit UpdateUsuarioUseCase(std::shared_ptr<Domain::IUsuarioRepository> repo);
    [[nodiscard]] bool execute(const UpdateUsuarioDto& dto) override;
private:
    std::shared_ptr<Domain::IUsuarioRepository> m_repo;
};

class DeleteUsuarioUseCase : public IDeleteUsuarioUseCase {
public:
    explicit DeleteUsuarioUseCase(std::shared_ptr<Domain::IUsuarioRepository> repo);
    [[nodiscard]] bool execute(const std::string& id) override;
private:
    std::shared_ptr<Domain::IUsuarioRepository> m_repo;
};

} // namespace Application