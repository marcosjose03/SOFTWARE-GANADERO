import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: vistaCarne
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
        var nuevosRegistros = produccion ? (produccion.registroCarne || []) : []
        listaRegistros.model = null
        listaRegistros.model = nuevosRegistros
        registros = nuevosRegistros
    }

    Component.onCompleted: recargar()

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

        property bool   esEdicion:     false
        property string fechaOriginal: ""

        function abrirNuevo() {
            esEdicion           = false
            fechaOriginal       = ""
            fechaField.text     = ""
            kgField.text        = ""
            errorPopup.text     = ""
            calRegistro.visible = false
            tituloPopup.text    = "Nuevo Registro"
            registroPopup.open()
        }

        function abrirEdicion(fecha, valor) {
            esEdicion           = true
            fechaOriginal       = fecha
            fechaField.text     = fecha
            kgField.text        = String(valor)
            errorPopup.text     = ""
            calRegistro.visible = false
            tituloPopup.text    = "Editar Registro"
            registroPopup.open()
        }

        Column {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 10

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

            Text { text: "Kilogramos (usar punto para decimales)" }
            TextField {
                id: kgField
                width: parent.width
                placeholderText: "Ej: 320.5"
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
                    if (kgField.text.indexOf(",") >= 0) {
                        errorPopup.text = "Usa punto (.) en lugar de coma para decimales"; return
                    }

                    var kg = parseFloat(kgField.text)
                    if (isNaN(kg) || kg < 0) {
                        errorPopup.text = "Ingresa un valor válido de kilogramos"; return
                    }

                    var ok = false
                    if (registroPopup.esEdicion) {
                        var fechaOrigConv  = vistaCarne.convertirFecha(
                            registroPopup.fechaOriginal)
                        var fechaNuevaConv = vistaCarne.convertirFecha(
                            fechaField.text)
                        ok = appViewModel.updateRegistroCarne(
                            animalId, fechaOrigConv, fechaNuevaConv, kg)
                    } else {
                        ok = appViewModel.addRegistroCarne(
                            animalId,
                            vistaCarne.convertirFecha(fechaField.text),
                            kg)
                    }

                    if (ok) {
                        registroPopup.close()
                        vistaCarne.recargar()
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
            appViewModel.deleteRegistroCarne(
                animalId,
                vistaCarne.convertirFecha(deleteDialog.fechaAEliminar))
            vistaCarne.recargar()
        }
    }

    // ── Layout principal ──────────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Button { text: "←"; onClicked: vistaCarne.volver() }
            Text {
                text: animal
                    ? "Carne — " + String(animal.identificador)
                      + " (" + animal.especie + ")"
                    : "Registro de Carne"
                font.pixelSize: 18
                font.bold: true
                Layout.fillWidth: true
            }
        }

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
                    Text { text: "Fecha"; width: 120; font.bold: true }
                    Text { text: "Kg";    width: 100; font.bold: true }
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
                            ? String(modelData.valor) + " kg" : ""
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
                text: "No hay registros de carne"
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