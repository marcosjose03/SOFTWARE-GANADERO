import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: vistaLeche
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    property string animalId:  ""
    property var    animal:    null
    property var    registros: []

    signal volver()

    Connections {
        target: appViewModel
        function onErrorOccurred(message) {
            errorBanner.errorMessage = message
            errorBanner.visible      = true
        }
    }

    function recargar() {
        var produccion = appViewModel.getProduccion(animalId)
        var nuevosRegistros = produccion ? (produccion.registroLeche || []) : []
        listaRegistros.model = null
        listaRegistros.model = nuevosRegistros
        registros = nuevosRegistros
    }

    Component.onCompleted: recargar()

    Rectangle { anchors.fill: parent; color: "white" }

    function convertirFecha(f) {
        if (!f || f === "") return ""
        var p = f.split("/")
        if (p.length !== 3) return f
        return p[2] + "-" + p[1] + "-" + p[0]
    }

    // ── Popup nuevo / editar ──────────────────────────────────────────────
    Popup {
        id: registroPopup
        anchors.centerIn: Overlay.overlay
        width: 360
        height: 280
        modal: true
        closePolicy: Popup.NoAutoClose
        background: Rectangle { color: "white"; border.color: "#e0e0e0"; radius: 6 }

        property bool   esEdicion:     false
        property string fechaOriginal: ""

        function abrirNuevo() {
            esEdicion           = false
            fechaOriginal       = ""
            fechaField.text     = ""
            litrosField.text    = ""
            errorPopup.text     = ""
            calRegistro.visible = false
            tituloPopup.text    = "Nuevo Registro"
            registroPopup.open()
        }

        function abrirEdicion(fecha, valor) {
            esEdicion           = true
            fechaOriginal       = fecha
            fechaField.text     = fecha
            litrosField.text    = String(valor)
            errorPopup.text     = ""
            calRegistro.visible = false
            tituloPopup.text    = "Editar Registro"
            registroPopup.open()
        }

        Column {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 10

            // Header
            Row {
                width: parent.width
                Text {
                    id: tituloPopup
                    text: "Nuevo Registro"
                    font.bold: true
                    font.pixelSize: 15
                    width: parent.width - 40
                }
                Button {
                    text: "✕"
                    width: 32
                    height: 28
                    onClicked: registroPopup.close()
                }
            }

            // Fecha
            Text { text: "Fecha" }
            Row {
                width: parent.width
                spacing: 4
                TextField {
                    id: fechaField
                    width: parent.width - 44
                    placeholderText: "dd/mm/yyyy"
                    readOnly: true
                }
                Button {
                    width: 40
                    height: fechaField.height
                    text: "📅"
                    onClicked: calRegistro.visible = !calRegistro.visible
                }
            }
            CalendarPopup {
                id: calRegistro
                width: parent.width
                visible: false
                onFechaSeleccionada: function(f) {
                    fechaField.text     = f
                    calRegistro.visible = false
                }
            }

            // Litros
            Text { text: "Litros (usar punto para decimales)" }
            TextField {
                id: litrosField
                width: parent.width
                placeholderText: "Ej: 12.5"
                validator: RegularExpressionValidator {
                    regularExpression: /^[0-9]*\.?[0-9]*$/
                }
            }

            Text {
                id: errorPopup
                color: "red"
                visible: text !== ""
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Button {
                width: parent.width
                text: registroPopup.esEdicion ? "Actualizar" : "Guardar"
                onClicked: {
                    errorPopup.text = ""

                    if (fechaField.text === "") {
                        errorPopup.text = "La fecha es obligatoria"; return
                    }
                    if (litrosField.text.indexOf(",") >= 0) {
                        errorPopup.text = "Usa punto (.) en lugar de coma para decimales"; return
                    }

                    var litros = parseFloat(litrosField.text)
                    if (isNaN(litros) || litros < 0) {
                        errorPopup.text = "Ingresa un valor válido de litros"; return
                    }

                    var ok = false
                    if (registroPopup.esEdicion) {
                        var fechaOrigConv  = vistaLeche.convertirFecha(
                            registroPopup.fechaOriginal)
                        var fechaNuevaConv = vistaLeche.convertirFecha(
                            fechaField.text)
                        ok = appViewModel.updateRegistroLeche(
                            animalId, fechaOrigConv, fechaNuevaConv, litros)
                    } else {
                        ok = appViewModel.addRegistroLeche(
                            animalId,
                            vistaLeche.convertirFecha(fechaField.text),
                            litros)
                    }

                    if (ok) {
                        registroPopup.close()
                        vistaLeche.recargar()
                    }
                }
            }
        }
    }

    // ── Confirmación eliminar ─────────────────────────────────────────────
    Dialog {
        id: deleteDialog
        title: "Eliminar registro"
        modal: true
        anchors.centerIn: Overlay.overlay
        standardButtons: Dialog.Ok | Dialog.Cancel
        property string fechaAEliminar: ""
        Text { text: "¿Estás seguro de que deseas eliminar este registro?" }
        onAccepted: {
            appViewModel.deleteRegistroLeche(
                animalId,
                vistaLeche.convertirFecha(deleteDialog.fechaAEliminar))
            vistaLeche.recargar()
        }
    }

    // ── Layout principal ──────────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Button { text: "←"; onClicked: vistaLeche.volver() }
            Text {
                text: animal
                    ? "Leche — " + String(animal.identificador)
                      + " (" + animal.especie + ")"
                    : "Registro de Leche"
                font.pixelSize: 18
                font.bold: true
                Layout.fillWidth: true
            }
        }

        // Banner error
        Rectangle {
            id: errorBanner
            Layout.fillWidth: true
            height: visible ? 36 : 0
            color: "#ffdddd"
            border.color: "#cc0000"
            radius: 4
            visible: false
            property string errorMessage: ""

            RowLayout {
                anchors.fill: parent
                anchors.margins: 6
                Text {
                    text: errorBanner.errorMessage
                    color: "#cc0000"
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                }
                Button {
                    text: "✕"
                    implicitWidth: 24
                    implicitHeight: 24
                    onClicked: errorBanner.visible = false
                }
            }
        }

        // Lista registros
        ListView {
            id: listaRegistros
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: []
            spacing: 4
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            header: Rectangle {
                width: listaRegistros.width
                height: 36
                color: "#f0f0f0"
                Row {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 8
                    Text { text: "Fecha";  width: 120; font.bold: true }
                    Text { text: "Litros"; width: 100; font.bold: true }
                }
            }

            delegate: Rectangle {
                width: listaRegistros.width
                height: 40
                color: index % 2 === 0 ? "#f9f9f9" : "#ffffff"
                border.color: "#eee"

                Row {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 8

                    Text {
                        text: modelData.fecha || ""
                        width: 120
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Text {
                        text: modelData.valor !== undefined
                            ? String(modelData.valor) + " L" : ""
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Button {
                        text: "✎"
                        width: 32
                        height: 28
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: registroPopup.abrirEdicion(
                            modelData.fecha, modelData.valor)
                    }
                    Button {
                        text: "🗑"
                        width: 32
                        height: 28
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            deleteDialog.fechaAEliminar = modelData.fecha
                            deleteDialog.open()
                        }
                    }
                }
            }

            Text {
                anchors.centerIn: parent
                text: "No hay registros de leche"
                color: "#999"
                visible: listaRegistros.count === 0
            }
        }

        Button {
            text: "+ Agregar Registro"
            Layout.fillWidth: true
            onClicked: registroPopup.abrirNuevo()
        }
    }
}