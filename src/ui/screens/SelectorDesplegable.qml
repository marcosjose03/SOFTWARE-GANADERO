import QtQuick 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: root
    implicitHeight: Theme.alturaCampo
    font.family:    Theme.fuente
    font.pixelSize: Theme.tamCuerpo

    background: Rectangle {
        radius:       Theme.radioSm
        color:        root.enabled ? Theme.superficie : Theme.superficieAlt
        border.color: root.popup.visible ? Theme.primario : Theme.borde
        border.width: root.popup.visible ? 2 : 1
        Behavior on border.color { ColorAnimation { duration: 100 } }
    }

    contentItem: Text {
        leftPadding:       Theme.espacioMd
        text:              root.displayText
        font:              root.font
        color:             root.enabled ? Theme.textoPrimario : Theme.textoSecundario
        verticalAlignment: Text.AlignVCenter
        elide:             Text.ElideRight
    }

    indicator: Text {
        x:              root.width - width - Theme.espacioMd
        y:              (root.height - height) / 2
        text:           root.popup.visible ? "▴" : "▾"
        font.pixelSize: Theme.tamEtiqueta
        color:          Theme.textoSecundario
    }

    popup: Popup {
        y:             root.height + Theme.espacioXs
        width:         root.width
        implicitHeight: Math.min(contentItem.implicitHeight, 280)
        padding:       1

        background: Rectangle {
            radius:       Theme.radioSm
            color:        Theme.superficie
            border.color: Theme.borde
            border.width: 1
        }

        contentItem: ListView {
            clip:  true
            model: root.popup.visible ? root.delegateModel : null
            ScrollIndicator.vertical: ScrollIndicator {}
        }
    }

    delegate: ItemDelegate {
        width:  root.width
        height: Theme.alturaCampo

        contentItem: Text {
            text:              modelData
            font:              root.font
            color:             Theme.textoPrimario
            verticalAlignment: Text.AlignVCenter
            leftPadding:       Theme.espacioMd
            elide:             Text.ElideRight
        }

        background: Rectangle {
            color: highlighted ? Theme.superficieAlt : Theme.superficie
        }

        highlighted: root.highlightedIndex === index
    }
}
