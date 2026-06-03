#include "FincaUseCases.h"
#include "infrastructure/utils/UuidGenerator.h"

namespace Application {

static FincaResultDto fincaToDto(const Domain::Finca& f) {
    return FincaResultDto{
        .id         = f.id,
        .nombre     = f.nombre,
        .nHectareas = f.nHectareas,
        .nPotreros  = f.nPotreros,
        .capacidad  = f.capacidad,
        .idUsuario  = f.idUsuario
    };
}

// ─── CreateFincaUseCase ──────────────────────────────────────────────────────

CreateFincaUseCase::CreateFincaUseCase(
    std::shared_ptr<Domain::IFincaRepository> repo)
    : m_repo(std::move(repo)) {}

std::optional<FincaResultDto>
CreateFincaUseCase::execute(const CreateFincaDto& dto) {
    if (dto.nombre.empty() || dto.idUsuario.empty())
        return std::nullopt;
    if (dto.nHectareas <= 0 || dto.nPotreros <= 0 || dto.capacidad <= 0)
        return std::nullopt;

    Domain::Finca f{
        .id         = Infrastructure::UuidGenerator::generate(),
        .nombre     = dto.nombre,
        .nHectareas = dto.nHectareas,
        .nPotreros  = dto.nPotreros,
        .capacidad  = dto.capacidad,
        .idUsuario  = dto.idUsuario
    };

    if (!m_repo->insert(f)) return std::nullopt;
    return fincaToDto(f);
}

// ─── GetAllFincasUseCase ─────────────────────────────────────────────────────

GetAllFincasUseCase::GetAllFincasUseCase(
    std::shared_ptr<Domain::IFincaRepository> repo)
    : m_repo(std::move(repo)) {}

std::vector<FincaResultDto> GetAllFincasUseCase::execute() {
    std::vector<FincaResultDto> result;
    for (const auto& f : m_repo->getAll())
        result.push_back(fincaToDto(f));
    return result;
}

// ─── GetFincaByIdUseCase ─────────────────────────────────────────────────────

GetFincaByIdUseCase::GetFincaByIdUseCase(
    std::shared_ptr<Domain::IFincaRepository> repo)
    : m_repo(std::move(repo)) {}

std::optional<FincaResultDto>
GetFincaByIdUseCase::execute(const std::string& id) {
    auto f = m_repo->getById(id);
    if (!f) return std::nullopt;
    return fincaToDto(*f);
}

// ─── UpdateFincaUseCase ──────────────────────────────────────────────────────

UpdateFincaUseCase::UpdateFincaUseCase(
    std::shared_ptr<Domain::IFincaRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdateFincaUseCase::execute(const UpdateFincaDto& dto) {
    if (dto.nombre.empty()) return false;
    if (dto.nHectareas <= 0 || dto.nPotreros <= 0 || dto.capacidad <= 0)
        return false;

    auto existing = m_repo->getById(dto.id);
    if (!existing) return false;

    existing->nombre     = dto.nombre;
    existing->nHectareas = dto.nHectareas;
    existing->nPotreros  = dto.nPotreros;
    existing->capacidad  = dto.capacidad;

    return m_repo->update(*existing);
}

// ─── DeleteFincaUseCase ──────────────────────────────────────────────────────

DeleteFincaUseCase::DeleteFincaUseCase(
    std::shared_ptr<Domain::IFincaRepository> repo)
    : m_repo(std::move(repo)) {}

bool DeleteFincaUseCase::execute(const std::string& id) {
    return m_repo->deleteById(id);
}

} // namespace Application