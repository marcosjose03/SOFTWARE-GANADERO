# Software Ganadero — Contexto técnico del proyecto

> Documento de contexto para asistentes de IA (Claude u otros) y para asesoría técnica.
> Todo lo aquí escrito está **verificado leyendo el código** de la rama `claude/eager-gauss-0dBPk`.
> Cada afirmación cita el archivo que la sustenta. Lo no confirmable se marca `NO VERIFICADO`.
> Método de verificación: lectura directa de fuentes + compilación y ejecución real de la suite de pruebas.

---

## 0. Resumen en una frase

Aplicación **de escritorio** en **C++20 + Qt6/QML** con **Clean Architecture** por capas y **persistencia dual** (SQLite para datos relacionales, LMDB para producción como documento JSON). No hay componente web, servidor ni sincronización en nube: es 100% local. La lógica de negocio está bien separada y cubierta por ~109 pruebas de integración; los puntos débiles están en la capa de datos (contraseñas en texto plano, sin atomicidad entre los dos motores, escritura O(M) en LMDB).

---

## 1. Stack y compilación

| Aspecto | Valor | Fuente |
|---|---|---|
| Lenguaje | C++20 | `CMakeLists.txt:4`, `CMakePresets.json:11` |
| UI | Qt6 / QML (Qt Quick Controls 2) | `src/ui/main.qml` |
| Compilador | MinGW g++ 13.1.0 | `CMakePresets.json:13` |
| Qt | 6.11.1 | `CMakePresets.json:12` |
| Dependencias | sqlite3, lmdb, nlohmann-json (vcpkg) | `vcpkg.json` |
| Pruebas | GoogleTest (FetchContent, gate `BUILD_TESTING`) | `CMakeLists.txt` |
| Flag especial | `NO_CACHEGEN` en `qt_add_qml_module` (evita OOM con MinGW) | `src/CMakeLists.txt:39` |

**Estado de compilación (verificado en este análisis):**
- Las 12 fuentes no-Qt + LMDB (`src/third_party/lmdb/mdb.c`) + SQLite del sistema **compilan y enlazan sin errores**. Los 8 binarios de test enlazan y la suite corre (ver §7).
- El binario Qt completo **no se compila en el entorno de análisis** (falta Qt6): `AppViewModel.cpp` requiere `<QObject>`. `NO VERIFICADO` aquí, pero el `.exe` sí se generó en la máquina Windows del equipo durante la sesión, con un único ajuste de runtime: renombrar `liblmdb.dll` → `lmdb.dll`.

**Árbol (3 niveles, sin `build/`, `vcpkg_installed/`, `src/third_party/`):**

```
src/
├── application/    # dto/ · session/ · usecases/{usuario,finca,ganado,produccion}
├── domain/         # entities/ · repositories/
├── infrastructure/ # database/ · repositories/ · utils/
├── presentation/   # viewmodels/
└── ui/             # main.qml · screens/
tests/              # repositories/ · usecases/{usuario,finca,ganado,produccion}
```

---

## 2. Modelo de datos SQLite

DDL real en `src/infrastructure/database/SQLiteDatabase.cpp`, método `initSchema()` (líneas 29-76). **3 tablas**, `CREATE TABLE IF NOT EXISTS`.

### Tabla `usuarios`
| Columna | Tipo | Nota |
|---|---|---|
| id | TEXT | PRIMARY KEY |
| nombre | TEXT | NOT NULL |
| email | TEXT | NOT NULL (sin UNIQUE) |
| contrasena | TEXT | NOT NULL — **texto plano** |
| fecha_registro | TEXT | |
| fecha_ultimo_backup | TEXT | sin lógica asociada |

### Tabla `fincas`
| Columna | Tipo | Nota |
|---|---|---|
| id | TEXT | PRIMARY KEY |
| nombre | TEXT | NOT NULL |
| n_hectareas | INTEGER | NOT NULL |
| n_potreros | INTEGER | NOT NULL |
| capacidad | INTEGER | NOT NULL |
| id_usuario | TEXT | NOT NULL, FK → usuarios(id) |

### Tabla `ganado`
PK `id TEXT`. Columnas: `especie, identificador, id_usuario, id_finca, nacimiento, sexo, estado` (TEXT NOT NULL); `raza, chapeta, fecha_destete, fecha_ultimo_parto, fecha_ultima_palpacion, fecha_inseminacion` (TEXT); `foto` (BLOB); `id_padre, id_madre` (TEXT REFERENCES ganado(id)). FK: `id_usuario → usuarios(id)`, `id_finca → fincas(id)`.

