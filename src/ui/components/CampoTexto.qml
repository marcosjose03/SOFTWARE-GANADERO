import QtQuick 2.15
import QtQuick.Controls 2.15
import SoftwareGanadero 1.0

TextField {
    id: root
    implicitHeight:       Theme.alturaCampo
    leftPadding:          Theme.espacioMd
    rightPadding:         Theme.espacioMd
    font.family:          Theme.fuente
    font.pixelSize:       Theme.tamCuerpo
    color:                Theme.textoPrimario
    placeholderTextColor: Theme.textoSecundario
    selectByMouse:        true

    background: Rectangle {
        radius:       Theme.radioSm
        color:        root.enabled ? Theme.superficie : Theme.superficieAlt
        border.color: root.activeFocus ? Theme.primario : Theme.borde
        border.width: root.activeFocus ? 2 : 1
        Behavior on border.color { ColorAnimation { duration: 100 } }
        Behavior on border.width { NumberAnimation  { duration: 100 } }
    }
}
