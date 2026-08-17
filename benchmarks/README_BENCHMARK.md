# Benchmark de persistencia

`benchmark_persistencia.cpp` mide los tiempos reales del stack de persistencia
(SQLite para ganado + LMDB para producción) insertando **N** animales con **M**
registros de leche cada uno, sin depender de Qt.

## Qué mide

1. **Fase 1** — crear N animales (fila en SQLite `ganado` + documento `Produccion` vacío en LMDB).
2. **Fase 2** — insertar N×M registros de leche. Cada `addRegistroLeche` hace
   *read-modify-write* del documento completo del animal en LMDB, por lo que el
   registro j-ésimo cuesta re-serializar j registros → degradación **O(M)** por animal.
3. **Fase 3** — leer la producción completa de cada animal (LMDB `get` + parseo JSON).

Reporta ms totales y por unidad, más el tamaño en disco de `data.mdb` y `ganadero.db`.

## Uso

```
./benchmark_persistencia [N] [M]
```

- `N` = número de animales (por defecto 100)
- `M` = registros de leche por animal (por defecto 100)

## Compilación

### Opción A — con CMake/vcpkg del proyecto (Windows, MinGW)

Copia el ejecutable dentro del árbol ya configurado y compílalo enlazando las
mismas dependencias que usa la app. Desde la raíz del proyecto, con el `build`
ya generado por `compilar_y_correr.bat`:

```bat
g++ -std=c++20 -O2 ^
  -Isrc ^
  -Isrc\third_party\lmdb ^
  -Ibuild\vcpkg_installed\x64-mingw-dynamic\include ^
  benchmarks\benchmark_persistencia.cpp ^
  src\infrastructure\database\SQLiteDatabase.cpp ^
  src\infrastructure\database\LmdbDatabase.cpp ^
  src\infrastructure\repositories\SqliteUsuarioRepository.cpp ^
  src\infrastructure\repositories\SqliteFincaRepository.cpp ^
  src\infrastructure\repositories\SqliteGanadoRepository.cpp ^
  src\infrastructure\repositories\LmdbProduccionRepository.cpp ^
  src\infrastructure\utils\UuidGenerator.cpp ^
  src\infrastructure\utils\DateTimeUtils.cpp ^
  src\application\usecases\usuario\UsuarioUseCases.cpp ^
  src\application\usecases\finca\FincaUseCases.cpp ^
  src\application\usecases\ganado\GanadoUseCases.cpp ^
  src\application\usecases\produccion\ProduccionUseCases.cpp ^
  -Lbuild\vcpkg_installed\x64-mingw-dynamic\lib -lsqlite3 -llmdb ^
  -o benchmark_persistencia.exe
```

Copia también `libsqlite3.dll` y `liblmdb.dll` (de
`build\vcpkg_installed\x64-mingw-dynamic\bin`) junto al `.exe` antes de ejecutar.

### Opción B — standalone en Linux (usa las fuentes LMDB incluidas)

No necesita vcpkg: usa `libsqlite3` del sistema y compila LMDB desde
`src/third_party/lmdb/`. `<nlohmann/json.hpp>` debe estar disponible
(paquete `nlohmann-json3-dev` o el single-header en el include path).

```bash
SRC=src
g++ -std=c++20 -O2 -I$SRC -I$SRC/third_party/lmdb \
  benchmarks/benchmark_persistencia.cpp \
  $SRC/infrastructure/database/SQLiteDatabase.cpp \
  $SRC/infrastructure/database/LmdbDatabase.cpp \
  $SRC/infrastructure/repositories/SqliteUsuarioRepository.cpp \
  $SRC/infrastructure/repositories/SqliteFincaRepository.cpp \
  $SRC/infrastructure/repositories/SqliteGanadoRepository.cpp \
  $SRC/infrastructure/repositories/LmdbProduccionRepository.cpp \
  $SRC/infrastructure/utils/UuidGenerator.cpp \
  $SRC/infrastructure/utils/DateTimeUtils.cpp \
  $SRC/application/usecases/usuario/UsuarioUseCases.cpp \
  $SRC/application/usecases/finca/FincaUseCases.cpp \
  $SRC/application/usecases/ganado/GanadoUseCases.cpp \
  $SRC/application/usecases/produccion/ProduccionUseCases.cpp \
  $SRC/third_party/lmdb/mdb.c $SRC/third_party/lmdb/midl.c \
  -lsqlite3 -lpthread \
  -o benchmark_persistencia
```

## Nota importante para mediciones válidas

`LmdbProduccionRepository::save()` contiene `std::cerr` de depuración que imprime
en **cada** escritura. Redirige el error estándar para no medir el costo de I/O
de consola:

```bash
./benchmark_persistencia 100 100 2>/dev/null
```

O, mejor, elimina esas líneas de depuración
(`src/infrastructure/repositories/LmdbProduccionRepository.cpp`, ~líneas 81-84)
antes de medir.

## Resultado de referencia (Linux, -O2, N=100 M=100)

```
FASE 1  crear 100 animales             : ~74 ms    (0,74 ms/animal)
FASE 2  insertar 10.000 registros leche: ~15.566 ms (1,56 ms/registro)
FASE 3  leer produccion de 100 animales: ~98 ms    (0,98 ms/animal)
Tamaño data.mdb (LMDB)  : 696 KB
Tamaño ganadero.db      : 4 KB
```

La Fase 2 domina el tiempo por el patrón read-modify-write: no es el costo de
LMDB en sí, sino el re-serializar el documento entero del animal en cada
inserción. Es el hallazgo central para justificar, en el trabajo de grado, una
mejora hacia registros append-only o claves compuestas `animalId:fecha`.
