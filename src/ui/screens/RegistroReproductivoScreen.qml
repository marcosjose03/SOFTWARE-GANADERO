import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width: parent ? parent.width : 1024
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

    Rectangle { anchors.fill: parent; color: "white" }

    // ── Vista principal ───────────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12
        visible: !nuevoAnimalLoader.active && !detalleLoader.active

        RowLayout {
            Layout.fillWidth: true
            Button { text: "←"; onClicked: appViewModel.goToMenu() }
            Text {
                text: "Registro Reproductivo"
                font.pixelSize: 16; font.bold: true; color: "#1a1a1a"
            }
            Item { width: 8 }
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
                    selectedAnimalId    = modelData.id
                    detalleLoader.active = true
                }
            }
        }

        Button {
            text: "+ Agregar Animal"
            Layout.fillWidth: true
            onClicked: nuevoAnimalLoader.active = true
        }
    }

    // ── Loader nuevo animal ───────────────────────────────────────────────
    Loader {
        id: nuevoAnimalLoader
        anchors.fill: parent
        active: false
        sourceComponent: NuevoAnimalModal {
            width: root.width
            height: root.height
            onVolver: {
                nuevoAnimalLoader.active = false
            }
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
        active: false
        sourceComponent: DetalleAnimalComponent {
            width: root.width
            height: root.height
            animalId: selectedAnimalId
            onVolver: detalleLoader.active = false
            onAnimalActualizado: recargarAnimales()
        }
    }
}