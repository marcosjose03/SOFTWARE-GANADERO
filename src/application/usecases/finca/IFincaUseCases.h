#pragma once
#include "application/dto/FincaDto.h"
#include <vector>
#include <optional>
#include <string>

namespace Application {

class ICreateFincaUseCase {
public:
    virtual ~ICreateFincaUseCase() = default;
    [[nodiscard]] virtual std::optional<FincaResultDto>
        execute(const CreateFincaDto& dto) = 0;
};

class IGetAllFincasUseCase {
public:
    virtual ~IGetAllFincasUseCase() = default;
    [[nodiscard]] virtual std::vector<FincaResultDto> execute() = 0;
};

class IGetFincaByIdUseCase {
public:
    virtual ~IGetFincaByIdUseCase() = default;
    [[nodiscard]] virtual std::optional<FincaResultDto>
        execute(const std::string& id) = 0;
};

class IUpdateFincaUseCase {
public:
    virtual ~IUpdateFincaUseCase() = default;
    [[nodiscard]] virtual bool execute(const UpdateFincaDto& dto) = 0;
};

class IDeleteFincaUseCase {
public:
    virtual ~IDeleteFincaUseCase() = default;
    [[nodiscard]] virtual bool execute(const std::string& id) = 0;
};

} // namespace Application