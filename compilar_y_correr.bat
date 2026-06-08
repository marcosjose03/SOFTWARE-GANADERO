@echo off
REM ============================================================
REM   GanSoft - Compilar y Correr (doble clic para usar)
REM ============================================================
setlocal

REM --- Rutas de Qt y herramientas (ajusta si cambias de version) ---
set "QT_DIR=C:\Qt\6.11.1\mingw_64"
set "CMAKE_BIN=C:\Qt\Tools\CMake_64\bin"
set "MINGW_BIN=C:\Qt\Tools\mingw1310_64\bin"

REM --- vcpkg: usar variable de entorno VCPKG_ROOT o buscar en ruta comun ---
if "%VCPKG_ROOT%"=="" set "VCPKG_ROOT=C:\vcpkg"
set "VCPKG_TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"

REM --- Agregar herramientas al PATH de esta ventana ---
set "PATH=%CMAKE_BIN%;%MINGW_BIN%;%QT_DIR%\bin;%PATH%"

cd /d "%~dp0"

REM --- Verificar que existe el toolchain de vcpkg ---
if not exist "%VCPKG_TOOLCHAIN%" (
    echo.
    echo   ERROR: No se encontro vcpkg en "%VCPKG_ROOT%"
    echo   Instala vcpkg en C:\vcpkg o define la variable VCPKG_ROOT.
    echo   Ver: https://learn.microsoft.com/vcpkg/get_started/get-started
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================
echo   [1/4] Configurando proyecto...
echo ============================================
if not exist "build\CMakeCache.txt" (
    cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
        -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%"
    if errorlevel 1 goto error
) else (
    echo    Ya configurado, omitiendo.
)

echo.
echo ============================================
echo   [2/4] Compilando (de a 1 para no quedarse sin memoria)...
echo ============================================
cmake --build build -j1
if errorlevel 1 goto error

echo.
echo ============================================
echo   [3/4] Copiando librerias de Qt...
echo ============================================
"%QT_DIR%\bin\windeployqt.exe" --qmldir "%~dp0src\ui" "build\bin\SoftwareGanadero.exe"

echo.
echo ============================================
echo   [4/4] Abriendo la aplicacion...
echo ============================================
start "" "build\bin\SoftwareGanadero.exe"

echo.
echo   Listo. La ventana de GanSoft deberia haberse abierto.
echo.
pause
exit /b 0

:error
echo.
echo   *** Hubo un error. Revisa los mensajes de arriba. ***
echo.
pause
exit /b 1
