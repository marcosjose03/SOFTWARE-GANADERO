#pragma once
#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include "domain/entities/GanadoEnums.h"

namespace Application {

struct CreateGanadoDto {
    Domain::Especie              especie;
    std::string                  identificador;
    std::string                  idUsuario;
    std::string                  idFinca;
    std::string                  nacimiento;
    Domain::SexoGanado           sexo;
    Domain::EstadoGanado         estado;
    std::optional<std::string>   raza;
    std::optional<std::string>   idPadre;
    std::optional<std::string>   idMadre;
    std::optional<std::string>   chapeta;
    std::optional<std::string>   fechaDestete;
    std::optional<std::vector<uint8_t>> foto;
    std::optional<std::string>   fechaUltimoParto;
    std::optional<std::string>   fechaUltimaPalpacion;
    std::optional<std::string>   fechaInseminacion;
};

struct UpdateGanadoDto {
    std::string                  id;
    Domain::Especie              especie;
    std::string                  identificador;
    std::string                  idFinca;
    std::string                  nacimiento;
    Domain::SexoGanado           sexo;
    Domain::EstadoGanado         estado;
    std::optional<std::string>   raza;
    std::optional<std::string>   idPadre;
    std::optional<std::string>   idMadre;
    std::optional<std::string>   chapeta;
    std::optional<std::string>   fechaDestete;
    std::optional<std::string>   fechaUltimoParto;
    std::optional<std::string>   fechaUltimaPalpacion;
    std::optional<std::string>   fechaInseminacion;
    std::optional<std::vector<uint8_t>> foto;
};

struct GanadoResultDto {
    std::string                  id;
    Domain::Especie              especie;
    std::string                  identificador;
    std::string                  idUsuario;
    std::string                  idFinca;
    std::string                  nacimiento;
    Domain::SexoGanado           sexo;
    Domain::EstadoGanado         estado;
    std::optional<std::string>   raza;
    std::optional<std::string>   idPadre;
    std::optional<std::string>   idMadre;
    std::optional<std::string>   chapeta;
    std::optional<std::string>   fechaDestete;
    std::optional<std::string>   fechaUltimoParto;
    std::optional<std::string>   fechaUltimaPalpacion;
    std::optional<std::string>   fechaInseminacion;
};

} // namespace Application