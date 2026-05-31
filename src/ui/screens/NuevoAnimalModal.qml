import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: vista
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    signal animalRegistrado()
    signal volver()

    property var  fincasList:         []
    property var  razasList:          []
    property var  todosAnimales:      []
    property bool esHembra:           false
    property bool especieSeleccionada: false

    function cargar() {
        fincasList    = appViewModel.getFincas()
        todosAnimales = appViewModel.getAllGanado()
        limpiar()
    }

    function limpiar() {
        especieCombo.currentIndex       = 0
        identificadorField.text         = ""
        fincaCombo.currentIndex         = 0
        nacimientoField.text            = ""
        sexoCombo.currentIndex          = 0
        estadoCombo.currentIndex        = 0
        razaField.text                  = ""
        razaDropdown.visible            = false
        padreSearch.text                = ""
        padreSelectedId.text            = ""
        madreSearch.text                = ""
        madreSelectedId.text            = ""
        chapetaField.text               = ""
        desteteField.text               = ""
        ultimoPartoField.text           = ""
        ultimaPalpacionField.text       = ""
        inseminacionField.text          = ""
        errorText.text                  = ""
        esHembra                        = false
        especieSeleccionada             = false
        razasList                       = []
        calNac.visible                  = false
        calDes.visible                  = false
        calPar.visible                  = false
        calPal.visible                  = false
        calIns.visible                  = false
        padreDropdown.visible           = false
        madreDropdown.visible           = false
    }

    function convertirFecha(f) {
        if (!f || f === "") return ""
        var p = f.split("/")
        if (p.length !== 3) return ""
        return p[2] + "-" + p[1] + "-" + p[0]
    }

    Component.onCompleted: cargar()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            Button { text: "←"; onClicked: vista.volver() }
            Text {
                text: "Nuevo Animal"
                font.pixelSize: 18
                font.bold: true
                Layout.fillWidth: true
            }
        }

        Flickable {
            id: formFlick
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            contentWidth: width
            contentHeight: formColumn.implicitHeight + 32
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            Column {
                id: formColumn
                width: formFlick.width - 12
                spacing: 8

                // ── Especie ───────────────────────────────────────────────
                Text { text: "Especie *"; width: parent.width }
                ComboBox {
                    id: especieCombo
                    width: parent.width
                    model: ["Seleccionar especie...", "Bovino", "Caprino", "Bufalino"]
                    currentIndex: 0
                    onCurrentIndexChanged: {
                        if (currentIndex > 0) {
                            vista.especieSeleccionada = true
                            vista.razasList = appViewModel.getRazasPorEspecie(
                                model[currentIndex])
                            razaField.text       = ""
                            razaDropdown.visible = false
                        } else {
                            vista.especieSeleccionada = false
                            vista.razasList      = []
                            razaField.text       = ""
                            razaDropdown.visible = false
                        }
                    }
                }

                // ── Identificador ─────────────────────────────────────────
                Text {
                    text: "Identificador *"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                TextField {
                    id: identificadorField
                    width: parent.width
                    placeholderText: "Ej: ab1234"
                    enabled: vista.especieSeleccionada
                }

                // ── Finca ─────────────────────────────────────────────────
                Text {
                    text: "Finca *"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                ComboBox {
                    id: fincaCombo
                    width: parent.width
                    enabled: vista.especieSeleccionada
                    model: {
                        var nombres = ["Seleccionar finca..."]
                        for (var i = 0; i < vista.fincasList.length; i++)
                            nombres.push(vista.fincasList[i].nombre)
                        return nombres
                    }
                    currentIndex: 0
                }

                // ── Nacimiento ────────────────────────────────────────────
                Text {
                    text: "Nacimiento"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                Row {
                    width: parent.width
                    spacing: 4
                    TextField {
                        id: nacimientoField
                        width: parent.width - 44
                        placeholderText: "dd/mm/yyyy"
                        readOnly: true
                        enabled: vista.especieSeleccionada
                    }
                    Button {
                        width: 40
                        text: "📅"
                        enabled: vista.especieSeleccionada
                        onClicked: {
                            calDes.visible = false; calPar.visible = false
                            calPal.visible = false; calIns.visible = false
                            calNac.visible = !calNac.visible
                        }
                    }
                }
                CalendarPopup {
                    id: calNac
                    width: parent.width
                    visible: false
                    onFechaSeleccionada: function(f) {
                        nacimientoField.text = f
                        calNac.visible       = false
                    }
                }

                // ── Sexo ──────────────────────────────────────────────────
                Text {
                    text: "Sexo *"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                ComboBox {
                    id: sexoCombo
                    width: parent.width
                    model: ["Seleccionar sexo...", "Macho", "Hembra"]
                    currentIndex: 0
                    enabled: vista.especieSeleccionada
                    onCurrentIndexChanged: vista.esHembra = (currentIndex === 2)
                }

                // ── Estado ────────────────────────────────────────────────
                Text {
                    text: "Estado *"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                ComboBox {
                    id: estadoCombo
                    width: parent.width
                    model: ["Seleccionar estado...", "Activo", "Muerto", "Vendido", "Enfermo"]
                    currentIndex: 0
                    enabled: vista.especieSeleccionada
                }

                // ── Raza ──────────────────────────────────────────────────
                Text {
                    text: "Raza"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                TextField {
                    id: razaField
                    width: parent.width
                    placeholderText: "Buscar raza..."
                    enabled: vista.especieSeleccionada
                    onTextChanged: {
                        if (!vista.especieSeleccionada || vista.razasList.length === 0) {
                            razaDropdown.visible = false
                            return
                        }
                        var busq = text.toLowerCase()
                        var filtradas = []
                        for (var i = 0; i < vista.razasList.length; i++)
                            if (vista.razasList[i].toLowerCase().indexOf(busq) >= 0)
                                filtradas.push(vista.razasList[i])
                        var esExacta = false
                        for (var j = 0; j < vista.razasList.length; j++)
                            if (vista.razasList[j].toLowerCase() === busq) {
                                esExacta = true; break
                            }
                        razaListView.model   = filtradas
                        razaDropdown.visible = filtradas.length > 0 && !esExacta
                    }
                    onActiveFocusChanged: {
                        if (activeFocus && vista.especieSeleccionada
                                && vista.razasList.length > 0) {
                            razaListView.model   = vista.razasList
                            razaDropdown.visible = true
                        }
                    }
                }
                Rectangle {
                    id: razaDropdown
                    width: parent.width
                    height: 150
                    border.color: "#999"
                    clip: true
                    visible: false
                    ListView {
                        id: razaListView
                        anchors.fill: parent
                        clip: true
                        model: []
                        delegate: ItemDelegate {
                            width: razaListView.width
                            text: modelData
                            onClicked: {
                                razaField.text       = modelData
                                razaDropdown.visible = false
                            }
                        }
                    }
                }

                // ── Padre ─────────────────────────────────────────────────
                Text {
                    text: "Padre"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                TextField {
                    id: padreSearch
                    width: parent.width
                    placeholderText: "Buscar por identificador..."
                    enabled: vista.especieSeleccionada
                    onTextChanged: {
                        var busq    = text.toLowerCase()
                        var especie = especieCombo.currentIndex > 0
                            ? especieCombo.model[especieCombo.currentIndex] : ""
                        var filtrados = []
                        for (var i = 0; i < vista.todosAnimales.length; i++) {
                            var a = vista.todosAnimales[i]
                            if (a.sexo === "Macho" &&
                                a.especie === especie &&
                                String(a.identificador).toLowerCase().indexOf(busq) >= 0)
                                filtrados.push(a)
                        }
                        padreListView.model   = filtrados
                        padreDropdown.visible = filtrados.length > 0
                    }
                    onActiveFocusChanged: {
                        if (activeFocus && vista.especieSeleccionada) {
                            var especie = especieCombo.currentIndex > 0
                                ? especieCombo.model[especieCombo.currentIndex] : ""
                            var machos = []
                            for (var i = 0; i < vista.todosAnimales.length; i++) {
                                var a = vista.todosAnimales[i]
                                if (a.sexo === "Macho" && a.especie === especie)
                                    machos.push(a)
                            }
                            padreListView.model   = machos
                            padreDropdown.visible = machos.length > 0
                        }
                    }
                }
                Rectangle {
                    id: padreDropdown
                    width: parent.width
                    height: 120
                    border.color: "#999"
                    clip: true
                    visible: false
                    ListView {
                        id: padreListView
                        anchors.fill: parent
                        clip: true
                        model: []
                        delegate: ItemDelegate {
                            width: padreListView.width
                            text: String(modelData.identificador) + " — " + modelData.especie
                            onClicked: {
                                padreSelectedId.text  = modelData.id
                                padreSearch.text      = String(modelData.identificador)
                                padreDropdown.visible = false
                            }
                        }
                    }
                }
                Text {
                    id: padreSelectedId
                    text: ""
                    color: "#555"
                    visible: false
                    width: parent.width
                }

                // ── Madre ─────────────────────────────────────────────────
                Text {
                    text: "Madre"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                TextField {
                    id: madreSearch
                    width: parent.width
                    placeholderText: "Buscar por identificador..."
                    enabled: vista.especieSeleccionada
                    onTextChanged: {
                        var busq    = text.toLowerCase()
                        var especie = especieCombo.currentIndex > 0
                            ? especieCombo.model[especieCombo.currentIndex] : ""
                        var filtradas = []
                        for (var i = 0; i < vista.todosAnimales.length; i++) {
                            var a = vista.todosAnimales[i]
                            if (a.sexo === "Hembra" &&
                                a.especie === especie &&
                                String(a.identificador).toLowerCase().indexOf(busq) >= 0)
                                filtradas.push(a)
                        }
                        madreListView.model   = filtradas
                        madreDropdown.visible = filtradas.length > 0
                    }
                    onActiveFocusChanged: {
                        if (activeFocus && vista.especieSeleccionada) {
                            var especie = especieCombo.currentIndex > 0
                                ? especieCombo.model[especieCombo.currentIndex] : ""
                            var hembras = []
                            for (var i = 0; i < vista.todosAnimales.length; i++) {
                                var a = vista.todosAnimales[i]
                                if (a.sexo === "Hembra" && a.especie === especie)
                                    hembras.push(a)
                            }
                            madreListView.model   = hembras
                            madreDropdown.visible = hembras.length > 0
                        }
                    }
                }
                Rectangle {
                    id: madreDropdown
                    width: parent.width
                    height: 120
                    border.color: "#999"
                    clip: true
                    visible: false
                    ListView {
                        id: madreListView
                        anchors.fill: parent
                        clip: true
                        model: []
                        delegate: ItemDelegate {
                            width: madreListView.width
                            text: String(modelData.identificador) + " — " + modelData.especie
                            onClicked: {
                                madreSelectedId.text  = modelData.id
                                madreSearch.text      = String(modelData.identificador)
                                madreDropdown.visible = false
                            }
                        }
                    }
                }
                Text {
                    id: madreSelectedId
                    text: ""
                    color: "#555"
                    visible: false
                    width: parent.width
                }

                // ── Chapeta ───────────────────────────────────────────────
                Text {
                    text: "Chapeta"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                TextField {
                    id: chapetaField
                    width: parent.width
                    placeholderText: "Opcional"
                    enabled: vista.especieSeleccionada
                }

                // ── Fecha Destete ─────────────────────────────────────────
                Text {
                    text: "Fecha Destete"
                    opacity: vista.especieSeleccionada ? 1 : 0.4
                    width: parent.width
                }
                Row {
                    width: parent.width
                    spacing: 4
                    TextField {
                        id: desteteField
                        width: parent.width - 44
                        placeholderText: "dd/mm/yyyy"
                        readOnly: true
                        enabled: vista.especieSeleccionada
                    }
                    Button {
                        width: 40
                        text: "📅"
                        enabled: vista.especieSeleccionada
                        onClicked: {
                            calNac.visible = false; calPar.visible = false
                            calPal.visible = false; calIns.visible = false
                            calDes.visible = !calDes.visible
                        }
                    }
                }
                CalendarPopup {
                    id: calDes
                    width: parent.width
                    visible: false
                    onFechaSeleccionada: function(f) {
                        desteteField.text = f
                        calDes.visible    = false
                    }
                }

                // ── Campos hembra ─────────────────────────────────────────
                Text {
                    text: "Fecha Último Parto"
                    visible: vista.esHembra
                    width: parent.width
                }
                Row {
                    width: parent.width
                    spacing: 4
                    visible: vista.esHembra
                    TextField {
                        id: ultimoPartoField
                        width: parent.width - 44
                        placeholderText: "dd/mm/yyyy"
                        readOnly: true
                    }
                    Button {
                        width: 40
                        text: "📅"
                        onClicked: {
                            calNac.visible = false; calDes.visible = false
                            calPal.visible = false; calIns.visible = false
                            calPar.visible = !calPar.visible
                        }
                    }
                }
                CalendarPopup {
                    id: calPar
                    width: parent.width
                    visible: false
                    onFechaSeleccionada: function(f) {
                        ultimoPartoField.text = f
                        calPar.visible        = false
                    }
                }

                Text {
                    text: "Fecha Última Palpación"
                    visible: vista.esHembra
                    width: parent.width
                }
                Row {
                    width: parent.width
                    spacing: 4
                    visible: vista.esHembra
                    TextField {
                        id: ultimaPalpacionField
                        width: parent.width - 44
                        placeholderText: "dd/mm/yyyy"
                        readOnly: true
                    }
                    Button {
                        width: 40
                        text: "📅"
                        onClicked: {
                            calNac.visible = false; calDes.visible = false
                            calPar.visible = false; calIns.visible = false
                            calPal.visible = !calPal.visible
                        }
                    }
                }
                CalendarPopup {
                    id: calPal
                    width: parent.width
                    visible: false
                    onFechaSeleccionada: function(f) {
                        ultimaPalpacionField.text = f
                        calPal.visible            = false
                    }
                }

                Text {
                    text: "Fecha Inseminación"
                    visible: vista.esHembra
                    width: parent.width
                }
                Row {
                    width: parent.width
                    spacing: 4
                    visible: vista.esHembra
                    TextField {
                        id: inseminacionField
                        width: parent.width - 44
                        placeholderText: "dd/mm/yyyy"
                        readOnly: true
                    }
                    Button {
                        width: 40
                        text: "📅"
                        onClicked: {
                            calNac.visible = false; calDes.visible = false
                            calPar.visible = false; calPal.visible = false
                            calIns.visible = !calIns.visible
                        }
                    }
                }
                CalendarPopup {
                    id: calIns
                    width: parent.width
                    visible: false
                    onFechaSeleccionada: function(f) {
                        inseminacionField.text = f
                        calIns.visible         = false
                    }
                }

                Text {
                    id: errorText
                    color: "red"
                    visible: text !== ""
                    width: parent.width
                    wrapMode: Text.WordWrap
                }

                Button {
                    width: parent.width
                    text: "Registrar Animal"
                    onClicked: {
                        errorText.text = ""
                        if (especieCombo.currentIndex === 0) {
                            errorText.text = "Selecciona la especie"; return
                        }
                        if (identificadorField.text === "") {
                            errorText.text = "El identificador es obligatorio"; return
                        }
                        if (fincaCombo.currentIndex <= 0) {
                            errorText.text = "Selecciona una finca"; return
                        }
                        if (sexoCombo.currentIndex === 0) {
                            errorText.text = "Selecciona el sexo"; return
                        }
                        if (estadoCombo.currentIndex === 0) {
                            errorText.text = "Selecciona el estado"; return
                        }

                        var idxFinca = fincaCombo.currentIndex - 1
                        var data = {
                            "especie":       especieCombo.model[especieCombo.currentIndex],
                            "identificador": identificadorField.text,
                            "idFinca":       vista.fincasList[idxFinca].id,
                            "nacimiento":    vista.convertirFecha(nacimientoField.text),
                            "sexo":          sexoCombo.model[sexoCombo.currentIndex],
                            "estado":        estadoCombo.model[estadoCombo.currentIndex],
                            "raza":          razaField.text,
                            "padre":         padreSelectedId.text,
                            "madre":         madreSelectedId.text,
                            "chapeta":       chapetaField.text,
                            "fechaDestete":  vista.convertirFecha(desteteField.text),
                            "fechaUltimoParto":
                                vista.esHembra ? vista.convertirFecha(ultimoPartoField.text) : "",
                            "fechaUltimaPalpacion":
                                vista.esHembra ? vista.convertirFecha(ultimaPalpacionField.text) : "",
                            "fechaInseminacion":
                                vista.esHembra ? vista.convertirFecha(inseminacionField.text) : ""
                        }

                        if (appViewModel.createGanado(data)) {
                            vista.animalRegistrado()
                            vista.volver()
                        } else {
                            errorText.text = "No se pudo registrar el animal"
                        }
                    }
                }

                Item { height: 16 }
            }
        }
    }
}