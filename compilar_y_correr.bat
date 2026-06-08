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
REM --- Triplete MinGW (compatible con el compilador de Qt) ---
REM     Estas variables de entorno son leidas por vcpkg antes que los
REM     argumentos de cmake, evitando que use x64-windows (MSVC).
set "VCPKG_DEFAULT_TRIPLET=x64-mingw-dynamic"
set "VCPKG_DEFAULT_HOST_TRIPLET=x64-mingw-dynamic"

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
        -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%" ^
        -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic ^
        -DVCPKG_HOST_TRIPLET=x64-mingw-dynamic
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
echo   [3/4] Copiando librerias de Qt y vcpkg...
echo ============================================
"%QT_DIR%\bin\windeployqt.exe" --qmldir "%~dp0src\ui" "build\bin\SoftwareGanadero.exe"

REM --- Copiar DLLs de vcpkg (sqlite3, lmdb) que windeployqt no copia ---
set "VCPKG_BIN=build\vcpkg_installed\x64-mingw-dynamic\bin"
if exist "%VCPKG_BIN%\libsqlite3.dll"  copy /Y "%VCPKG_BIN%\libsqlite3.dll"  "build\bin\" >nul
if exist "%VCPKG_BIN%\liblmdb.dll"     copy /Y "%VCPKG_BIN%\liblmdb.dll"     "build\bin\" >nul

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
