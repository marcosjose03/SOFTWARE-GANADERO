#pragma once
#include "../entities/Ganado.h"
#include <vector>
#include <optional>
#include <string>

namespace Domain {

class IGanadoRepository {
public:
    virtual ~IGanadoRepository() = default;

    [[nodiscard]] virtual std::vector<Ganado>   getAll()                             const = 0;
    [[nodiscard]] virtual std::optional<Ganado> getById(const std::string& id)       const = 0;
    virtual bool                                insert(const Ganado& ganado)               = 0;
    virtual bool                                update(const Ganado& ganado)               = 0;
    virtual bool                                deleteById(const std::string& id)           = 0;
    [[nodiscard]] virtual std::vector<Ganado>   getByFinca(const std::string& idFinca) const = 0;
};



} // namespace Domain