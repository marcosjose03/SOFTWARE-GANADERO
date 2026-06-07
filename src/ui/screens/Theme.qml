pragma Singleton
import QtQuick 2.15

QtObject {
    // Paleta
    readonly property color fondo:           "#F5F6F2"
    readonly property color superficie:      "#FFFFFF"
    readonly property color superficieAlt:   "#ECEEE8"
    readonly property color primario:        "#2E7D4F"
    readonly property color primarioHover:   "#266B43"
    readonly property color primarioClaro:   "#E8F5EE"
    readonly property color textoPrimario:   "#1C1E1A"
    readonly property color textoSecundario: "#5F6359"
    readonly property color borde:           "#D4D7CE"
    readonly property color exito:           "#2E7D4F"
    readonly property color advertencia:     "#C9871F"
    readonly property color advertenciaFondo:"#FEF3E0"
    readonly property color critico:         "#C0392B"
    readonly property color criticoFondo:    "#FDECEA"
    readonly property color deshabilitado:   "#B8BBB2"

    // Espaciado (base 8)
    readonly property int espacioXs: 4
    readonly property int espacioSm: 8
    readonly property int espacioMd: 16
    readonly property int espacioLg: 24
    readonly property int espacioXl: 40

    // Tipografía
    readonly property string fuente:        Qt.platform.os === "windows" ? "Segoe UI" : "Helvetica Neue"
    readonly property int tamTitulo:        24
    readonly property int tamSubtitulo:     18
    readonly property int tamCuerpo:        15
    readonly property int tamEtiqueta:      13
    readonly property int tamMicro:         11

    // Bordes y radios
    readonly property int radio:   10
    readonly property int radioSm: 6
    readonly property int radioXl: 16

    // Dimensiones de controles
    readonly property int alturaCampo:      44
    readonly property int alturaBoton:      46
    readonly property int anchoFormulario:  420
    readonly property int alturaEncabezado: 56
}
