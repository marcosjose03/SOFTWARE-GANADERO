#pragma once
#include "../../domain/repositories/IProduccionRepository.h"
#include "../database/LmdbDatabase.h"
#include <memory>

namespace Infrastructure {

class LmdbProduccionRepository : public Domain::IProduccionRepository {
public:
    explicit LmdbProduccionRepository(std::shared_ptr<LmdbDatabase> db);

    [[nodiscard]] std::optional<Domain::Produccion> getById(const std::string& id) const override;
    bool insert(const Domain::Produccion& p)                                              override;
    bool deleteById(const std::string& id)                                                override;

    bool updatePrenez(const std::string& id, bool prenez)                                 override;
    bool updateOrdeno(const std::string& id, bool ordeno)                                 override;

    bool addParto(const std::string& id, const Domain::Parto& parto)                      override;
    bool updateParto(const std::string& id, const Domain::Parto& parto)                   override;
    bool deleteParto(const std::string& id, const std::string& partoId)                   override;

    bool addRegistroLeche(const std::string& id, const Domain::RegistroFecha& r)          override;
    bool updateRegistroLeche(const std::string& id,
                             const std::string& fechaOriginal,
                             const std::string& fechaNueva,
                             double nuevoValor)                                              override;
    bool deleteRegistroLeche(const std::string& id, const std::string& fecha)             override;

    bool addRegistroCarne(const std::string& id, const Domain::RegistroFecha& r)          override;
    bool updateRegistroCarne(const std::string& id,
                             const std::string& fecha, double nuevoValor)                 override;
    bool deleteRegistroCarne(const std::string& id, const std::string& fecha)             override;

private:
    std::shared_ptr<LmdbDatabase> m_db;

    [[nodiscard]] std::optional<Domain::Produccion> load(const std::string& id) const;
    bool save(const Domain::Produccion& p);

    static Domain::Produccion fromJson(const std::string& json);
    static std::string        toJson(const Domain::Produccion& p);
};

} // namespace Infrastructure