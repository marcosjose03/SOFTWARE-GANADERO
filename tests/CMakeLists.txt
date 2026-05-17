find_package(SQLite3 REQUIRED)
find_package(unofficial-lmdb CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(ganadero_tests
    repositories/SqliteUsuarioRepositoryTest.cpp
    repositories/SqliteFincaRepositoryTest.cpp
    repositories/SqliteGanadoRepositoryTest.cpp
    repositories/LmdbProduccionRepositoryTest.cpp
    ${CMAKE_SOURCE_DIR}/src/infrastructure/database/SQLiteDatabase.cpp
    ${CMAKE_SOURCE_DIR}/src/infrastructure/database/LmdbDatabase.cpp
    ${CMAKE_SOURCE_DIR}/src/infrastructure/repositories/SqliteUsuarioRepository.cpp
    ${CMAKE_SOURCE_DIR}/src/infrastructure/repositories/SqliteFincaRepository.cpp
    ${CMAKE_SOURCE_DIR}/src/infrastructure/repositories/SqliteGanadoRepository.cpp
    ${CMAKE_SOURCE_DIR}/src/infrastructure/repositories/LmdbProduccionRepository.cpp
)

target_link_libraries(ganadero_tests PRIVATE
    gtest
    gtest_main
    SQLite3::SQLite3
    unofficial::lmdb::lmdb
    nlohmann_json::nlohmann_json
)

target_include_directories(ganadero_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(ganadero_tests)