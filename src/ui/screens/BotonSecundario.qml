import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: root
    implicitHeight: Theme.alturaBoton
    focusPolicy:    Qt.StrongFocus

    background: Rectangle {
        radius:       Theme.radio
        color:        root.pressed || root.hovered ? Theme.superficieAlt : "transparent"
        border.color: !root.enabled ? Theme.deshabilitado : Theme.borde
        border.width: 1
        Behavior on color { ColorAnimation { duration: 120 } }
    }

    contentItem: Text {
        text:                root.text
        color:               !root.enabled ? Theme.deshabilitado : Theme.primario
        font.family:         Theme.fuente
        font.pixelSize:      Theme.tamCuerpo
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:   Text.AlignVCenter
        elide:               Text.ElideRight
    }
}
