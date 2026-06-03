#pragma once
#include "application/dto/UsuarioDto.h"
#include <vector>
#include <optional>
#include <string>

namespace Application {

class ICreateUsuarioUseCase {
public:
    virtual ~ICreateUsuarioUseCase() = default;
    [[nodiscard]] virtual std::optional<UsuarioResultDto>
        execute(const CreateUsuarioDto& dto) = 0;
};

class ILoginUseCase {
public:
    virtual ~ILoginUseCase() = default;
    [[nodiscard]] virtual std::optional<UsuarioResultDto>
        execute(const LoginDto& dto) = 0;
};

class IGetAllUsuariosUseCase {
public:
    virtual ~IGetAllUsuariosUseCase() = default;
    [[nodiscard]] virtual std::vector<UsuarioResultDto> execute() = 0;
};

class IGetUsuarioByIdUseCase {
public:
    virtual ~IGetUsuarioByIdUseCase() = default;
    [[nodiscard]] virtual std::optional<UsuarioResultDto>
        execute(const std::string& id) = 0;
};

class IUpdateUsuarioUseCase {
public:
    virtual ~IUpdateUsuarioUseCase() = default;
    [[nodiscard]] virtual bool execute(const UpdateUsuarioDto& dto) = 0;
};

class IDeleteUsuarioUseCase {
public:
    virtual ~IDeleteUsuarioUseCase() = default;
    [[nodiscard]] virtual bool execute(const std::string& id) = 0;
};

class ICheckEmailExistsUseCase {
public:
    virtual ~ICheckEmailExistsUseCase() = default;
    [[nodiscard]] virtual bool execute(const std::string& email) = 0;
};

} // namespace Application