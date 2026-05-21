#pragma once
#include <string>

namespace Application {

struct CreateFincaDto {
    std::string nombre;
    int         nHectareas;
    int         nPotreros;
    int         capacidad;
    std::string idUsuario;
};

struct UpdateFincaDto {
    std::string id;
    std::string nombre;
    int         nHectareas;
    int         nPotreros;
    int         capacidad;
};

struct FincaResultDto {
    std::string id;
    std::string nombre;
    int         nHectareas;
    int         nPotreros;
    int         capacidad;
    std::string idUsuario;
};

} // namespace Application