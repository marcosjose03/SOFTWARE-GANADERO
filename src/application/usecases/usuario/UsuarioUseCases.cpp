#include "UsuarioUseCases.h"
#include "infrastructure/utils/UuidGenerator.h"
#include "infrastructure/utils/DateTimeUtils.h"
#include <iostream>

namespace Application {

// ─── Helper compartido ───────────────────────────────────────────────────────

static UsuarioResultDto usuarioToDto(const Domain::Usuario& u) {
    return UsuarioResultDto{
        .id                = u.id,
        .nombre            = u.nombre,
        .email             = u.email,
        .fechaRegistro     = u.fechaRegistro,
        .fechaUltimoBackup = u.fechaUltimoBackup
    };
}

// ─── CreateUsuarioUseCase ────────────────────────────────────────────────────

CreateUsuarioUseCase::CreateUsuarioUseCase(
    std::shared_ptr<Domain::IUsuarioRepository> repo)
    : m_repo(std::move(repo)) {}

std::optional<UsuarioResultDto>
CreateUsuarioUseCase::execute(const CreateUsuarioDto& dto) {
    if (dto.nombre.empty() || dto.email.empty() || dto.contrasena.empty())
        return std::nullopt;

    Domain::Usuario u{
        .id                = Infrastructure::UuidGenerator::generate(),
        .nombre            = dto.nombre,
        .email             = dto.email,
        .contrasena        = dto.contrasena,
        .fechaRegistro     = Infrastructure::DateTimeUtils::currentDate(),
        .fechaUltimoBackup = std::nullopt
    };

    std::cerr << "Intentando insert id: " << u.id << std::endl;
    bool ok = m_repo->insert(u);
    std::cerr << "insert result: " << ok << std::endl;

    if (!ok) return std::nullopt;
    return usuarioToDto(u);
}

UsuarioResultDto CreateUsuarioUseCase::toDto(const Domain::Usuario& u) const {
    return usuarioToDto(u);
}

// ─── LoginUseCase ────────────────────────────────────────────────────────────

LoginUseCase::LoginUseCase(std::shared_ptr<Domain::IUsuarioRepository> repo)
    : m_repo(std::move(repo)) {}

std::optional<UsuarioResultDto>
LoginUseCase::execute(const LoginDto& dto) {
    auto all = m_repo->getAll();
    for (const auto& u : all) {
        if (u.email == dto.email && u.contrasena == dto.contrasena)
            return usuarioToDto(u);
    }
    return std::nullopt;
}

UsuarioResultDto LoginUseCase::toDto(const Domain::Usuario& u) const {
    return usuarioToDto(u);
}

// ─── GetAllUsuariosUseCase ───────────────────────────────────────────────────

GetAllUsuariosUseCase::GetAllUsuariosUseCase(
    std::shared_ptr<Domain::IUsuarioRepository> repo)
    : m_repo(std::move(repo)) {}

std::vector<UsuarioResultDto> GetAllUsuariosUseCase::execute() {
    std::vector<UsuarioResultDto> result;
    for (const auto& u : m_repo->getAll())
        result.push_back(usuarioToDto(u));
    return result;
}

UsuarioResultDto GetAllUsuariosUseCase::toDto(const Domain::Usuario& u) const {
    return usuarioToDto(u);
}

// ─── GetUsuarioByIdUseCase ───────────────────────────────────────────────────

GetUsuarioByIdUseCase::GetUsuarioByIdUseCase(
    std::shared_ptr<Domain::IUsuarioRepository> repo)
    : m_repo(std::move(repo)) {}

std::optional<UsuarioResultDto>
GetUsuarioByIdUseCase::execute(const std::string& id) {
    auto u = m_repo->getById(id);
    if (!u) return std::nullopt;
    return usuarioToDto(*u);
}

UsuarioResultDto GetUsuarioByIdUseCase::toDto(const Domain::Usuario& u) const {
    return usuarioToDto(u);
}

// ─── UpdateUsuarioUseCase ────────────────────────────────────────────────────

UpdateUsuarioUseCase::UpdateUsuarioUseCase(
    std::shared_ptr<Domain::IUsuarioRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdateUsuarioUseCase::execute(const UpdateUsuarioDto& dto) {
    if (dto.nombre.empty() || dto.email.empty() || dto.contrasena.empty())
        return false;

    auto existing = m_repo->getById(dto.id);
    if (!existing) return false;

    existing->nombre     = dto.nombre;
    existing->email      = dto.email;
    existing->contrasena = dto.contrasena;

    return m_repo->update(*existing);
}

// ─── DeleteUsuarioUseCase ────────────────────────────────────────────────────

DeleteUsuarioUseCase::DeleteUsuarioUseCase(
    std::shared_ptr<Domain::IUsuarioRepository> repo)
    : m_repo(std::move(repo)) {}

bool DeleteUsuarioUseCase::execute(const std::string& id) {
    return m_repo->deleteById(id);
}

// ─── CheckEmailExistsUseCase ─────────────────────────────────────────────────

CheckEmailExistsUseCase::CheckEmailExistsUseCase(
    std::shared_ptr<Domain::IUsuarioRepository> repo)
    : m_repo(std::move(repo)) {}

bool CheckEmailExistsUseCase::execute(const std::string& email) {
    return m_repo->getByEmail(email).has_value();
}

} // namespace Application