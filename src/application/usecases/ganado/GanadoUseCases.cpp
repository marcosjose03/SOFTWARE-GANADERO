#include "GanadoUseCases.h"
#include "infrastructure/utils/UuidGenerator.h"
#include "domain/entities/GanadoEnums.h"

namespace Application {

static std::string validarFechasRelativas(
    const std::string& nacimiento,
    const std::optional<std::string>& fechaDestete,
    const std::optional<std::string>& fechaUltimoParto,
    const std::optional<std::string>& fechaUltimaPalpacion,
    const std::optional<std::string>& fechaInseminacion) {

    if (nacimiento.empty()) return "";

    if (fechaDestete.has_value() && !fechaDestete->empty()
            && *fechaDestete < nacimiento)
        return "La fecha de destete no puede ser anterior a la fecha de nacimiento";

    if (fechaUltimoParto.has_value() && !fechaUltimoParto->empty()
            && *fechaUltimoParto < nacimiento)
        return "La fecha de último parto no puede ser anterior a la fecha de nacimiento";

    if (fechaUltimaPalpacion.has_value() && !fechaUltimaPalpacion->empty()
            && *fechaUltimaPalpacion < nacimiento)
        return "La fecha de última palpación no puede ser anterior a la fecha de nacimiento";

    if (fechaInseminacion.has_value() && !fechaInseminacion->empty()
            && *fechaInseminacion < nacimiento)
        return "La fecha de inseminación no puede ser anterior a la fecha de nacimiento";

    return "";
}

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
        .foto                 = g.foto,
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
    // Validar fechas relativas
    if (dto.raza.has_value() &&
        !Domain::razaEsValidaParaEspecie(*dto.raza, dto.especie))
        return std::nullopt;

    auto errorFechas = validarFechasRelativas(
        dto.nacimiento,
        dto.fechaDestete,
        dto.fechaUltimoParto,
        dto.fechaUltimaPalpacion,
        dto.fechaInseminacion);
    if (!errorFechas.empty())
        throw std::invalid_argument(errorFechas);

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
        .fechaUltimoParto     = dto.fechaUltimoParto,
        .fechaUltimaPalpacion = dto.fechaUltimaPalpacion,
        .fechaInseminacion    = dto.fechaInseminacion
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
    auto errorFechas = validarFechasRelativas(
        dto.nacimiento,
        dto.fechaDestete,
        dto.fechaUltimoParto,
        dto.fechaUltimaPalpacion,
        dto.fechaInseminacion);
    if (!errorFechas.empty())
        throw std::invalid_argument(errorFechas);

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

// ─── GetGanadoByFincaUseCase ─────────────────────────────────────────────────

GetGanadoByFincaUseCase::GetGanadoByFincaUseCase(
    std::shared_ptr<Domain::IGanadoRepository> repo)
    : m_repo(std::move(repo)) {}

std::vector<GanadoResultDto>
GetGanadoByFincaUseCase::execute(const std::string& idFinca) {
    std::vector<GanadoResultDto> result;
    for (const auto& g : m_repo->getByFinca(idFinca))
        result.push_back(ganadoToDto(g));
    return result;
}

// ─── ValidarProgenitoresUseCase ──────────────────────────────────────────────

ValidarProgenitoresUseCase::ValidarProgenitoresUseCase(
    std::shared_ptr<Domain::IGanadoRepository> repo)
    : m_repo(std::move(repo)) {}

std::string ValidarProgenitoresUseCase::execute(
    const std::string& fechaNacimiento,
    const std::optional<std::string>& idPadre,
    const std::optional<std::string>& idMadre) {

    if (fechaNacimiento.empty()) return "";

    if (idPadre.has_value() && !idPadre->empty()) {
        auto padre = m_repo->getById(*idPadre);
        if (padre.has_value() && !padre->nacimiento.empty()) {
            if (fechaNacimiento < padre->nacimiento)
                return "La fecha de nacimiento del animal no puede ser "
                       "anterior a la del padre (" + padre->nacimiento + ")";
        }
    }

    if (idMadre.has_value() && !idMadre->empty()) {
        auto madre = m_repo->getById(*idMadre);
        if (madre.has_value() && !madre->nacimiento.empty()) {
            if (fechaNacimiento < madre->nacimiento)
                return "La fecha de nacimiento del animal no puede ser "
                       "anterior a la de la madre (" + madre->nacimiento + ")";
        }
    }

    return "";
}

// ─── ActualizarFechaPartaMadreUseCase ────────────────────────────────────────

ActualizarFechaPartaMadreUseCase::ActualizarFechaPartaMadreUseCase(
    std::shared_ptr<Domain::IGanadoRepository>     ganadoRepo,
    std::shared_ptr<Domain::IProduccionRepository> produccionRepo)
    : m_ganadoRepo(std::move(ganadoRepo))
    , m_produccionRepo(std::move(produccionRepo)) {}

void ActualizarFechaPartaMadreUseCase::execute(
    const std::string& fechaNacimientoAnimal,
    const std::optional<std::string>& idMadre) {

    if (fechaNacimientoAnimal.empty()) return;
    if (!idMadre.has_value() || idMadre->empty()) return;

    auto madre = m_ganadoRepo->getById(*idMadre);
    if (!madre.has_value()) return;

    bool debeActualizar = false;

    if (!madre->fechaUltimoParto.has_value() ||
        madre->fechaUltimoParto->empty()) {
        // Campo vacío → actualizar
        debeActualizar = true;
    } else if (fechaNacimientoAnimal > *madre->fechaUltimoParto) {
        // Nacimiento más reciente que último parto → actualizar
        debeActualizar = true;
    }
    // Si son iguales o último parto es más reciente → no hacer nada

    if (!debeActualizar) return;

    // Actualizar en SQLite
    madre->fechaUltimoParto = fechaNacimientoAnimal;
    m_ganadoRepo->update(*madre);

    // Actualizar en LMDB — agregar parto
    auto produccion = m_produccionRepo->getById(*idMadre);
    if (produccion.has_value()) {
        Domain::Parto parto{
            Infrastructure::UuidGenerator::generate(),
            fechaNacimientoAnimal
        };
        m_produccionRepo->addParto(*idMadre, parto);
    }
}

} // namespace Application