**Observaciones:**
- **Índices:** ninguno explícito (`grep "CREATE INDEX"` → vacío). Solo los implícitos de PRIMARY KEY.
- **UNIQUE:** ninguno. `email` no tiene unicidad a nivel de BD (se valida en la app; ver §5).
- **PRAGMA** en el constructor (`SQLiteDatabase.cpp:12-13`): `journal_mode=WAL`, `foreign_keys=ON`.
- **Versionado de esquema:** `NO EXISTE`. Sin migraciones ni `PRAGMA user_version`; el esquema se crea idempotentemente al arrancar.

---

## 3. Modelo de datos LMDB (crítico)

Verificado en `src/infrastructure/repositories/LmdbProduccionRepository.cpp` y `LmdbDatabase.cpp`.

- **Qué guarda:** la producción de cada animal (preñez, ordeño, lista de partos, registros de leche, registros de carne).
- **Clave:** el **UUID del animal, crudo, sin prefijo** — `MDB_val key{ p.id.size(), p.id.data() }` (`save` línea 91, `load` línea 67). Es el mismo `id` que la fila en `ganado` (`Produccion.h:18`).
- **Valor:** JSON serializado con nlohmann (`toJson`, líneas 16-35).
- **Unidad de escritura:** el **historial COMPLETO del animal bajo una sola clave**. NO hay una entrada por evento. Cada operación es *read-modify-write* del documento entero.

Formato JSON del valor:
```json
{
  "id": "<uuid>",
  "prenez": false,
  "ordeno": true,
  "partos":         [ { "id": "<uuid>", "fecha": "dd/mm/aaaa" } ],
  "registro_leche": [ { "fecha": "aaaa-mm-dd", "valor": 12.5 } ],
  "registro_carne": [ { "fecha": "aaaa-mm-dd", "valor": 320.5 } ]
}
```

Patrón read-modify-write (`LmdbProduccionRepository.cpp:172-178`):
```cpp
bool addRegistroLeche(const std::string& id, const RegistroFecha& r) {
    auto p = load(id);              // lee el documento completo
    if (!p) return false;
    p->registroLeche.push_back(r);  // modifica en memoria
    return save(*p);                // re-serializa TODO y reescribe (mdb_put)
}
```

- **Consecuencia:** insertar el registro j-ésimo re-serializa j registros → **O(M) por animal**. Medido: 10.000 registros ≈ 15,5 s (ver `benchmarks/`).
- **Índices secundarios:** ninguno. `mdb_dbi_open(txn, nullptr, 0, &m_dbi)` (`LmdbDatabase.cpp:22`) abre la DBI por defecto sin flags. Toda consulta pasa por la clave = UUID.
- **Tamaño del mapa:** fijo 256 MB por defecto (`LmdbDatabase.h:10`).
- **Debug:** `save` imprime por `std::cerr` en cada escritura (`LmdbProduccionRepository.cpp:81-84`) — quitar antes de medir/producción.

---

## 4. Arquitectura y contratos

Clean/Onion por capas. Dependencia hacia adentro: `infrastructure` implementa las interfaces `I*Repository` de `domain/repositories/`.

### Casos de uso por módulo (`src/application/usecases/`)
| Módulo | Casos de uso | Firma representativa |
|---|---|---|
| Usuario | Create, Login, GetAll, GetById, Update, Delete, CheckEmailExists | `Login: LoginDto → optional<UsuarioResultDto>` (`UsuarioUseCases.cpp:57`) |
| Finca | Create, GetAll, GetById, Update, Delete | `Create: CreateFincaDto → optional<FincaResultDto>` |
| Ganado | Create, GetAll, GetById, Update, Delete, GetByFinca, ValidarProgenitores, ActualizarFechaPartaMadre | `Create: CreateGanadoDto → optional<GanadoResultDto>` (`GanadoUseCases.h:14`) |
| Producción | GetProduccion, Update{Prenez,Ordeno}, {Add,Update,Delete}Parto, {Add,Update,Delete}RegistroLeche, …Carne | `AddRegistroLeche: (id, RegistroFechaDto) → bool` (`ProduccionUseCases.h:69-74`) |

- **Manejo de errores en contratos:** sin excepciones de negocio. Éxito/fallo con `std::optional<T>` (consultas/creaciones) o `bool` (mutaciones). Sin códigos ni mensajes a nivel de caso de uso.

