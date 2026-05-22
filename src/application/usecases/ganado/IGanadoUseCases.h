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

} // namespace Application