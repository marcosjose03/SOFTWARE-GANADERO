import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    property var  animalesList:      []
    property string selectedAnimalId: ""
    property var    selectedAnimal:   null

    Connections {
        target: appViewModel
        function onErrorOccurred(message) {
            errorBanner.errorMessage = message
            errorBanner.visible      = true
        }
    }

    function recargarAnimales() {
        var todos = appViewModel.getAllGanado()
        var conOrdeno = []
        for (var i = 0; i < todos.length; i++) {
            var ord = todos[i].ordeno
            if (ord === true || ord === "true" || ord === 1)
                conOrdeno.push(todos[i])
        }
        animalesList     = conOrdeno
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

    Rectangle { anchors.fill: parent; color: "white" }

    // ── Vista principal ───────────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12
        visible: !registroLoader.active

        RowLayout {
            Layout.fillWidth: true
            Button { text: "←"; onClicked: appViewModel.goToMenu() }
            Text {
                text: "Registro de Leche"
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

        // Filtros
        RowLayout {
            Layout.fillWidth: true
            CheckBox {
                id: checkBovino
                text: "Bovino"
                onCheckedChanged: animalGrid.model = filtrarAnimales()
            }
            CheckBox {
                id: checkCaprino
                text: "Caprino"
                onCheckedChanged: animalGrid.model = filtrarAnimales()
            }
            CheckBox {
                id: checkBufalino
                text: "Bufalino"
                onCheckedChanged: animalGrid.model = filtrarAnimales()
            }
            TextField {
                id: searchField
                Layout.fillWidth: true
                placeholderText: "Buscar por identificador..."
                onTextChanged: animalGrid.model = filtrarAnimales()
            }
        }

        GridView {
            id: animalGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: 120
            cellHeight: 50
            model: []
            delegate: Button {
                width: 110
                height: 44
                text: modelData.identificador !== undefined
                    ? String(modelData.identificador) : ""
                onClicked: {
                    selectedAnimalId = modelData.id
                    selectedAnimal   = modelData
                    registroLoader.active = true
                }
            }
        }
    }

    // ── Vista registros de leche ──────────────────────────────────────────
    Loader {
        id: registroLoader
        anchors.fill: parent
        active: false
        sourceComponent: RegistroLecheAnimalView {
            width:    root.width
            height:   root.height
            animalId: selectedAnimalId
            animal:   selectedAnimal
            onVolver: {
                registroLoader.active = false
                recargarAnimales()
            }
        }
    }
}