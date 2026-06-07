import QtQuick 2.15

Column {
    id: root

    property string icono:      "📭"
    property string mensaje:    "No hay datos disponibles"
    property string textoBoton: ""
    signal accionClicked()

    spacing: Theme.espacioMd

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        text:           root.icono
        font.pixelSize: 52
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        text:                root.mensaje
        font.family:         Theme.fuente
        font.pixelSize:      Theme.tamCuerpo
        color:               Theme.textoSecundario
        horizontalAlignment: Text.AlignHCenter
        wrapMode:            Text.WordWrap
        width:               320
    }

    BotonPrimario {
        anchors.horizontalCenter: parent.horizontalCenter
        text:    root.textoBoton
        visible: root.textoBoton !== ""
        width:   220
        onClicked: root.accionClicked()
    }
}
