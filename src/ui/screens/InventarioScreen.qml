import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    property var fincasList: []

    function recargarFincas() {
        fincasList       = appViewModel.getFincas()
        fincaGrid.model  = fincasList
    }

    Component.onCompleted: recargarFincas()

    Rectangle { anchors.fill: parent; color: Theme.fondo }

    // ── Modal crear / editar finca ────────────────────────────────────────
    Popup {
        id: fincaModal
        anchors.centerIn: Overlay.overlay
        width:   420
        modal:   true
        padding: 0
        closePolicy: Popup.NoAutoClose

        property bool   esEdicion: false
        property string fincaId:   ""

        function abrirNueva() {
            esEdicion           = false
            fincaId             = ""
            nombreField.text    = ""
            hectareasField.text = ""
            potrerosField.text  = ""
            capacidadField.text = ""
            errorFinca.text     = ""
            fincaModal.open()
        }

        function abrirEdicion(finca) {
            esEdicion           = true
            fincaId             = finca.id
            nombreField.text    = finca.nombre
            hectareasField.text = String(finca.nHectareas)
            potrerosField.text  = String(finca.nPotreros)
            capacidadField.text = String(finca.capacidad)
            errorFinca.text     = ""
            fincaModal.open()
        }

        background: Tarjeta { radius: Theme.radio }

        ColumnLayout {
            anchors.fill:    parent
            anchors.margins: Theme.espacioLg
            spacing:         Theme.espacioMd

            // Encabezado del modal
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text:           fincaModal.esEdicion ? "Editar Finca" : "Nueva Finca"
                    font.family:    Theme.fuente
                    font.pixelSize: Theme.tamSubtitulo
                    font.weight:    Font.Bold
                    color:          Theme.textoPrimario
                    Layout.fillWidth: true
                }

                AbstractButton {
                    id: btnCerrarModal
                    implicitWidth: 32; implicitHeight: 32
                    onClicked: fincaModal.close()

                    background: Rectangle {
                        radius: Theme.radioSm
                        color:  btnCerrarModal.hovered ? Theme.superficieAlt : "transparent"
                    }
                    contentItem: Text {
                        text: "✕"
                        font.pixelSize: Theme.tamCuerpo
                        color: Theme.textoSecundario
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:   Text.AlignVCenter
                    }
                }
            }

            // Campo Nombre
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs
                Text {
                    text: "Nombre de la finca *"
                    font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                    color: Theme.textoSecundario
                }
                CampoTexto { id: nombreField; width: parent.width; placeholderText: "Ej: La Esperanza" }
            }

            // Campo Hectáreas
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs
                Text {
                    text: "Hectáreas *"
                    font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                    color: Theme.textoSecundario
                }
                CampoTexto {
                    id: hectareasField; width: parent.width
                    placeholderText: "Número de hectáreas"
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator { bottom: 1 }
                }
            }

            // Campo Potreros
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs
                Text {
                    text: "Potreros *"
                    font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                    color: Theme.textoSecundario
                }
                CampoTexto {
                    id: potrerosField; width: parent.width
                    placeholderText: "Número de potreros"
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator { bottom: 1 }
                }
            }

            // Campo Capacidad
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs
                Text {
                    text: "Capacidad (animales) *"
                    font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                    color: Theme.textoSecundario
                }
                CampoTexto {
                    id: capacidadField; width: parent.width
                    placeholderText: "Capacidad máxima"
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator { bottom: 1 }
                }
            }

            // Error
            Rectangle {
                Layout.fillWidth: true
                height:           errorFinca.implicitHeight + Theme.espacioSm
                radius:           Theme.radioSm
                color:            Theme.criticoFondo
                border.color:     Theme.critico
                border.width:     1
                visible:          errorFinca.text !== ""

                Text {
                    id: errorFinca
                    anchors.centerIn: parent
                    width:       parent.width - Theme.espacioMd
                    color:       Theme.critico
                    font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                    wrapMode:    Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // Botón eliminar (solo edición)
            BotonSecundario {
                text:             "Eliminar finca"
                Layout.fillWidth: true
                visible:          fincaModal.esEdicion
                onClicked:        deleteFincaDialog.open()

                contentItem: Text {
                    text:                parent.text
                    font.family:         Theme.fuente; font.pixelSize: Theme.tamCuerpo
                    color:               Theme.critico
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter
                }
            }

            // Botón guardar
            BotonPrimario {
                text:             fincaModal.esEdicion ? "Guardar cambios" : "Registrar Finca"
                Layout.fillWidth: true
                onClicked: {
                    errorFinca.text = ""
                    if (nombreField.text === "") {
                        errorFinca.text = "El nombre es obligatorio"; return
                    }
                    var h = parseInt(hectareasField.text) || 0
                    var p = parseInt(potrerosField.text)  || 0
                    var c = parseInt(capacidadField.text) || 0
                    if (h <= 0 || p <= 0 || c <= 0) {
                        errorFinca.text = "Los valores numéricos deben ser mayores a 0"; return
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

    // ── Diálogo de confirmación de eliminación ────────────────────────────
    Dialog {
        id: deleteFincaDialog
        title:           "Eliminar finca"
        modal:           true
        anchors.centerIn: Overlay.overlay
        standardButtons: Dialog.Ok | Dialog.Cancel

        Column {
            spacing: Theme.espacioSm
            Text {
                text:        "¿Confirmas eliminar esta finca?"
                font.family: Theme.fuente; font.pixelSize: Theme.tamCuerpo
                color:       Theme.textoPrimario
            }
            Text {
                id:          deleteErrorText
                color:       Theme.critico
                visible:     text !== ""
                font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                wrapMode:    Text.WordWrap
                width:       360
            }
        }

        onOpened:   deleteErrorText.text = ""
        onAccepted: {
            if (appViewModel.fincaTieneAnimales(fincaModal.fincaId)) {
                deleteErrorText.text = "No es posible eliminar: la finca tiene animales registrados"
                deleteFincaDialog.open()
                return
            }
            appViewModel.deleteFinca(fincaModal.fincaId)
            fincaModal.close()
            root.recargarFincas()
        }
    }

    // ── Layout principal ──────────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        spacing:      0

        // Encabezado
        EncabezadoPantalla {
            titulo:           "Inventario de Fincas"
            Layout.fillWidth: true
            onVolverClicked:  appViewModel.goToMenu()

            // Botón agregar en el encabezado
            BotonPrimario {
                anchors.right:         parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin:   Theme.espacioMd
                width:                 140
                height:                36
                text:                  "+ Nueva Finca"
                onClicked:             fincaModal.abrirNueva()

                background: Rectangle {
                    radius: Theme.radioSm
                    color:  parent.pressed ? Qt.darker(Theme.primario, 1.15)
                          : parent.hovered ? Theme.primarioHover
                                           : Theme.primario
                    Behavior on color { ColorAnimation { duration: 120 } }
                }
            }
        }

        // Área de contenido
        Item {
            Layout.fillWidth:  true
            Layout.fillHeight: true

            // Estado vacío
            EstadoVacio {
                anchors.centerIn: parent
                icono:      "🏚️"
                mensaje:    "Aún no tienes fincas registradas.\nCrea tu primera finca para comenzar."
                textoBoton: "Crear primera finca"
                visible:    fincaGrid.model.length === 0
                onAcionClicked: fincaModal.abrirNueva()
            }

            // Grid de fincas
            GridView {
                id: fincaGrid
                anchors.fill:          parent
                anchors.margins:       Theme.espacioLg
                anchors.topMargin:     Theme.espacioLg
                cellWidth:             Math.min(280, (width - Theme.espacioMd) / Math.max(1, Math.floor(width / 260)))
                cellHeight:            130
                model:                 fincasList
                visible:               fincasList.length > 0

                delegate: Item {
                    width:  fincaGrid.cellWidth - Theme.espacioMd
                    height: fincaGrid.cellHeight - Theme.espacioMd

                    AbstractButton {
                        anchors.fill: parent
                        onClicked:    fincaModal.abrirEdicion(modelData)

                        background: Tarjeta {
                            anchors.fill:  parent
                            border.color:  parent.pressed || parent.hovered ? Theme.primario : Theme.borde
                            Behavior on border.color { ColorAnimation { duration: 100 } }
                        }

                        contentItem: ColumnLayout {
                            anchors {
                                fill:    parent
                                margins: Theme.espacioMd
                            }
                            spacing: Theme.espacioXs

                            Row {
                                spacing: Theme.espacioSm
                                Text { text: "🏚️"; font.pixelSize: 22 }
                                Text {
                                    text:        modelData.nombre || ""
                                    font.family: Theme.fuente
                                    font.pixelSize: Theme.tamCuerpo
                                    font.weight:    Font.DemiBold
                                    color:          Theme.textoPrimario
                                    elide:          Text.ElideRight
                                    width:          parent.parent.width - 42
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }

                            Text {
                                text:        (modelData.nHectareas || "—") + " ha  ·  "
                                           + (modelData.nPotreros  || "—") + " potreros"
                                font.family: Theme.fuente
                                font.pixelSize: Theme.tamEtiqueta
                                color:          Theme.textoSecundario
                                Layout.fillWidth: true
                            }

                            Text {
                                text:        "Capacidad: " + (modelData.capacidad || "—") + " animales"
                                font.family: Theme.fuente
                                font.pixelSize: Theme.tamEtiqueta
                                color:          Theme.textoSecundario
                                Layout.fillWidth: true
                            }

                            Item { Layout.fillHeight: true }

                            Text {
                                text:        "Toca para editar →"
                                font.family: Theme.fuente
                                font.pixelSize: Theme.tamMicro
                                color:          Theme.primario
                            }
                        }
                    }
                }
            }
        }
    }
}
