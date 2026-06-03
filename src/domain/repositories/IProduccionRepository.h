#pragma once
#include "../entities/Produccion.h"
#include <optional>
#include <string>

namespace Domain {

class IProduccionRepository {
public:
    virtual ~IProduccionRepository() = default;

    // CRUD principal
    [[nodiscard]] virtual std::optional<Produccion> getById(const std::string& id)   const = 0;
    virtual bool                                    insert(const Produccion& p)            = 0;
    virtual bool                                    deleteById(const std::string& id)       = 0;

    // Campos simples
    virtual bool updatePrenez(const std::string& id, bool prenez)                          = 0;
    virtual bool updateOrdeno(const std::string& id, bool ordeno)                          = 0;

    // Partos
    virtual bool addParto(const std::string& id, const Parto& parto)                       = 0;
    virtual bool updateParto(const std::string& id, const Parto& parto)                    = 0;
    virtual bool deleteParto(const std::string& id, const std::string& partoId)            = 0;

    // Registro leche
    virtual bool addRegistroLeche(const std::string& id, const RegistroFecha& r)           = 0;
    virtual bool updateRegistroLeche(const std::string& id,
                                     const std::string& fecha,
                                     double nuevoValor)                                    = 0;
    virtual bool deleteRegistroLeche(const std::string& id, const std::string& fecha)      = 0;

    // Registro carne
    virtual bool addRegistroCarne(const std::string& id, const RegistroFecha& r)           = 0;
    virtual bool updateRegistroCarne(const std::string& id,
                                     const std::string& fecha,
                                     double nuevoValor)                                    = 0;
    virtual bool deleteRegistroCarne(const std::string& id, const std::string& fecha)      = 0;
};

} // namespace Domain