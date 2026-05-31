import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: detalle
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    property string animalId:       ""
    property var    animalData:     null
    property var    produccionData: null
    property var    todosAnimales:  []

    signal volver()
    signal animalActualizado()

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
        todosAnimales: detalle.todosAnimales
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

                Repeater {
                    model: {
                        if (!animalData) return []
                        var campos = [
                            { label: "Especie",       valor: animalData.especie,
                              campo: "especie",        tipo: "combo_especie" },
                            { label: "Identificador", valor: String(animalData.identificador),
                              campo: "identificador",  tipo: "texto" },
                            { label: "Nacimiento",    valor: animalData.nacimiento,
                              campo: "nacimiento",     tipo: "fecha" },
                            { label: "Sexo",          valor: animalData.sexo,
                              campo: "sexo",           tipo: "combo_sexo" },
                            { label: "Estado",        valor: animalData.estado,
                              campo: "estado",         tipo: "combo_estado" },
                            { label: "Raza",          valor: animalData.raza,
                              campo: "raza",           tipo: "raza" },
                            { label: "Padre",         valor: animalData.padreIdentificador || "—",
                              campo: "padre",          tipo: "progenitor_macho" },
                            { label: "Madre",         valor: animalData.madreIdentificador || "—",
                              campo: "madre",          tipo: "progenitor_hembra" },
                            { label: "Chapeta",       valor: animalData.chapeta,
                              campo: "chapeta",        tipo: "texto" },
                            { label: "Destete",       valor: animalData.fechaDestete,
                              campo: "fechaDestete",   tipo: "fecha" }
                        ]
                        if (animalData.sexo === "Hembra") {
                            campos.push({ label: "Último Parto",
                                valor: animalData.fechaUltimoParto,
                                campo: "fechaUltimoParto",    tipo: "fecha" })
                            campos.push({ label: "Última Palpación",
                                valor: animalData.fechaUltimaPalpacion,
                                campo: "fechaUltimaPalpacion", tipo: "fecha" })
                            campos.push({ label: "Inseminación",
                                valor: animalData.fechaInseminacion,
                                campo: "fechaInseminacion",   tipo: "fecha" })
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

                Row {
                    width: camposColumn.width
                    spacing: 8
                    Text { text: "Preñez:"; width: 150 }
                    Switch {
                        checked: produccionData ? produccionData.prenez : false
                        onToggled: appViewModel.updatePrenez(animalId, checked)
                    }
                }

                Row {
                    width: camposColumn.width
                    spacing: 8
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