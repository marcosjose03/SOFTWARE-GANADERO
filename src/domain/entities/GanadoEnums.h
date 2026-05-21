#pragma once
#include <string>
#include <vector>
#include <stdexcept>

namespace Domain {

// ─── Enums ───────────────────────────────────────────────────────────────────

enum class Especie {
    Bovino,
    Caprino,
    Bufalino
};

enum class SexoGanado {
    Macho,
    Hembra
};

enum class EstadoGanado {
    Activo,
    Muerto,
    Vendido,
    Enfermo
};

// ─── Conversión string ↔ enum ─────────────────────────────────────────────────

inline std::string especieToString(Especie e) {
    switch (e) {
        case Especie::Bovino:   return "Bovino";
        case Especie::Caprino:  return "Caprino";
        case Especie::Bufalino: return "Bufalino";
    }
    throw std::invalid_argument("Especie desconocida");
}

inline Especie especieFromString(const std::string& s) {
    if (s == "Bovino")   return Especie::Bovino;
    if (s == "Caprino")  return Especie::Caprino;
    if (s == "Bufalino") return Especie::Bufalino;
    throw std::invalid_argument("Especie inválida: " + s);
}

inline std::string sexoToString(SexoGanado s) {
    switch (s) {
        case SexoGanado::Macho:  return "Macho";
        case SexoGanado::Hembra: return "Hembra";
    }
    throw std::invalid_argument("Sexo desconocido");
}

inline SexoGanado sexoFromString(const std::string& s) {
    if (s == "Macho")  return SexoGanado::Macho;
    if (s == "Hembra") return SexoGanado::Hembra;
    throw std::invalid_argument("Sexo inválido: " + s);
}

inline std::string estadoToString(EstadoGanado e) {
    switch (e) {
        case EstadoGanado::Activo:  return "Activo";
        case EstadoGanado::Muerto:  return "Muerto";
        case EstadoGanado::Vendido: return "Vendido";
        case EstadoGanado::Enfermo: return "Enfermo";
    }
    throw std::invalid_argument("Estado desconocido");
}

inline EstadoGanado estadoFromString(const std::string& s) {
    if (s == "Activo")  return EstadoGanado::Activo;
    if (s == "Muerto")  return EstadoGanado::Muerto;
    if (s == "Vendido") return EstadoGanado::Vendido;
    if (s == "Enfermo") return EstadoGanado::Enfermo;
    throw std::invalid_argument("Estado inválido: " + s);
}

// ─── Razas por especie ────────────────────────────────────────────────────────

inline const std::vector<std::string>& razasPorEspecie(Especie e) {
    static const std::vector<std::string> bovino = {
        "Aberdeen Angus", "Akaushi", "Aland", "American White Park",
        "Angus Negro", "Angus Rojo", "Beefalo", "Beefmaster",
        "Belted Galloway", "Blanco Orejinegro (BON)", "Blonde d'Aquitaine",
        "Bonsmara", "Boran", "Braford", "Brahman Gris", "Brahman Rojo",
        "Bramousin", "Brangus Negro", "Brangus Rojo", "Braunvieh",
        "British White", "Brown Swiss (Pardo Suizo de Leche)", "Buffalypso",
        "Caqueteño", "Caracu", "Carora", "Casanareño", "Charbray",
        "Charolais", "Chianina", "Chino Santandereano", "Costeño con Cuernos",
        "Criollo Argentino", "Criollo Boliviano", "Criollo Chileno",
        "Criollo de Chihuahua", "Criollo del Chaco", "Criollo Ecuatoriano",
        "Criollo Llanero", "Criollo Mexicano", "Criollo Paraguayo",
        "Criollo Peruano", "Criollo Uruguayo", "Criollo Venezolano",
        "Curraleiro Pé-Duro", "Dexter", "Florida Cracker", "Franqueiro",
        "Galloway", "Gelbvieh", "Gir standard (de carne)", "Gir Leiteiro (lechero)",
        "Guzerá", "Hartón del Valle", "Hays Converter", "Hereford Astado",
        "Hereford Mocho (Polled Hereford)", "Holstein Negro",
        "Holstein Rojo (Red Holstein)", "Indubrasil", "Jamaica Black",
        "Jamaica Hope", "Jamaica Red", "Jersey", "Limousin", "Lucerna",
        "Luing", "Maine-Anjou", "Marchigiana",
        "Mestizo (Ganado comercial / Cruzado)", "Montbéliarde", "Murray Grey",
        "Nelore Astado", "Nelore Mocho", "Normando", "Pantaneiro",
        "Piedmontese", "Pinzgauer", "Red Poll", "Romagnola", "Romosinuano",
        "Sahiwal", "Salers", "Sanmartinero", "Santa Gertrudis", "Senepol",
        "Shorthorn de Carne", "Shorthorn Lechero", "Simbrah",
        "Simmental (Fleckvieh tradicional)", "Simmental Americano",
        "South Devon", "Tabapua", "Texas Longhorn", "Tuli", "Velásquez",
        "Wagyu Negro", "Wagyu Marrón (Akaushi Americano)"
    };

    static const std::vector<std::string> caprino = {
        "Alpina Americana", "Alpina Británica", "Alpina Francesa", "Angora",
        "Azuaya", "Boer", "Cachemira", "Canindé", "Chaco", "Chilluda",
        "Criolla Argentina", "Criolla Boliviana", "Criolla Chilena",
        "Criolla Colombiana", "Criolla de las Antillas", "Criolla de San Juan",
        "Criolla Dominicana", "Criolla Haitiana", "Criolla Mexicana",
        "Criolla Paraguaya", "Criolla Peruana", "Criolla Venezolana",
        "Del Monte", "Kiko", "Kinder", "Kalahari Red", "La Mancha", "Marota",
        "Mestizo (Cabra comercial / Cruzada)", "Miotónica", "Moxotó",
        "Murciana", "Murciano-Granadina", "Nubiana (Anglo-Nubiana)",
        "Oberhasli", "Paconchita", "Pastoreña", "Pygora", "Repartida",
        "Saanen Americana", "Saanen Suiza", "Sabi", "San Clemente",
        "Savanna", "Somalí", "Spanish Goat", "Toggenburg Americana",
        "Toggenburg Suiza"
    };

    static const std::vector<std::string> bufalino = {
        "Bradawari", "Buffalypso", "Carabao (Búfalo de Pantano)",
        "Jafarabadi (Jaffarabadi)", "Kundi", "Mediterránea",
        "Mediterránea Italiana", "Meshana",
        "Mestizo (Búfalo comercial / Cruzado)", "Murrah", "Nagpuri",
        "Nili-Ravi", "Surti"
    };

    switch (e) {
        case Especie::Bovino:   return bovino;
        case Especie::Caprino:  return caprino;
        case Especie::Bufalino: return bufalino;
    }
    throw std::invalid_argument("Especie desconocida");
}

inline bool razaEsValidaParaEspecie(const std::string& raza, Especie especie) {
    const auto& razas = razasPorEspecie(especie);
    for (const auto& r : razas)
        if (r == raza) return true;
    return false;
}

// ─── Operadores para GTest ────────────────────────────────────────────────────

inline std::ostream& operator<<(std::ostream& os, Especie e) {
    return os << especieToString(e);
}

inline std::ostream& operator<<(std::ostream& os, SexoGanado s) {
    return os << sexoToString(s);
}

inline std::ostream& operator<<(std::ostream& os, EstadoGanado e) {
    return os << estadoToString(e);
}

} // namespace Domain