cmake_minimum_required(VERSION 3.25)
project(SoftwareGanadero VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Charts)
find_package(SQLite3 REQUIRED)

# Soporte para lmdb via vcpkg o sistema
find_package(unofficial-lmdb CONFIG QUIET)
if(NOT unofficial-lmdb_FOUND)
    find_path(LMDB_INCLUDE_DIR lmdb.h)
    find_library(LMDB_LIBRARY NAMES lmdb)
    if(LMDB_INCLUDE_DIR AND LMDB_LIBRARY)
        add_library(unofficial::lmdb::lmdb UNKNOWN IMPORTED)
        set_target_properties(unofficial::lmdb::lmdb PROPERTIES
            IMPORTED_LOCATION "${LMDB_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${LMDB_INCLUDE_DIR}")
        message(STATUS "Found system LMDB: ${LMDB_LIBRARY}")
    else()
        message(FATAL_ERROR "LMDB not found")
    endif()
endif()

# Soporte para nlohmann_json via vcpkg o sistema
find_package(nlohmann_json CONFIG QUIET)
if(NOT nlohmann_json_FOUND)
    find_package(nlohmann_json REQUIRED)
endif()

add_subdirectory(src)

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