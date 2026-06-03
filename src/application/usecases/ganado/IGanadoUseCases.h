#pragma once
#include "application/dto/GanadoDto.h"
#include <vector>
#include <optional>
#include <string>

namespace Application {

class ICreateGanadoUseCase {
public:
    virtual ~ICreateGanadoUseCase() = default;
    [[nodiscard]] virtual std::optional<GanadoResultDto>
        execute(const CreateGanadoDto& dto) = 0;
};

class IGetAllGanadoUseCase {
public:
    virtual ~IGetAllGanadoUseCase() = default;
    [[nodiscard]] virtual std::vector<GanadoResultDto> execute() = 0;
};

class IGetGanadoByIdUseCase {
public:
    virtual ~IGetGanadoByIdUseCase() = default;
    [[nodiscard]] virtual std::optional<GanadoResultDto>
        execute(const std::string& id) = 0;
};

class IUpdateGanadoUseCase {
public:
    virtual ~IUpdateGanadoUseCase() = default;
    [[nodiscard]] virtual bool execute(const UpdateGanadoDto& dto) = 0;
};

class IDeleteGanadoUseCase {
public:
    virtual ~IDeleteGanadoUseCase() = default;
    [[nodiscard]] virtual bool execute(const std::string& id) = 0;
};

class IGetGanadoByFincaUseCase {
public:
    virtual ~IGetGanadoByFincaUseCase() = default;
    [[nodiscard]] virtual std::vector<GanadoResultDto>
        execute(const std::string& idFinca) = 0;
};

class IValidarProgenitoresUseCase {
public:
    virtual ~IValidarProgenitoresUseCase() = default;
    // Retorna string vacío si es válido, o el mensaje de error
    [[nodiscard]] virtual std::string execute(
        const std::string& fechaNacimiento,
        const std::optional<std::string>& idPadre,
        const std::optional<std::string>& idMadre) = 0;
};

class IActualizarFechaPartaMadreUseCase {
public:
    virtual ~IActualizarFechaPartaMadreUseCase() = default;
    virtual void execute(
        const std::string& fechaNacimientoAnimal,
        const std::optional<std::string>& idMadre) = 0;
};

} // namespace Application