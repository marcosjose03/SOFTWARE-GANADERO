import QtQuick 2.15
import QtQuick.Controls 2.15
import SoftwareGanadero 1.0

Rectangle {
    id: root

    property string titulo: ""
    signal volverClicked()

    height: Theme.alturaEncabezado
    color:  Theme.superficie

    Rectangle {
        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.bottom: parent.bottom
        height:         1
        color:          Theme.borde
    }

    AbstractButton {
        id: btnVolver
        anchors.left:           parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin:     Theme.espacioMd
        width:                  40
        height:                 40
        onClicked:              root.volverClicked()

        background: Rectangle {
            radius: Theme.radioSm
            color:  btnVolver.hovered ? Theme.superficieAlt : "transparent"
            Behavior on color { ColorAnimation { duration: 100 } }
        }

        contentItem: Text {
            text:                "←"
            font.pixelSize:      Theme.tamSubtitulo
            font.family:         Theme.fuente
            color:               Theme.primario
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:   Text.AlignVCenter
        }
    }

    Text {
        anchors.centerIn:  parent
        text:              root.titulo
        font.family:       Theme.fuente
        font.pixelSize:    Theme.tamSubtitulo
        font.weight:       Font.DemiBold
        color:             Theme.textoPrimario
    }
}
