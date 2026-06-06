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
if(NOT SQLite3_FOUND)
    find_package(unofficial-sqlite3 CONFIG QUIET)
endif()
if(NOT SQLite3_FOUND AND NOT unofficial-sqlite3_FOUND)
    message(STATUS "SQLite3 no encontrado, descargando amalgamation...")
    FetchContent_Declare(
        sqlite3_src
        URL https://www.sqlite.org/2024/sqlite-amalgamation-3450100.zip
    )
    FetchContent_MakeAvailable(sqlite3_src)
    add_library(sqlite3_lib STATIC
        ${sqlite3_src_SOURCE_DIR}/sqlite3.c
    )
    target_include_directories(sqlite3_lib PUBLIC ${sqlite3_src_SOURCE_DIR})
    add_library(SQLite::SQLite3 ALIAS sqlite3_lib)
    set(SQLite3_FOUND TRUE)
endif()

# --- LMDB ---
find_package(unofficial-lmdb CONFIG QUIET)
if(NOT unofficial-lmdb_FOUND)
    find_path(LMDB_INCLUDE_DIR lmdb.h QUIET)
    find_library(LMDB_LIBRARY NAMES lmdb QUIET)
    if(LMDB_INCLUDE_DIR AND LMDB_LIBRARY)
        add_library(unofficial::lmdb::lmdb UNKNOWN IMPORTED)
        set_target_properties(unofficial::lmdb::lmdb PROPERTIES
            IMPORTED_LOCATION "${LMDB_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${LMDB_INCLUDE_DIR}")
        message(STATUS "LMDB del sistema: ${LMDB_LIBRARY}")
    else()
        message(STATUS "LMDB no encontrado, descargando fuente...")
        FetchContent_Declare(
            lmdb_src
            GIT_REPOSITORY https://github.com/LMDB/lmdb.git
            GIT_TAG        LMDB_0.9.31
        )
        FetchContent_MakeAvailable(lmdb_src)
        add_library(lmdb_lib STATIC
            ${lmdb_src_SOURCE_DIR}/libraries/liblmdb/mdb.c
            ${lmdb_src_SOURCE_DIR}/libraries/liblmdb/midl.c
        )
        target_include_directories(lmdb_lib PUBLIC
            ${lmdb_src_SOURCE_DIR}/libraries/liblmdb)
        if(WIN32)
            target_link_libraries(lmdb_lib PUBLIC ntdll)
        endif()
        add_library(unofficial::lmdb::lmdb ALIAS lmdb_lib)
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

add_subdirectory(src)

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
