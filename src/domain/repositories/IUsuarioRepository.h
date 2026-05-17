#pragma once
#include "../entities/Usuario.h"
#include <vector>
#include <optional>
#include <string>

namespace Domain {

class IUsuarioRepository {
public:
    virtual ~IUsuarioRepository() = default;

    [[nodiscard]] virtual std::vector<Usuario>    getAll()                              const = 0;
    [[nodiscard]] virtual std::optional<Usuario>  getById(const std::string& id)        const = 0;
    virtual bool                                  insert(const Usuario& usuario)              = 0;
    virtual bool                                  update(const Usuario& usuario)              = 0;
    virtual bool                                  deleteById(const std::string& id)           = 0;
};

} // namespace Domain