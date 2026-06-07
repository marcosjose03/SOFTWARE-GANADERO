import QtQuick 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: root

    property string label:       ""
    property bool   obligatorio: false
    default property alias contenido: campoSlot.data

    spacing: Theme.espacioXs

    Text {
        text:             root.label + (root.obligatorio ? " *" : "")
        font.family:      Theme.fuente
        font.pixelSize:   Theme.tamEtiqueta
        color:            Theme.textoSecundario
        Layout.fillWidth: true
    }

    Item {
        id: campoSlot
        Layout.fillWidth: true
        implicitHeight:   childrenRect.height
        height:           implicitHeight
    }
}
