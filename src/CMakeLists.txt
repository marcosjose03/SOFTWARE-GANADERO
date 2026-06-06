set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/utils/UuidGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/utils/DateTimeUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/utils/ImageProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/database/SQLiteDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/database/LmdbDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/SqliteUsuarioRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/SqliteFincaRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/SqliteGanadoRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/LmdbProduccionRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/usecases/usuario/UsuarioUseCases.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/usecases/finca/FincaUseCases.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/usecases/ganado/GanadoUseCases.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/usecases/produccion/ProduccionUseCases.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/session/SessionManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/presentation/viewmodels/AppViewModel.cpp
)

qt_add_executable(SoftwareGanadero ${SOURCES})
set_target_properties(SoftwareGanadero PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

qt_add_qml_module(SoftwareGanadero
    URI SoftwareGanadero
    VERSION 1.0
    QML_FILES
        ui/main.qml
        ui/screens/LoginScreen.qml
        ui/screens/RegisterScreen.qml
        ui/screens/MenuScreen.qml
        ui/screens/InventarioScreen.qml
        ui/screens/RegistroReproductivoScreen.qml
        ui/screens/RegistroLecheScreen.qml
        ui/screens/RegistroPesoScreen.qml
        ui/screens/CalendarPopup.qml
        ui/screens/EditarCampoModal.qml
        ui/screens/NuevoAnimalModal.qml
        ui/screens/DetalleAnimalComponent.qml
        ui/screens/RegistroLecheAnimalView.qml
        ui/screens/RegistroCarneAnimalView.qml
)

target_include_directories(SoftwareGanadero PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SoftwareGanadero PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Charts
    nlohmann_json::nlohmann_json
)

target_compile_options(SoftwareGanadero PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)
