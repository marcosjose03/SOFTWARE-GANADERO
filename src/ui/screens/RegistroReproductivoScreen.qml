import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    property string selectedAnimalId: ""
    property var    animalesList:     []

    function recargarAnimales() {
        animalesList     = appViewModel.getAllGanado()
        animalGrid.model = filtrarAnimales()
    }

    function filtrarAnimales() {
        var resultado    = []
        var busqueda     = searchField.text.toLowerCase()
        var filtroActivo = checkBovino.checked || checkCaprino.checked || checkBufalino.checked
        for (var i = 0; i < animalesList.length; i++) {
            var a = animalesList[i]
            if (filtroActivo) {
                var ok = (checkBovino.checked   && a.especie === "Bovino")  ||
                         (checkCaprino.checked  && a.especie === "Caprino") ||
                         (checkBufalino.checked && a.especie === "Bufalino")
                if (!ok) continue
            }
            if (busqueda !== "" &&
                String(a.identificador).toLowerCase().indexOf(busqueda) < 0)
                continue
            resultado.push(a)
        }
        return resultado
    }

    Component.onCompleted: recargarAnimales()

    Rectangle { anchors.fill: parent; color: Theme.fondo }

    // ── Vista principal (lista de animales) ───────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        spacing:      0
        visible:      !nuevoAnimalLoader.active && !detalleLoader.active

        EncabezadoPantalla {
            titulo:           "Registro Reproductivo"
            Layout.fillWidth: true
            onVolverClicked:  appViewModel.goToMenu()
        }

        // Barra de filtros
        Rectangle {
            Layout.fillWidth: true
            height:           filtrosRow.implicitHeight + Theme.espacioMd * 2
            color:            Theme.superficie
            border.color:     Theme.borde

            RowLayout {
                id: filtrosRow
                anchors {
                    left:    parent.left;  right:  parent.right
                    verticalCenter: parent.verticalCenter
                    leftMargin: Theme.espacioMd; rightMargin: Theme.espacioMd
                }
                spacing: Theme.espacioSm

                Text {
                    text:        "Filtrar:"
                    font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                    color:       Theme.textoSecundario
                }

                CheckBox {
                    id: checkBovino; text: "Bovino"
                    contentItem: Text {
                        leftPadding: checkBovino.indicator.width + 4
                        text:        checkBovino.text
                        font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                        color:       checkBovino.checked ? Theme.primario : Theme.textoSecundario
                        verticalAlignment: Text.AlignVCenter
                    }
                    onCheckedChanged: animalGrid.model = filtrarAnimales()
                }
                CheckBox {
                    id: checkCaprino; text: "Caprino"
                    contentItem: Text {
                        leftPadding: checkCaprino.indicator.width + 4
                        text:        checkCaprino.text
                        font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                        color:       checkCaprino.checked ? Theme.primario : Theme.textoSecundario
                        verticalAlignment: Text.AlignVCenter
                    }
                    onCheckedChanged: animalGrid.model = filtrarAnimales()
                }
                CheckBox {
                    id: checkBufalino; text: "Bufalino"
                    contentItem: Text {
                        leftPadding: checkBufalino.indicator.width + 4
                        text:        checkBufalino.text
                        font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                        color:       checkBufalino.checked ? Theme.primario : Theme.textoSecundario
                        verticalAlignment: Text.AlignVCenter
                    }
                    onCheckedChanged: animalGrid.model = filtrarAnimales()
                }

                Item { Layout.fillWidth: true }

                // Campo de búsqueda
                CampoTexto {
                    id:              searchField
                    width:           220
                    height:          36
                    placeholderText: "🔍 Buscar por identificador..."
                    onTextChanged:   animalGrid.model = filtrarAnimales()
                }
            }
        }

        // Área de contenido
        Item {
            Layout.fillWidth:  true
            Layout.fillHeight: true

            EstadoVacio {
                anchors.centerIn: parent
                icono:      "🐄"
                mensaje:    "No hay animales registrados.\nAgrega tu primer animal para comenzar."
                textoBoton: "+ Agregar Animal"
                visible:    animalGrid.model.length === 0
                onAcionClicked: nuevoAnimalLoader.active = true
            }

            GridView {
                id: animalGrid
                anchors.fill:    parent
                anchors.margins: Theme.espacioMd
                cellWidth:       Math.min(200, (width - Theme.espacioMd) / Math.max(1, Math.floor(width / 180)))
                cellHeight:      110
                model:           []
                visible:         count > 0

                delegate: Item {
                    width:  animalGrid.cellWidth - Theme.espacioSm
                    height: animalGrid.cellHeight - Theme.espacioSm

                    AbstractButton {
                        anchors.fill: parent
                        onClicked: {
                            selectedAnimalId     = modelData.id
                            detalleLoader.active = true
                        }

                        background: Tarjeta {
                            anchors.fill:  parent
                            border.color:  parent.pressed || parent.hovered
                                           ? Theme.primario : Theme.borde
                            Behavior on border.color { ColorAnimation { duration: 100 } }
                        }

                        contentItem: ColumnLayout {
                            anchors { fill: parent; margins: Theme.espacioSm }
                            spacing: Theme.espacioXs

                            Text {
                                text:           especieIcono(modelData.especie || "")
                                font.pixelSize: 24
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text:                String(modelData.identificador || "")
                                font.family:         Theme.fuente
                                font.pixelSize:      Theme.tamCuerpo
                                font.weight:         Font.DemiBold
                                color:               Theme.textoPrimario
                                horizontalAlignment: Text.AlignHCenter
                                elide:               Text.ElideRight
                                Layout.fillWidth:    true
                            }
                            Text {
                                text:                (modelData.especie || "") + " · " + (modelData.sexo || "")
                                font.family:         Theme.fuente
                                font.pixelSize:      Theme.tamMicro
                                color:               Theme.textoSecundario
                                horizontalAlignment: Text.AlignHCenter
                                Layout.fillWidth:    true
                            }
                        }
                    }
                }
            }
        }

        // Botón agregar animal (barra inferior)
        Rectangle {
            Layout.fillWidth: true
            height:           Theme.alturaBoton + Theme.espacioMd * 2
            color:            Theme.superficie

            Rectangle {
                anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top
                height: 1; color: Theme.borde
            }

            BotonPrimario {
                anchors.centerIn: parent
                width:   220
                height:  Theme.alturaBoton
                text:    "+ Agregar Animal"
                onClicked: nuevoAnimalLoader.active = true
            }
        }
    }

    // ── Loader nuevo animal ───────────────────────────────────────────────
    Loader {
        id: nuevoAnimalLoader
        anchors.fill: parent
        active:       false
        sourceComponent: NuevoAnimalModal {
            width:  root.width
            height: root.height
            onVolver:          { nuevoAnimalLoader.active = false }
            onAnimalRegistrado: {
                recargarAnimales()
                nuevoAnimalLoader.active = false
            }
        }
    }

    // ── Loader detalle animal ─────────────────────────────────────────────
    Loader {
        id: detalleLoader
        anchors.fill: parent
        active:       false
        sourceComponent: DetalleAnimalComponent {
            width:   root.width
            height:  root.height
            animalId: selectedAnimalId
            onVolver:          { detalleLoader.active = false }
            onAnimalActualizado: recargarAnimales()
        }
    }

    function especieIcono(especie) {
        if (especie === "Bovino")   return "🐄"
        if (especie === "Caprino")  return "🐐"
        if (especie === "Bufalino") return "🐃"
        return "🐾"
    }
}