### Interfaces → implementación
| Interfaz (domain) | Implementación (infrastructure) | Métodos |
|---|---|---|
| IUsuarioRepository | SqliteUsuarioRepository | getAll, getById, getByEmail, insert, update, deleteById |
| IFincaRepository | SqliteFincaRepository | getAll, getById, insert, update, deleteById |
| IGanadoRepository | SqliteGanadoRepository | getAll, getById, insert, update, deleteById, getByFinca |
| IProduccionRepository | LmdbProduccionRepository | getById, insert, deleteById, update{Prenez,Ordeno}, {add,update,delete}Parto, {add,update,delete}Registro{Leche,Carne} |

- **Independencia del dominio: verificada.** Todos los `#include` de `src/domain/` son solo STL y cabeceras propias. `grep "sqlite3|lmdb|nlohmann|json|Q_OBJECT|QString"` sobre `src/domain/` → **vacío**. No hay SQL, JSON ni Qt filtrados al dominio. La conversión enum↔string vive en `GanadoEnums.h` (lógica de dominio pura).
- **Inyección de dependencias:** manual por constructor con `std::shared_ptr`, cableada en `src/main.cpp` (BDs → repos → casos de uso → ViewModel). Sin contenedor DI.

---

## 5. Puente C++ ↔ QML

- **Clase expuesta:** una sola, `Presentation::AppViewModel : QObject` (`AppViewModel.h:18`).
- **Registro:** por contexto — `engine.rootContext()->setContextProperty("appViewModel", appViewModel.get())` (`main.cpp:118`). No usa `qmlRegisterType`.
- **Propiedades Q_PROPERTY** (`AppViewModel.h:22-27`): `currentScreen` (NOTIFY currentScreenChanged), `isLoggedIn` y `userName` (NOTIFY sessionChanged).
- **Métodos invocables:** ~40 `Q_INVOKABLE` con tipos Qt — ej. `createGanado(QVariantMap)`, `getProduccion(QString)→QVariantMap`, `addRegistroLeche(QString,QString,double)`.
- **Navegación:** señal `currentScreenChanged` → `main.qml:26-38` hace `stackView.replace(...)` según el string de pantalla.
- **Refresco de listas:** manual; los .qml llaman `recargar()` tras cada operación. No hay modelos reactivos.
- **Errores al usuario:** señal `errorOccurred(QString)` (`AppViewModel.h:154`), emitida en 15 puntos (ej. `:115, :128, :140`). Las vistas la escuchan con `Connections { function onErrorOccurred(m){...} }` y muestran un banner rojo.
- **Validación de email duplicado:** SÍ existe, en el flujo C++ de `createAccount` (`AppViewModel.cpp:128` → `CheckEmailExistsUseCase`), aunque no hay UNIQUE en la BD.

---

## 6. Pantallas QML (14 archivos, enrutados por StackView en `main.qml`)

| Pantalla | Qué hace | Métodos AppViewModel |
|---|---|---|
| main.qml | Ventana raíz, StackView, enrutado por `currentScreen` | props/señales |
| LoginScreen | Inicio de sesión | login, goToRegister |
| RegisterScreen | Crear cuenta | createAccount |
| MenuScreen | Menú principal | goToInventario/Reproductivo/Leche/Peso, logout |
| InventarioScreen | CRUD de fincas (grid + modal) | getFincas, createFinca, updateFinca, deleteFinca |
| RegistroReproductivoScreen | Lista de animales → detalle | getAllGanado; abre DetalleAnimalComponent |
| RegistroLecheScreen | Lista de animales → registro leche | getAllGanado; abre RegistroLecheAnimalView |
| RegistroPesoScreen | Lista de animales → registro carne/peso | getAllGanado; abre RegistroCarneAnimalView |
| DetalleAnimalComponent | Ficha: campos, foto, preñez/ordeño | getGanado, getProduccion, updatePrenez, updateOrdeno, updateFoto |
| NuevoAnimalModal | Alta/edición de animal (714 líneas) | createGanado, getRazasPorEspecie, validarProgenitores |
| EditarCampoModal | Edición de un campo individual | updateGanado, getEspecies/Sexos/Estados |
| RegistroLecheAnimalView | Tabla add/update/delete de leche | getProduccion, addRegistroLeche, updateRegistroLeche, deleteRegistroLeche |
| RegistroCarneAnimalView | Tabla add/update/delete de carne/peso | getProduccion, addRegistroCarne, updateRegistroCarne, deleteRegistroCarne |
| CalendarPopup | Selector de fecha reutilizable | — (UI puro) |

