#pragma once
#include <string>
#include <optional>

namespace Domain {

struct Usuario {
    std::string              id;
    std::string              nombre;
    std::string              email;
    std::string              contrasena;
    std::optional<std::string> fechaRegistro;
    std::optional<std::string> fechaUltimoBackup;
};

} // namespace Domain