set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/database/SQLiteDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/database/LmdbDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/SqliteUsuarioRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/SqliteFincaRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/SqliteGanadoRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure/repositories/LmdbProduccionRepository.cpp
)

qt_add_executable(SoftwareGanadero ${SOURCES})

qt_add_qml_module(SoftwareGanadero
    URI SoftwareGanadero
    VERSION 1.0
    QML_FILES
        ui/main.qml
)

target_include_directories(SoftwareGanadero PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SoftwareGanadero PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Charts
    SQLite3::SQLite3
    unofficial::lmdb::lmdb
    nlohmann_json::nlohmann_json
)

target_compile_options(SoftwareGanadero PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)