#pragma once
#include <string>
#include <vector>

namespace Domain {

struct RegistroFecha {
    std::string fecha;
    double      valor;
};

struct Parto {
    std::string id;
    std::string fecha;
};

struct Produccion {
    std::string              id;  // mismo UUID que Ganado
    bool                     prenez;
    bool                     ordeno;
    std::vector<Parto>       partos;
    std::vector<RegistroFecha> registroLeche;
    std::vector<RegistroFecha> registroCarne;
};

} // namespace Domain