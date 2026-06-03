import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1 as Platform

Item {
    id: detalle
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    property string animalId:       ""
    property var    animalData:     null
    property var    produccionData: null
    property var    todosAnimales:  []
    property string errorMessage:   ""

    signal volver()
    signal animalActualizado()

    Connections {
        target: appViewModel
        function onErrorOccurred(message) {
            detalle.errorMessage = message
            errorBanner.visible  = true
        }
    }

    function cargar() {
        animalData     = appViewModel.getGanado(animalId)
        produccionData = appViewModel.getProduccion(animalId)
        todosAnimales  = appViewModel.getAllGanado()
    }

    Component.onCompleted: cargar()

    Dialog {
        id: deleteDialog
        title: "Eliminar animal"
        modal: true
        anchors.centerIn: Overlay.overlay
        standardButtons: Dialog.Ok | Dialog.Cancel
        Text { text: "¿Estás seguro de que deseas eliminar este animal?" }
        onAccepted: {
            appViewModel.deleteGanado(animalId)
            detalle.animalActualizado()
            detalle.volver()
        }
    }

    EditarCampoModal {
        id: editarModal
        todosAnimales:  detalle.todosAnimales
        animalIdActual: detalle.animalId
        onCampoActualizado: {
            cargar()
            detalle.animalActualizado()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 8

        // ── Header ────────────────────────────────────────────────────────
        RowLayout {
            Layout.fillWidth: true
            Button { text: "←"; onClicked: detalle.volver() }
            Text {
                text: animalData ? String(animalData.identificador) : ""
                font.pixelSize: 18
                font.bold: true
                Layout.fillWidth: true
            }
            Button {
                text: "Eliminar animal"
                onClicked: deleteDialog.open()
            }
        }

        // ── Banner de error ───────────────────────────────────────────────
        Rectangle {
            id: errorBanner
            Layout.fillWidth: true
            height: visible ? 36 : 0
            color: "#ffdddd"
            border.color: "#cc0000"
            radius: 4
            visible: false

            RowLayout {
                anchors.fill: parent
                anchors.margins: 6
                Text {
                    text: detalle.errorMessage
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

        // ── Contenido ─────────────────────────────────────────────────────
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            contentWidth: width
            contentHeight: camposColumn.implicitHeight + 32
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            Column {
                id: camposColumn
                width: parent.width - 12
                spacing: 8

                // ── Foto ──────────────────────────────────────────────────
                Row {
                    width: camposColumn.width
                    spacing: 8

                    Rectangle {
                        width: 120
                        height: 120
                        border.color: "#ccc"
                        color: "#f5f5f5"

                        Image {
                            id: fotoDetalle
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                            source: {
                                var b64 = appViewModel.getFotoBase64(animalId)
                                return b64 !== "" ? "data:image/png;base64," + b64 : ""
                            }
                            visible: source !== ""
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "Sin foto"
                            color: "#999"
                            visible: fotoDetalle.source === ""
                        }
                    }

                    Button {
                        text: "✎ Cambiar foto"
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            editarFotoPopup.fotoBase64Nueva = ""
                            fotoPreviewNueva.source = ""
                            editarFotoPopup.open()
                        }
                    }
                }

                // ── Popup editar foto ─────────────────────────────────────
                Popup {
                    id: editarFotoPopup
                    anchors.centerIn: Overlay.overlay
                    width: 320
                    height: 300
                    modal: true
                    closePolicy: Popup.NoAutoClose

                    property string fotoBase64Nueva: ""

                    Platform.FileDialog {
                        id: fotoDialogDetalle
                        title: "Seleccionar imagen"
                        nameFilters: ["Imágenes (*.png *.jpg *.jpeg *.bmp)"]
                        onAccepted: {
                            var b64 = appViewModel.leerArchivoBase64(
                                fotoDialogDetalle.file.toString())
                            if (b64 !== "") {
                                editarFotoPopup.fotoBase64Nueva = b64
                                fotoPreviewNueva.source = "data:image/png;base64," + b64
                            }
                        }
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        RowLayout {
                            Layout.fillWidth: true
                            Text {
                                text: "Cambiar foto"
                                font.bold: true
                                Layout.fillWidth: true
                            }
                            Button { text: "✕"; onClicked: editarFotoPopup.close() }
                        }

                        Rectangle {
                            Layout.alignment: Qt.AlignHCenter
                            width: 150
                            height: 150
                            border.color: "#ccc"
                            color: "#f5f5f5"

                            Image {
                                id: fotoPreviewNueva
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectCrop
                                source: ""
                                visible: source !== ""
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Vista previa"
                                color: "#999"
                                visible: fotoPreviewNueva.source === ""
                            }
                        }

                        Button {
                            text: "Seleccionar imagen"
                            Layout.fillWidth: true
                            onClicked: fotoDialogDetalle.open()
                        }

                        Button {
                            text: "Actualizar foto"
                            Layout.fillWidth: true
                            enabled: editarFotoPopup.fotoBase64Nueva !== ""
                            onClicked: {
                                if (appViewModel.updateFoto(
                                        animalId, editarFotoPopup.fotoBase64Nueva)) {
                                    fotoDetalle.source = "data:image/png;base64,"
                                        + editarFotoPopup.fotoBase64Nueva
                                    editarFotoPopup.fotoBase64Nueva = ""
                                    fotoPreviewNueva.source = ""
                                    editarFotoPopup.close()
                                    detalle.animalActualizado()
                                }
                            }
                        }
                    }
                }

                // ── Campos ────────────────────────────────────────────────
                Repeater {
                    model: {
                        if (!animalData) return []
                        var campos = [
                            { label: "Especie",
                              valor: animalData.especie,
                              campo: "especie",        tipo: "combo_especie" },
                            { label: "Identificador",
                              valor: String(animalData.identificador),
                              campo: "identificador",  tipo: "texto" },
                            { label: "Nacimiento",
                              valor: animalData.nacimiento,
                              campo: "nacimiento",     tipo: "fecha" },
                            { label: "Sexo",
                              valor: animalData.sexo,
                              campo: "sexo",           tipo: "combo_sexo" },
                            { label: "Estado",
                              valor: animalData.estado,
                              campo: "estado",         tipo: "combo_estado" },
                            { label: "Raza",
                              valor: animalData.raza,
                              campo: "raza",           tipo: "raza" },
                            { label: "Padre",
                              valor: animalData.padreIdentificador || "—",
                              campo: "padre",          tipo: "progenitor_macho" },
                            { label: "Madre",
                              valor: animalData.madreIdentificador || "—",
                              campo: "madre",          tipo: "progenitor_hembra" },
                            { label: "Chapeta",
                              valor: animalData.chapeta,
                              campo: "chapeta",        tipo: "texto" },
                            { label: "Destete",
                              valor: animalData.fechaDestete,
                              campo: "fechaDestete",   tipo: "fecha" }
                        ]
                        if (animalData.sexo === "Hembra") {
                            campos.push({ label: "Último Parto",
                                valor: animalData.fechaUltimoParto,
                                campo: "fechaUltimoParto",     tipo: "fecha" })
                            campos.push({ label: "Última Palpación",
                                valor: animalData.fechaUltimaPalpacion,
                                campo: "fechaUltimaPalpacion", tipo: "fecha" })
                            campos.push({ label: "Inseminación",
                                valor: animalData.fechaInseminacion,
                                campo: "fechaInseminacion",    tipo: "fecha" })
                        }
                        return campos
                    }

                    Row {
                        width: camposColumn.width
                        spacing: 8
                        Text { text: modelData.label + ":"; width: 150 }
                        Text {
                            text: modelData.valor || "—"
                            width: camposColumn.width - 200
                            elide: Text.ElideRight
                        }
                        Button {
                            text: "✎"
                            width: 32
                            height: 28
                            onClicked: editarModal.abrirCampo(
                                animalId, modelData.campo,
                                modelData.valor, modelData.tipo,
                                animalData.especie
                            )
                        }
                    }
                }

                // ── Preñez / Ordeño ───────────────────────────────────────
                Row {
                    width: camposColumn.width
                    spacing: 8
                    visible: animalData && animalData.sexo === "Hembra"
                    Text { text: "Preñez:"; width: 150 }
                    Switch {
                        checked: produccionData ? produccionData.prenez : false
                        onToggled: appViewModel.updatePrenez(animalId, checked)
                    }
                }

                Row {
                    width: camposColumn.width
                    spacing: 8
                    visible: animalData && animalData.sexo === "Hembra"
                    Text { text: "Ordeño:"; width: 150 }
                    Switch {
                        checked: produccionData ? produccionData.ordeno : false
                        onToggled: appViewModel.updateOrdeno(animalId, checked)
                    }
                }
            }
        }
    }
}