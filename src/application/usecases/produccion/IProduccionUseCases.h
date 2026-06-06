#pragma once
#include "application/dto/ProduccionDto.h"
#include <optional>
#include <string>

namespace Application {

class IGetProduccionUseCase {
public:
    virtual ~IGetProduccionUseCase() = default;
    [[nodiscard]] virtual std::optional<ProduccionResultDto>
        execute(const std::string& id) = 0;
};

class IUpdatePrenezUseCase {
public:
    virtual ~IUpdatePrenezUseCase() = default;
    [[nodiscard]] virtual bool execute(const std::string& id, bool prenez) = 0;
};

class IUpdateOrdenoUseCase {
public:
    virtual ~IUpdateOrdenoUseCase() = default;
    [[nodiscard]] virtual bool execute(const std::string& id, bool ordeno) = 0;
};

class IAddPartoUseCase {
public:
    virtual ~IAddPartoUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id, const PartoDto& parto) = 0;
};

class IUpdatePartoUseCase {
public:
    virtual ~IUpdatePartoUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id, const PartoDto& parto) = 0;
};

class IDeletePartoUseCase {
public:
    virtual ~IDeletePartoUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id, const std::string& partoId) = 0;
};

class IAddRegistroLecheUseCase {
public:
    virtual ~IAddRegistroLecheUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id, const RegistroFechaDto& r) = 0;
};

class IUpdateRegistroLecheUseCase {
public:
    virtual ~IUpdateRegistroLecheUseCase() = default;
    [[nodiscard]] virtual bool execute(
        const std::string& id,
        const std::string& fechaOriginal,
        const std::string& fechaNueva,
        double nuevoValor) = 0;
};

class IDeleteRegistroLecheUseCase {
public:
    virtual ~IDeleteRegistroLecheUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id, const std::string& fecha) = 0;
};

class IAddRegistroCarneUseCase {
public:
    virtual ~IAddRegistroCarneUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id, const RegistroFechaDto& r) = 0;
};

class IUpdateRegistroCarneUseCase {
public:
    virtual ~IUpdateRegistroCarneUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id,
                const std::string& fecha,
                double nuevoValor) = 0;
};

class IDeleteRegistroCarneUseCase {
public:
    virtual ~IDeleteRegistroCarneUseCase() = default;
    [[nodiscard]] virtual bool
        execute(const std::string& id, const std::string& fecha) = 0;
};

} // namespace Application