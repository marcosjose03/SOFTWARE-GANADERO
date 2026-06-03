#include "ProduccionUseCases.h"

namespace Application {

static ProduccionResultDto produccionToDto(const Domain::Produccion& p) {
    ProduccionResultDto dto;
    dto.id     = p.id;
    dto.prenez = p.prenez;
    dto.ordeno = p.ordeno;

    for (const auto& parto : p.partos)
        dto.partos.push_back({parto.id, parto.fecha});

    for (const auto& r : p.registroLeche)
        dto.registroLeche.push_back({r.fecha, r.valor});

    for (const auto& r : p.registroCarne)
        dto.registroCarne.push_back({r.fecha, r.valor});

    return dto;
}

// ─── GetProduccionUseCase ────────────────────────────────────────────────────

GetProduccionUseCase::GetProduccionUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

std::optional<ProduccionResultDto>
GetProduccionUseCase::execute(const std::string& id) {
    auto p = m_repo->getById(id);
    if (!p) return std::nullopt;
    return produccionToDto(*p);
}

// ─── UpdatePrenezUseCase ─────────────────────────────────────────────────────

UpdatePrenezUseCase::UpdatePrenezUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdatePrenezUseCase::execute(const std::string& id, bool prenez) {
    return m_repo->updatePrenez(id, prenez);
}

// ─── UpdateOrdenoUseCase ─────────────────────────────────────────────────────

UpdateOrdenoUseCase::UpdateOrdenoUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdateOrdenoUseCase::execute(const std::string& id, bool ordeno) {
    return m_repo->updateOrdeno(id, ordeno);
}

// ─── AddPartoUseCase ─────────────────────────────────────────────────────────

AddPartoUseCase::AddPartoUseCase(
    std::shared_ptr<Domain::IProduccionRepository> produccionRepo,
    std::shared_ptr<Domain::IGanadoRepository>     ganadoRepo)
    : m_produccionRepo(std::move(produccionRepo))
    , m_ganadoRepo(std::move(ganadoRepo)) {}

bool AddPartoUseCase::execute(const std::string& id, const PartoDto& parto) {
    Domain::Parto p{parto.id, parto.fecha};

    if (!m_produccionRepo->addParto(id, p)) return false;

    // Actualizar fecha_ultimo_parto en SQLite
    auto ganado = m_ganadoRepo->getById(id);
    if (ganado) {
        ganado->fechaUltimoParto = parto.fecha;
        m_ganadoRepo->update(*ganado);
    }
    return true;
}

// ─── UpdatePartoUseCase ──────────────────────────────────────────────────────

UpdatePartoUseCase::UpdatePartoUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdatePartoUseCase::execute(const std::string& id, const PartoDto& parto) {
    return m_repo->updateParto(id, {parto.id, parto.fecha});
}

// ─── DeletePartoUseCase ──────────────────────────────────────────────────────

DeletePartoUseCase::DeletePartoUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool DeletePartoUseCase::execute(const std::string& id,
                                  const std::string& partoId) {
    return m_repo->deleteParto(id, partoId);
}

// ─── AddRegistroLecheUseCase ─────────────────────────────────────────────────

AddRegistroLecheUseCase::AddRegistroLecheUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool AddRegistroLecheUseCase::execute(const std::string& id,
                                       const RegistroFechaDto& r) {
    return m_repo->addRegistroLeche(id, {r.fecha, r.valor});
}

// ─── UpdateRegistroLecheUseCase ──────────────────────────────────────────────

UpdateRegistroLecheUseCase::UpdateRegistroLecheUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdateRegistroLecheUseCase::execute(const std::string& id,
                                          const std::string& fecha,
                                          double nuevoValor) {
    return m_repo->updateRegistroLeche(id, fecha, nuevoValor);
}

// ─── DeleteRegistroLecheUseCase ──────────────────────────────────────────────

DeleteRegistroLecheUseCase::DeleteRegistroLecheUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool DeleteRegistroLecheUseCase::execute(const std::string& id,
                                          const std::string& fecha) {
    return m_repo->deleteRegistroLeche(id, fecha);
}

// ─── AddRegistroCarneUseCase ─────────────────────────────────────────────────

AddRegistroCarneUseCase::AddRegistroCarneUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool AddRegistroCarneUseCase::execute(const std::string& id,
                                       const RegistroFechaDto& r) {
    return m_repo->addRegistroCarne(id, {r.fecha, r.valor});
}

// ─── UpdateRegistroCarneUseCase ──────────────────────────────────────────────

UpdateRegistroCarneUseCase::UpdateRegistroCarneUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdateRegistroCarneUseCase::execute(const std::string& id,
                                          const std::string& fecha,
                                          double nuevoValor) {
    return m_repo->updateRegistroCarne(id, fecha, nuevoValor);
}

// ─── DeleteRegistroCarneUseCase ──────────────────────────────────────────────

DeleteRegistroCarneUseCase::DeleteRegistroCarneUseCase(
    std::shared_ptr<Domain::IProduccionRepository> repo)
    : m_repo(std::move(repo)) {}

bool DeleteRegistroCarneUseCase::execute(const std::string& id,
                                          const std::string& fecha) {
    return m_repo->deleteRegistroCarne(id, fecha);
}

} // namespace Application