- **Maquetas sin backend** (`MenuScreen.qml:125-127`): botones **"Herramienta visual", "Alertas", "Proyecciones"** con `enabled: false`. `Qt6::Charts` se enlaza en CMake pero **ningún QML lo importa** (`grep "import QtCharts"` → vacío): no hay gráficos.
- **Iconografía:** emoji/símbolos Unicode como texto de botón — `📅` (U+1F4C5), `🗑` (U+1F5D1), `✎` (U+270E), `✕` (U+2715), `←` (U+2190). Fotos vía data URI base64 (`DetalleAnimalComponent.qml:136-145`). `resources.qrc` solo empaqueta `ui/main.qml`, sin assets de icono. Render de emoji en la máquina destino: `NO VERIFICADO` (no se pudo ejecutar la GUI en el entorno de análisis).

---

## 7. Estado real por módulo

Leyenda: `completo` / `parcial` / `inexistente`.

| Módulo | Dominio | Caso de uso | Repositorio | UI | Pruebas |
|---|---|---|---|---|---|
| Usuario / login | completo | completo | completo | parcial¹ | completo (18+10) |
| Finca | completo | completo | completo | completo | completo (9+6) |
| Ganado (inventario) | completo | completo | completo | completo | completo (26+11) |
| Genealogía (padre/madre) | completo | completo | completo | parcial² | parcial |
| Producción – leche | completo | completo | completo | completo | completo |
| Producción – carne/peso | completo | completo | completo | completo | completo |
| Reproductivo – preñez/ordeño | completo | completo | completo | completo | completo (16+13) |
| Reproductivo – partos | completo | completo | completo | **inexistente³** | completo |
| Alertas / gráficos / proyecciones | inexistente | inexistente | inexistente | maqueta | inexistente |
| Sincronización / backup | inexistente⁴ | inexistente | inexistente | inexistente | inexistente |

1. Sin pantalla de editar perfil ni cambiar contraseña (`updateAccount` en backend, sin UI).
2. `validarProgenitores` se usa en NuevoAnimalModal, pero `getGanadoByFinca` no se usa en UI (se usa `getAllGanado`).
3. `addParto`/`updateParto`/`deleteParto` existen y están probados, pero ningún QML los llama; la UI solo maneja el campo suelto `fechaUltimoParto`.
4. Solo la columna `fecha_ultimo_backup`, sin lógica.

**Suite de pruebas (compilada y ejecutada realmente en el análisis):**
```
GanadoUseCasesTest            26   ProduccionUseCasesTest        13
UsuarioUseCasesTest           18   SqliteGanadoRepositoryTest    11
LmdbProduccionRepositoryTest  16   SqliteUsuarioRepositoryTest   10
FincaUseCasesTest              9   SqliteFincaRepositoryTest      6
--------------------------------------------------------------------
TOTAL: 109 pruebas — 109 PASAN, 0 FALLAN — 0,26 s (GoogleTest)
```
- Framework: GoogleTest. SQLite en `:memory:` recreado por `SetUp()`; LMDB en directorio temporal borrado con `remove_all` antes/después.
- **No hay pruebas de UI ni del `AppViewModel`** (la capa de presentación queda sin cobertura). Los tests son de integración (usan repositorios reales, no mocks).

**Lo que un usuario NO puede hacer hoy aunque exista lógica de respaldo:**
- Registrar/editar/eliminar partos (backend + pruebas listos, sin pantalla).
- Editar su cuenta o cambiar contraseña (`updateAccount` sin UI).
- Ver gráficos, alertas o proyecciones (maquetas deshabilitadas).
- Respaldar/sincronizar datos (sin implementación).
- Filtrar ganado por finca con consulta indexada (`getGanadoByFinca` sin UI; la app usa `getAllGanado`).

---

## 8. Viabilidad de 3 adiciones

### a) Importador Excel/CSV
- **Lectura de archivos existente:** solo `AppViewModel::leerArchivoBase64` (`AppViewModel.cpp:216`), para convertir imágenes a base64. **No hay parser CSV/Excel.** `ImageProcessor` solo redimensiona imágenes (`ImageProcessor.h:12`).
- **Librería:** CSV no requiere librería (`QTextStream`); Excel `.xlsx` sí (p. ej. QXlsx vía vcpkg, no presente).
- **Punto de entrada correcto:** nuevo `application/usecases/import/ImportUseCase` que por fila llame a los casos de uso existentes (`CreateGanadoUseCase`, `CreateFincaUseCase`, `AddRegistroLecheUseCase`, `AddRegistroCarneUseCase`) para respetar validaciones. Nunca escribir directo a los repositorios.
- **Archivos a tocar/crear:** `ImportUseCase` (+interfaz); `Q_INVOKABLE importarArchivo(QString)` en `AppViewModel`; QML con `FileDialog` (ya se importa `Qt.labs.platform` en `NuevoAnimalModal.qml:4`).
- **Riesgos:** dos formatos de fecha (leche/carne `aaaa-mm-dd`, partos `dd/mm/aaaa`); validar especie/raza contra enums; FK exigen finca/usuario previos; sin transacción de importación.
- **Estimación:** CSV 8-12 h; +Excel 4-6 h.

