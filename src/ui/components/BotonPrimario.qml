import QtQuick 2.15
import QtQuick.Controls 2.15
import SoftwareGanadero 1.0

Button {
    id: root
    implicitHeight: Theme.alturaBoton
    focusPolicy:    Qt.StrongFocus

    background: Rectangle {
        radius: Theme.radio
        color:  !root.enabled     ? Theme.deshabilitado
              : root.pressed      ? Qt.darker(Theme.primario, 1.15)
              : root.hovered      ? Theme.primarioHover
                                  : Theme.primario
        Behavior on color { ColorAnimation { duration: 120 } }
    }

    contentItem: Text {
        text:                root.text
        color:               "white"
        font.family:         Theme.fuente
        font.pixelSize:      Theme.tamCuerpo
        font.weight:         Font.DemiBold
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:   Text.AlignVCenter
        elide:               Text.ElideRight
    }
}
