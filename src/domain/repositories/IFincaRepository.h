#pragma once
#include "../entities/Finca.h"
#include <vector>
#include <optional>
#include <string>

namespace Domain {

class IFincaRepository {
public:
    virtual ~IFincaRepository() = default;

    [[nodiscard]] virtual std::vector<Finca>   getAll()                            const = 0;
    [[nodiscard]] virtual std::optional<Finca> getById(const std::string& id)      const = 0;
    virtual bool                               insert(const Finca& finca)               = 0;
    virtual bool                               update(const Finca& finca)               = 0;
    virtual bool                               deleteById(const std::string& id)         = 0;
};

} // namespace Domain