### b) Módulo de alertas sobre datos existentes
- **Consultas disponibles hoy:** ganado → getAll, getById, getByFinca; producción → **solo getById(uuid)**. No hay rango de fechas ni agregaciones.
- **Calculable sin entidades nuevas:** "sin pesaje en N días" (recorrer getAllGanado + getProduccion por animal — O(N) lecturas, sin índice); "caída de leche vs su promedio"; "días desde último parto" (`fechaUltimoParto` o lista `partos`).
- **Archivos a tocar/crear:** `application/usecases/alertas/AlertasUseCase` (cálculo puro sobre repos existentes); `Q_INVOKABLE getAlertas()→QVariantList`; pantalla QML (el botón "Alertas" ya existe deshabilitado). Sin entidad de dominio nueva.
- **Riesgos:** rendimiento (carga JSON completo de cada animal), dos formatos de fecha, umbrales configurables.
- **Estimación:** 10-16 h.

### c) Iconografía
- **Actual:** emoji como texto (`📅 🗑 ✎ ✕ ←`); fotos vía data URI base64; `resources.qrc` solo tiene `main.qml`, sin iconos.
- **Render de emoji en máquina destino:** `NO VERIFICADO` (depende de la fuente del SO; en Qt ha sido irregular — confirmar en la máquina objetivo).
- **Para robustez:** añadir SVG/PNG a `resources.qrc` y usar `source: "qrc:/icons/..."`.
- **Estimación:** 4-8 h.

---

## 9. Deuda técnica y riesgos (ordenados por gravedad)

1. **Contraseñas en texto plano.** `UsuarioUseCases.cpp:35` (copia literal al crear), `:61` (comparación en claro al login); persistida con `bind_text` (`SqliteUsuarioRepository.cpp:84`). Riesgo: leer `ganadero.db` expone todas las credenciales. Sin hashing ni sal.
2. **Sin integridad entre motores.** SQLite (ganado) y LMDB (producción) no comparten transacción. Al crear, el insert de producción **no se comprueba**: `m_produccionRepo->insert(p);` (`GanadoUseCases.cpp:119`) — si falla, animal sin producción y el caso de uso retorna éxito. Al borrar (`:201-202`) borra producción y luego ganado sin atomicidad ni rollback. Riesgo: documentos huérfanos/faltantes, indetectables.
3. **Escritura O(M) en LMDB.** Read-modify-write del documento completo en cada inserción (`LmdbProduccionRepository.cpp:172-178`, `save` `:80-98`) + `std::cerr` por escritura (`:81-84`). Riesgo: degradación con historiales largos (10.000 registros ≈ 15,5 s).
4. **Login ineficiente y sin unicidad en BD.** `LoginUseCase` hace `getAll()` + recorrido lineal en claro (`UsuarioUseCases.cpp:59-63`); `email` sin índice ni `UNIQUE`. Riesgo: O(n) por login y emails duplicados si se inserta fuera del flujo de la app.
5. **Errores pobres y pruebas parciales.** Casos de uso devuelven `bool`/`optional` sin causa; `std::cerr` disperso; `mapSize` LMDB fijo (256 MB) con `MDB_MAP_FULL` no manejado; 109 pruebas de integración pero **sin pruebas de UI ni de `AppViewModel`**.

**Lo más frágil hoy (3 líneas):** la partición en dos motores sin escritura atómica — un animal puede existir en SQLite sin su producción en LMDB (o al revés) y nada lo detecta ni lo repara. Sumado a contraseñas en texto plano y al reescribir el documento completo en cada registro (O(M)), la capa de datos es el punto débil. La lógica de negocio, en cambio, está bien separada y probada.

---

## 10. Artefactos relacionados en el repo

- `docs/Informe_Tecnico_SoftwareGanadero.docx` — informe en Word.
- `docs/Informe_Arquitectura_SoftwareGanadero.pdf` — este informe en PDF.
- `benchmarks/benchmark_persistencia.cpp` — mide inserción/lectura (N animales × M registros); demuestra la degradación O(M). Compila sin Qt.
- `benchmarks/README_BENCHMARK.md` — instrucciones de compilación (vcpkg/CMake y standalone) y resultado de referencia.
- `compilar_y_correr.bat` — build + windeployqt + copia de DLLs de vcpkg + lanzar, en un doble clic (Windows/MinGW).
