#pragma once
#include <string>
#include <vector>

namespace Application {

struct RegistroFechaDto {
    std::string fecha;
    double      valor;
};

struct PartoDto {
    std::string id;
    std::string fecha;
};

struct ProduccionResultDto {
    std::string                  id;
    bool                         prenez;
    bool                         ordeno;
    std::vector<PartoDto>        partos;
    std::vector<RegistroFechaDto> registroLeche;
    std::vector<RegistroFechaDto> registroCarne;
};

} // namespace Application