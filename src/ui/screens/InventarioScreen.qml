import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    property var fincasList: []

    function recargarFincas() {
        fincasList = appViewModel.getFincas()
        fincaGrid.model = fincasList
    }

    Component.onCompleted: recargarFincas()

    Popup {
        id: fincaModal
        anchors.centerIn: Overlay.overlay
        width: 360
        height: 340
        modal: true
        closePolicy: Popup.NoAutoClose

        property bool   esEdicion: false
        property string fincaId:   ""

        function abrirNueva() {
            esEdicion             = false
            fincaId               = ""
            nombreField.text      = ""
            hectareasField.text   = ""
            potrerosField.text    = ""
            capacidadField.text   = ""
            errorFinca.text       = ""
            btnRegistrar.text     = "Registrar Finca"
            fincaModal.open()
        }

        function abrirEdicion(finca) {
            esEdicion             = true
            fincaId               = finca.id
            nombreField.text      = finca.nombre
            hectareasField.text   = String(finca.nHectareas)
            potrerosField.text    = String(finca.nPotreros)
            capacidadField.text   = String(finca.capacidad)
            errorFinca.text       = ""
            btnRegistrar.text     = "Actualizar"
            fincaModal.open()
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 10

            RowLayout {
                Layout.fillWidth: true
                Text {
                    text: fincaModal.esEdicion ? "Editar Finca" : "Nueva Finca"
                    font.pixelSize: 16
                    font.bold: true
                    Layout.fillWidth: true
                }
                Button { text: "✕"; onClicked: fincaModal.close() }
            }

            Text { text: "Nombre" }
            TextField { id: nombreField; Layout.fillWidth: true }

            Text { text: "Número de hectáreas" }
            TextField {
                id: hectareasField
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
            }

            Text { text: "Número de potreros" }
            TextField {
                id: potrerosField
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
            }

            Text { text: "Capacidad" }
            TextField {
                id: capacidadField
                Layout.fillWidth: true
                inputMethodHints: Qt.ImhDigitsOnly
            }

            Button {
                text: "Eliminar finca"
                Layout.fillWidth: true
                visible: fincaModal.esEdicion
                onClicked: deleteFincaDialog.open()
            }

            Text {
                id: errorFinca
                color: "red"
                visible: text !== ""
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }

            Button {
                id: btnRegistrar
                text: "Registrar Finca"
                Layout.fillWidth: true
                onClicked: {
                    errorFinca.text = ""
                    if (nombreField.text === "") {
                        errorFinca.text = "El nombre es obligatorio"
                        return
                    }
                    var h = parseInt(hectareasField.text) || 0
                    var p = parseInt(potrerosField.text)  || 0
                    var c = parseInt(capacidadField.text) || 0
                    if (h <= 0 || p <= 0 || c <= 0) {
                        errorFinca.text = "Los valores numéricos deben ser mayores a 0"
                        return
                    }
                    var ok = fincaModal.esEdicion
                        ? appViewModel.updateFinca(fincaModal.fincaId, nombreField.text, h, p, c)
                        : appViewModel.createFinca(nombreField.text, h, p, c)
                    if (ok) {
                        fincaModal.close()
                        root.recargarFincas()
                    } else {
                        errorFinca.text = "No se pudo guardar la finca"
                    }
                }
            }
        }
    }

    Dialog {
        id: deleteFincaDialog
        title: "Eliminar finca"
        modal: true
        anchors.centerIn: Overlay.overlay
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            Text { text: "¿Estás seguro de que deseas eliminar esta finca?" }
            Text { id: deleteErrorText; color: "red"; visible: text !== "" }
        }

        onOpened: deleteErrorText.text = ""
        onAccepted: {
            if (appViewModel.fincaTieneAnimales(fincaModal.fincaId)) {
                deleteErrorText.text =
                    "No es posible eliminar la finca porque tiene animales registrados"
                deleteFincaDialog.open()
                return
            }
            appViewModel.deleteFinca(fincaModal.fincaId)
            fincaModal.close()
            root.recargarFincas()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            Button { text: "←"; onClicked: appViewModel.goToMenu() }
            Text {
                text: "Inventario"
                font.pixelSize: 18
                font.bold: true
                Layout.fillWidth: true
            }
        }

        GridView {
            id: fincaGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: 150
            cellHeight: 60
            model: fincasList

            header: Item {
                width: fincaGrid.width
                height: 60
                Button {
                    anchors.centerIn: parent
                    text: "+"
                    width: 140
                    height: 50
                    onClicked: fincaModal.abrirNueva()
                }
            }

            delegate: Button {
                width: 140
                height: 50
                text: modelData.nombre || ""
                onClicked: fincaModal.abrirEdicion(modelData)
            }
        }
    }
}