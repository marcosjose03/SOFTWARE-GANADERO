#pragma once
#include <string>

namespace Domain {

struct Finca {
    std::string id;
    std::string nombre;
    int         nHectareas;
    int         nPotreros;
    int         capacidad;
    std::string idUsuario;
};

} // namespace Domain