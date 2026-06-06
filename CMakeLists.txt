cmake_minimum_required(VERSION 3.25)
project(SoftwareGanadero VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(FetchContent)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Charts)

# --- SQLite3 ---
find_package(SQLite3 QUIET)
if(SQLite3_FOUND)
    set(SQLITE3_TARGET SQLite::SQLite3)
else()
    find_package(unofficial-sqlite3 CONFIG QUIET)
    if(unofficial-sqlite3_FOUND)
        set(SQLITE3_TARGET unofficial::sqlite3::sqlite3)
    else()
        message(STATUS "SQLite3 no encontrado, descargando amalgamation...")
        FetchContent_Declare(
            sqlite3_src
            URL https://www.sqlite.org/2024/sqlite-amalgamation-3450100.zip
        )
        FetchContent_MakeAvailable(sqlite3_src)
        add_library(sqlite3_bundled STATIC
            ${sqlite3_src_SOURCE_DIR}/sqlite3.c
        )
        target_include_directories(sqlite3_bundled PUBLIC ${sqlite3_src_SOURCE_DIR})
        set(SQLITE3_TARGET sqlite3_bundled)
    endif()
endif()

# --- LMDB ---
find_package(unofficial-lmdb CONFIG QUIET)
if(unofficial-lmdb_FOUND)
    set(LMDB_TARGET unofficial::lmdb::lmdb)
else()
    find_path(LMDB_INCLUDE_DIR lmdb.h QUIET)
    find_library(LMDB_LIBRARY NAMES lmdb QUIET)
    if(LMDB_INCLUDE_DIR AND LMDB_LIBRARY)
        add_library(lmdb_system UNKNOWN IMPORTED)
        set_target_properties(lmdb_system PROPERTIES
            IMPORTED_LOCATION "${LMDB_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${LMDB_INCLUDE_DIR}")
        set(LMDB_TARGET lmdb_system)
        message(STATUS "LMDB del sistema: ${LMDB_LIBRARY}")
    else()
        message(STATUS "LMDB no encontrado, descargando fuente...")
        FetchContent_Declare(
            lmdb_src
            GIT_REPOSITORY https://github.com/LMDB/lmdb.git
            GIT_TAG        LMDB_0.9.31
        )
        FetchContent_MakeAvailable(lmdb_src)
        add_library(lmdb_bundled STATIC
            ${lmdb_src_SOURCE_DIR}/libraries/liblmdb/mdb.c
            ${lmdb_src_SOURCE_DIR}/libraries/liblmdb/midl.c
        )
        target_include_directories(lmdb_bundled PUBLIC
            ${lmdb_src_SOURCE_DIR}/libraries/liblmdb)
        if(WIN32)
            target_link_libraries(lmdb_bundled PUBLIC ntdll)
        endif()
        set(LMDB_TARGET lmdb_bundled)
    endif()
endif()

# --- nlohmann_json ---
find_package(nlohmann_json CONFIG QUIET)
if(NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json no encontrado, descargando...")
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# Pasar targets resueltos al subdirectorio
set(SQLITE3_TARGET ${SQLITE3_TARGET} CACHE INTERNAL "")
set(LMDB_TARGET    ${LMDB_TARGET}    CACHE INTERNAL "")

add_subdirectory(src)

# Link SQLite3 and LMDB here (after add_subdirectory) so FetchContent targets are guaranteed to exist
target_link_libraries(SoftwareGanadero PRIVATE ${SQLITE3_TARGET} ${LMDB_TARGET})

if(BUILD_TESTING)
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
