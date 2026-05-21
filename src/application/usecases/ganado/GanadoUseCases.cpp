#include "GanadoUseCases.h"
#include "infrastructure/utils/UuidGenerator.h"
#include "domain/entities/GanadoEnums.h"

namespace Application {

static GanadoResultDto ganadoToDto(const Domain::Ganado& g) {
    return GanadoResultDto{
        .id                   = g.id,
        .especie              = g.especie,
        .identificador        = g.identificador,
        .idUsuario            = g.idUsuario,
        .idFinca              = g.idFinca,
        .nacimiento           = g.nacimiento,
        .sexo                 = g.sexo,
        .estado               = g.estado,
        .raza                 = g.raza,
        .idPadre              = g.idPadre,
        .idMadre              = g.idMadre,
        .chapeta              = g.chapeta,
        .fechaDestete         = g.fechaDestete,
        .fechaUltimoParto     = g.fechaUltimoParto,
        .fechaUltimaPalpacion = g.fechaUltimaPalpacion,
        .fechaInseminacion    = g.fechaInseminacion
    };
}

// ─── CreateGanadoUseCase ─────────────────────────────────────────────────────

CreateGanadoUseCase::CreateGanadoUseCase(
    std::shared_ptr<Domain::IGanadoRepository>     ganadoRepo,
    std::shared_ptr<Domain::IProduccionRepository> produccionRepo)
    : m_ganadoRepo(std::move(ganadoRepo))
    , m_produccionRepo(std::move(produccionRepo)) {}

std::optional<GanadoResultDto>
CreateGanadoUseCase::execute(const CreateGanadoDto& dto) {
    // Validar raza contra especie
    if (dto.raza.has_value() &&
        !Domain::razaEsValidaParaEspecie(*dto.raza, dto.especie))
        return std::nullopt;

    const std::string id = Infrastructure::UuidGenerator::generate();

    Domain::Ganado g{
        .id                   = id,
        .especie              = dto.especie,
        .identificador        = dto.identificador,
        .idUsuario            = dto.idUsuario,
        .idFinca              = dto.idFinca,
        .nacimiento           = dto.nacimiento,
        .sexo                 = dto.sexo,
        .estado               = dto.estado,
        .raza                 = dto.raza,
        .idPadre              = dto.idPadre,
        .idMadre              = dto.idMadre,
        .chapeta              = dto.chapeta,
        .fechaDestete         = dto.fechaDestete,
        .foto                 = dto.foto,
        .fechaUltimoParto     = std::nullopt,
        .fechaUltimaPalpacion = std::nullopt,
        .fechaInseminacion    = std::nullopt
    };

    if (!m_ganadoRepo->insert(g)) return std::nullopt;

    // Crear producción vacía con el mismo id
    Domain::Produccion p{
        .id            = id,
        .prenez        = false,
        .ordeno        = false,
        .partos        = {},
        .registroLeche = {},
        .registroCarne = {}
    };
    m_produccionRepo->insert(p);

    return ganadoToDto(g);
}

// ─── GetAllGanadoUseCase ─────────────────────────────────────────────────────

GetAllGanadoUseCase::GetAllGanadoUseCase(
    std::shared_ptr<Domain::IGanadoRepository> repo)
    : m_repo(std::move(repo)) {}

std::vector<GanadoResultDto> GetAllGanadoUseCase::execute() {
    std::vector<GanadoResultDto> result;
    for (const auto& g : m_repo->getAll())
        result.push_back(ganadoToDto(g));
    return result;
}

// ─── GetGanadoByIdUseCase ────────────────────────────────────────────────────

GetGanadoByIdUseCase::GetGanadoByIdUseCase(
    std::shared_ptr<Domain::IGanadoRepository> repo)
    : m_repo(std::move(repo)) {}

std::optional<GanadoResultDto>
GetGanadoByIdUseCase::execute(const std::string& id) {
    auto g = m_repo->getById(id);
    if (!g) return std::nullopt;
    return ganadoToDto(*g);
}

// ─── UpdateGanadoUseCase ─────────────────────────────────────────────────────

UpdateGanadoUseCase::UpdateGanadoUseCase(
    std::shared_ptr<Domain::IGanadoRepository> repo)
    : m_repo(std::move(repo)) {}

bool UpdateGanadoUseCase::execute(const UpdateGanadoDto& dto) {
    if (dto.raza.has_value() &&
        !Domain::razaEsValidaParaEspecie(*dto.raza, dto.especie))
        return false;

    auto existing = m_repo->getById(dto.id);
    if (!existing) return false;

    existing->especie              = dto.especie;
    existing->identificador        = dto.identificador;
    existing->idFinca              = dto.idFinca;
    existing->nacimiento           = dto.nacimiento;
    existing->sexo                 = dto.sexo;
    existing->estado               = dto.estado;
    existing->raza                 = dto.raza;
    existing->idPadre              = dto.idPadre;
    existing->idMadre              = dto.idMadre;
    existing->chapeta              = dto.chapeta;
    existing->fechaDestete         = dto.fechaDestete;
    existing->fechaUltimoParto     = dto.fechaUltimoParto;
    existing->fechaUltimaPalpacion = dto.fechaUltimaPalpacion;
    existing->fechaInseminacion    = dto.fechaInseminacion;
    existing->foto                 = dto.foto;

    return m_repo->update(*existing);
}

// ─── DeleteGanadoUseCase ─────────────────────────────────────────────────────

DeleteGanadoUseCase::DeleteGanadoUseCase(
    std::shared_ptr<Domain::IGanadoRepository>     ganadoRepo,
    std::shared_ptr<Domain::IProduccionRepository> produccionRepo)
    : m_ganadoRepo(std::move(ganadoRepo))
    , m_produccionRepo(std::move(produccionRepo)) {}

bool DeleteGanadoUseCase::execute(const std::string& id) {
    m_produccionRepo->deleteById(id);
    return m_ganadoRepo->deleteById(id);
}

} // namespace Application