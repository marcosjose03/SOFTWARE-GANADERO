import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: modal
    anchors.centerIn: Overlay.overlay
    width: 400
    height: 320
    modal: true
    closePolicy: Popup.NoAutoClose

    signal campoActualizado()

    property string animalId:       ""
    property string campo:          ""
    property string valorPrevio:    ""
    property string tipo:           ""
    property string especieActual:  ""
    property var    razasList:      []
    property var    todosAnimales:  []
    property string animalIdActual: ""

    // UUID seleccionado para progenitor
    property string progenitorIdSeleccionado: ""

    function convertirFecha(f) {
        if (!f || f === "") return ""
        var p = f.split("/")
        if (p.length !== 3) return ""
        return p[2] + "-" + p[1] + "-" + p[0]
    }

    function abrirCampo(id, c, valor, t, especie) {
        animalId       = id
        campo          = c
        valorPrevio    = valor || ""
        tipo           = t
        especieActual  = especie || ""
        errorEdit.text = ""
        progenitorIdSeleccionado = ""
        calEditar.visible = false
        progenitorDrop.visible = false

        inputField.text  = (t === "texto" || t === "raza") ? valorPrevio : ""
        fechaField.text  = (t === "fecha") ? valorPrevio : ""

        if (t === "combo_especie") comboInput.model = appViewModel.getEspecies()
        if (t === "combo_sexo")   comboInput.model = appViewModel.getSexos()
        if (t === "combo_estado") comboInput.model = appViewModel.getEstados()
        if (t === "raza") {
            razasList = appViewModel.getRazasPorEspecie(especieActual)
            razaListView.model = razasList
            inputField.text = valorPrevio
        }
        if (t === "progenitor_macho" || t === "progenitor_hembra") {
            var sexoBuscar = (t === "progenitor_macho") ? "Macho" : "Hembra"
            var lista = []
            for (var i = 0; i < todosAnimales.length; i++) {
                var a = todosAnimales[i]
                if (a.sexo === sexoBuscar &&
                    a.especie === especieActual &&
                    a.id !== animalIdActual)
                    lista.push(a)
            }
            progenitorListView.model = lista
            progenitorBusqueda.text  = ""
            progenitorDrop.visible   = lista.length > 0
        }

        if (t.startsWith("combo")) {
            var idx = comboInput.model.indexOf(valorPrevio)
            comboInput.currentIndex = idx >= 0 ? idx : 0
        }

        modal.open()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Text { text: "Editar " + campo; font.bold: true; Layout.fillWidth: true }
            Button { text: "✕"; onClicked: modal.close() }
        }

        // Input texto / raza
        TextField {
            id: inputField
            Layout.fillWidth: true
            visible: tipo === "texto" || tipo === "raza"
            onTextChanged: {
                if (tipo !== "raza") return
                var busq = text.toLowerCase()
                var filtradas = []
                for (var i = 0; i < razasList.length; i++)
                    if (razasList[i].toLowerCase().indexOf(busq) >= 0)
                        filtradas.push(razasList[i])
                razaListView.model   = filtradas
                razaDropdown.visible = filtradas.length > 0
            }
            onActiveFocusChanged: {
                if (activeFocus && tipo === "raza" && razasList.length > 0) {
                    razaListView.model   = razasList
                    razaDropdown.visible = true
                }
            }
        }
        Rectangle {
            Layout.fillWidth: true
            height: 100
            border.color: "#ccc"
            clip: true
            visible: tipo === "raza"
            ListView {
                id: razaListView
                anchors.fill: parent
                clip: true
                model: []
                delegate: ItemDelegate {
                    width: razaListView.width
                    text: modelData
                    onClicked: {
                        inputField.text      = modelData
                        razaDropdown.visible = false
                    }
                }
            }
        }
        Rectangle {
            id: razaDropdown
            visible: false
            height: 0
        }

        // Input fecha
        RowLayout {
            Layout.fillWidth: true
            visible: tipo === "fecha"
            TextField {
                id: fechaField
                Layout.fillWidth: true
                placeholderText: "dd/mm/yyyy"
                readOnly: true
            }
            Button {
                text: "📅"
                onClicked: calEditar.visible = !calEditar.visible
            }
        }
        CalendarPopup {
            id: calEditar
            Layout.fillWidth: true
            visible: false
            onFechaSeleccionada: function(f) {
                fechaField.text   = f
                calEditar.visible = false
            }
        }

        // Combo
        ComboBox {
            id: comboInput
            Layout.fillWidth: true
            visible: tipo === "combo_especie" || tipo === "combo_sexo" || tipo === "combo_estado"
            model: []
        }

        // Progenitor — buscador
        ColumnLayout {
            Layout.fillWidth: true
            visible: tipo === "progenitor_macho" || tipo === "progenitor_hembra"
            spacing: 4

            TextField {
                id: progenitorBusqueda
                Layout.fillWidth: true
                placeholderText: "Buscar por identificador..."
                onTextChanged: {
                    var sexoBuscar = (tipo === "progenitor_macho") ? "Macho" : "Hembra"
                    var busq = text.toLowerCase()
                    var lista = []
                    for (var i = 0; i < todosAnimales.length; i++) {
                        var a = todosAnimales[i]
                        if (a.sexo === sexoBuscar &&
                            a.especie === especieActual &&
                            a.id !== animalIdActual &&
                            String(a.identificador).toLowerCase().indexOf(busq) >= 0)
                            lista.push(a)
                    }
                    progenitorListView.model = lista
                    progenitorDrop.visible   = lista.length > 0
                }
            }

            // Seleccionado actual
            Text {
                visible: progenitorIdSeleccionado !== ""
                text: "Seleccionado: " + progenitorBusqueda.text
                color: "#333"
                Layout.fillWidth: true
            }

            Rectangle {
                id: progenitorDrop
                Layout.fillWidth: true
                height: 120
                border.color: "#999"
                clip: true
                visible: false
                ListView {
                    id: progenitorListView
                    anchors.fill: parent
                    clip: true
                    model: []
                    delegate: ItemDelegate {
                        width: progenitorListView.width
                        text: String(modelData.identificador) + " — " + modelData.especie
                        onClicked: {
                            progenitorIdSeleccionado = modelData.id
                            progenitorBusqueda.text  = String(modelData.identificador)
                            progenitorDrop.visible   = false
                        }
                    }
                }
            }
        }

        Text {
            id: errorEdit
            color: "red"
            visible: text !== ""
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        Button {
            text: "Actualizar"
            Layout.fillWidth: true
            onClicked: {
                errorEdit.text = ""
                var ganado = appViewModel.getGanado(animalId)
                if (!ganado) { errorEdit.text = "Animal no encontrado"; return }

                if (tipo === "texto" || tipo === "raza") {
                    ganado[campo] = inputField.text
                } else if (tipo === "fecha") {
                    ganado[campo] = convertirFecha(fechaField.text)
                } else if (tipo.startsWith("combo")) {
                    ganado[campo] = comboInput.currentText
                } else if (tipo === "progenitor_macho" || tipo === "progenitor_hembra") {
                    if (progenitorIdSeleccionado === "") {
                        errorEdit.text = "Selecciona un animal de la lista"
                        return
                    }
                    ganado[campo] = progenitorIdSeleccionado
                }

                if (appViewModel.updateGanado(ganado)) {
                    modal.campoActualizado()
                    modal.close()
                } else {
                    errorEdit.text = "No se pudo actualizar"
                }
            }
        }
    }
}