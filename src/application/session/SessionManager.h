#pragma once
#include <string>
#include <optional>
#include "application/dto/UsuarioDto.h"

namespace Application {

class SessionManager {
public:
    // Inicia sesión y persiste el id localmente
    void login(const UsuarioResultDto& usuario);

    // Cierra sesión y borra la persistencia local
    void logout();

    // Carga la sesión desde disco al arrancar la app
    // Retorna true si había sesión guardada y el usuario existe
    bool loadSession(const std::string& userId,
                     const UsuarioResultDto& usuario);

    [[nodiscard]] bool        isLoggedIn()  const;
    [[nodiscard]] std::string userId()      const;
    [[nodiscard]] std::string userName()    const;
    [[nodiscard]] std::string userEmail()   const;

    // Ruta del archivo donde se persiste el id de sesión
    [[nodiscard]] static std::string sessionFilePath();

private:
    std::optional<UsuarioResultDto> m_usuario;

    void persistSession(const std::string& userId);
    void clearPersistedSession();
};

} // namespace Application