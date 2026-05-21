#pragma once
#include <string>
#include <optional>

namespace Application {

struct CreateUsuarioDto {
    std::string nombre;
    std::string email;
    std::string contrasena;
};

struct UpdateUsuarioDto {
    std::string              id;
    std::string              nombre;
    std::string              email;
    std::string              contrasena;
};

struct LoginDto {
    std::string email;
    std::string contrasena;
};

struct UsuarioResultDto {
    std::string              id;
    std::string              nombre;
    std::string              email;
    std::optional<std::string> fechaRegistro;
    std::optional<std::string> fechaUltimoBackup;
};

} // namespace Application