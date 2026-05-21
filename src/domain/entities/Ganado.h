#pragma once
#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include "GanadoEnums.h"

namespace Domain {

struct Ganado {
    std::string              id;
    Especie                  especie;
    int                      identificador;
    std::string              idUsuario;
    std::string              idFinca;
    std::string              nacimiento;
    SexoGanado               sexo;
    EstadoGanado             estado;
    std::optional<std::string>           raza;
    std::optional<std::string>           idPadre;
    std::optional<std::string>           idMadre;
    std::optional<std::string>           chapeta;
    std::optional<std::string>           fechaDestete;
    std::optional<std::vector<uint8_t>>  foto;
    std::optional<std::string>           fechaUltimoParto;
    std::optional<std::string>           fechaUltimaPalpacion;
    std::optional<std::string>           fechaInseminacion;
};

} // namespace Domain