cmake_minimum_required(VERSION 3.25)
project(SoftwareGanadero VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Charts)

# --- SQLite3 bundled (amalgamation en src/third_party/sqlite3) ---
add_library(sqlite3_bundled STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/sqlite3/sqlite3.c
)
target_include_directories(sqlite3_bundled PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/sqlite3
)
target_compile_options(sqlite3_bundled PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
)
if(WIN32)
    target_compile_definitions(sqlite3_bundled PRIVATE
        SQLITE_OS_WIN=1
    )
endif()

# --- LMDB bundled (fuentes en src/third_party/lmdb) ---
add_library(lmdb_bundled STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/lmdb/mdb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/lmdb/midl.c
)
target_include_directories(lmdb_bundled PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/lmdb
)
target_compile_options(lmdb_bundled PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
)
if(WIN32)
    target_link_libraries(lmdb_bundled PUBLIC ntdll)
endif()

# --- nlohmann_json header-only (FetchContent o sistema) ---
find_package(nlohmann_json CONFIG QUIET)
if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    message(STATUS "nlohmann_json no encontrado, descargando...")
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

add_subdirectory(src)

# Enlazar SQLite3 y LMDB al ejecutable desde aquí (targets ya definidos arriba)
target_link_libraries(SoftwareGanadero PRIVATE sqlite3_bundled lmdb_bundled)

if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests)
endif()
