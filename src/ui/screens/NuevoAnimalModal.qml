import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1 as Platform

Item {
    id: vista
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    Connections {
        target: appViewModel
        function onErrorOccurred(message) { errorText.text = message }
    }

    signal animalRegistrado()
    signal volver()

    property var  fincasList:          []
    property var  razasList:           []
    property var  todosAnimales:       []
    property bool esHembra:            false
    property bool especieSeleccionada: false
    property string fotoBase64:        ""

    function cargar() {
        fincasList    = appViewModel.getFincas()
        todosAnimales = appViewModel.getAllGanado()
        limpiar()
    }

    function limpiar() {
        especieCombo.currentIndex        = 0
        identificadorField.text          = ""
        fincaCombo.currentIndex          = 0
        nacimientoField.text             = ""
        sexoCombo.currentIndex           = 0
        estadoCombo.currentIndex         = 0
        razaField.text                   = ""
        razaDropdown.visible             = false
        padreSearch.text                 = ""
        padreSelectedId.text             = ""
        madreSearch.text                 = ""
        madreSelectedId.text             = ""
        chapetaField.text                = ""
        desteteField.text                = ""
        ultimoPartoField.text            = ""
        ultimaPalpacionField.text        = ""
        inseminacionField.text           = ""
        errorText.text                   = ""
        esHembra                         = false
        especieSeleccionada              = false
        razasList                        = []
        fotoBase64                       = ""
        fotoPreview.source               = ""
        calNac.visible                   = false
        calDes.visible                   = false
        calPar.visible                   = false
        calPal.visible                   = false
        calIns.visible                   = false
        padreDropdown.visible            = false
        madreDropdown.visible            = false
    }

    function convertirFecha(f) {
        if (!f || f === "") return ""
        var p = f.split("/")
        if (p.length !== 3) return ""
        return p[2] + "-" + p[1] + "-" + p[0]
    }

    Component.onCompleted: cargar()

    Platform.FileDialog {
        id: fotoDialog
        title: "Seleccionar imagen"
        nameFilters: ["Imágenes (*.png *.jpg *.jpeg *.bmp)"]
        onAccepted: {
            var path = fotoDialog.file.toString()
            var b64  = appViewModel.leerArchivoBase64(path)
            if (b64 !== "") {
                vista.fotoBase64   = b64
                fotoPreview.source = "data:image/png;base64," + b64
            }
        }
    }

    Rectangle { anchors.fill: parent; color: Theme.fondo }

    ColumnLayout {
        anchors.fill: parent
        spacing:      0

        // Encabezado
        EncabezadoPantalla {
            titulo:           "Nuevo Animal"
            Layout.fillWidth: true
            onVolverClicked:  vista.volver()
        }

        // Formulario scrolleable
        Flickable {
            id: formFlick
            Layout.fillWidth:  true
            Layout.fillHeight: true
            clip:              true
            contentWidth:      width
            contentHeight:     formColumn.implicitHeight + Theme.espacioXl
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            Column {
                id: formColumn
                width:   Math.min(Theme.anchoFormulario, formFlick.width - Theme.espacioLg * 2)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: Theme.espacioLg
                spacing: Theme.espacioMd

                // ── SECCIÓN: Identificación ────────────────────────────────
                SectionHeader { titulo: "Identificación" }

                // Especie
                FieldLabel { texto: "Especie *" }
                SelectorDesplegable {
                    id: especieCombo
                    width: parent.width
                    model: ["Seleccionar especie...", "Bovino", "Caprino", "Bufalino"]
                    currentIndex: 0
                    onCurrentIndexChanged: {
                        if (currentIndex > 0) {
                            vista.especieSeleccionada = true
                            vista.razasList = appViewModel.getRazasPorEspecie(model[currentIndex])
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

                // Identificador
                FieldLabel { texto: "Identificador *"; deshabilitado: !vista.especieSeleccionada }
                CampoTexto {
                    id: identificadorField; width: parent.width
                    placeholderText: "Ej: ab1234"
                    enabled: vista.especieSeleccionada
                }

                // Finca
                FieldLabel { texto: "Finca *"; deshabilitado: !vista.especieSeleccionada }
                SelectorDesplegable {
                    id: fincaCombo; width: parent.width
                    enabled: vista.especieSeleccionada
                    model: {
                        var nombres = ["Seleccionar finca..."]
                        for (var i = 0; i < vista.fincasList.length; i++)
                            nombres.push(vista.fincasList[i].nombre)
                        return nombres
                    }
                    currentIndex: 0
                }

                // Sexo
                FieldLabel { texto: "Sexo *"; deshabilitado: !vista.especieSeleccionada }
                SelectorDesplegable {
                    id: sexoCombo; width: parent.width
                    model: ["Seleccionar sexo...", "Macho", "Hembra"]
                    currentIndex: 0; enabled: vista.especieSeleccionada
                    onCurrentIndexChanged: vista.esHembra = (currentIndex === 2)
                }

                // Estado
                FieldLabel { texto: "Estado *"; deshabilitado: !vista.especieSeleccionada }
                SelectorDesplegable {
                    id: estadoCombo; width: parent.width
                    model: ["Seleccionar estado...", "Activo", "Muerto", "Vendido", "Enfermo"]
                    currentIndex: 0; enabled: vista.especieSeleccionada
                }

                // ── SECCIÓN: Características ───────────────────────────────
                SectionHeader { titulo: "Características" }

                // Nacimiento
                FieldLabel { texto: "Fecha de nacimiento"; deshabilitado: !vista.especieSeleccionada }
                Row {
                    width: parent.width; spacing: Theme.espacioSm
                    CampoTexto {
                        id: nacimientoField
                        width: parent.width - 52
                        placeholderText: "dd/mm/yyyy"
                        readOnly: true; enabled: vista.especieSeleccionada
                    }
                    AbstractButton {
                        width: 48; height: Theme.alturaCampo
                        enabled: vista.especieSeleccionada
                        onClicked: {
                            calDes.visible = false; calPar.visible = false
                            calPal.visible = false; calIns.visible = false
                            calNac.visible = !calNac.visible
                        }
                        background: Rectangle {
                            radius: Theme.radioSm
                            color: parent.pressed ? Theme.primarioHover
                                 : parent.hovered ? Theme.primarioClaro
                                                  : Theme.superficie
                            border.color: Theme.borde; border.width: 1
                        }
                        contentItem: Text {
                            text: "📅"; font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment:   Text.AlignVCenter
                        }
                    }
                }
                CalendarPopup {
                    id: calNac; width: parent.width; visible: false
                    onFechaSeleccionada: function(f) { nacimientoField.text = f; calNac.visible = false }
                }

                // Raza (con autocomplete)
                FieldLabel { texto: "Raza"; deshabilitado: !vista.especieSeleccionada }
                CampoTexto {
                    id: razaField; width: parent.width
                    placeholderText: "Buscar raza..."
                    enabled: vista.especieSeleccionada
                    onTextChanged: {
                        if (!vista.especieSeleccionada || vista.razasList.length === 0) {
                            razaDropdown.visible = false; return
                        }
                        var busq     = text.toLowerCase()
                        var filtradas = []
                        for (var i = 0; i < vista.razasList.length; i++)
                            if (vista.razasList[i].toLowerCase().indexOf(busq) >= 0)
                                filtradas.push(vista.razasList[i])
                        var esExacta = false
                        for (var j = 0; j < vista.razasList.length; j++)
                            if (vista.razasList[j].toLowerCase() === busq) { esExacta = true; break }
                        razaListView.model   = filtradas
                        razaDropdown.visible = filtradas.length > 0 && !esExacta
                    }
                    onActiveFocusChanged: {
                        if (activeFocus && vista.especieSeleccionada && vista.razasList.length > 0) {
                            razaListView.model   = vista.razasList
                            razaDropdown.visible = true
                        }
                    }
                }
                Rectangle {
                    id: razaDropdown; width: parent.width; height: 160
                    border.color: Theme.borde; radius: Theme.radioSm
                    clip: true; visible: false; color: Theme.superficie
                    ListView {
                        id: razaListView; anchors.fill: parent; clip: true; model: []
                        delegate: ItemDelegate {
                            width: razaListView.width
                            text: modelData
                            font.family: Theme.fuente; font.pixelSize: Theme.tamCuerpo
                            onClicked: { razaField.text = modelData; razaDropdown.visible = false }
                        }
                    }
                }

                // Chapeta
                FieldLabel { texto: "Chapeta"; deshabilitado: !vista.especieSeleccionada }
                CampoTexto {
                    id: chapetaField; width: parent.width
                    placeholderText: "Número de chapeta (opcional)"
                    enabled: vista.especieSeleccionada
                }

                // ── SECCIÓN: Genealogía ────────────────────────────────────
                SectionHeader { titulo: "Genealogía" }

                // Padre
                FieldLabel { texto: "Padre"; deshabilitado: !vista.especieSeleccionada }
                CampoTexto {
                    id: padreSearch; width: parent.width
                    placeholderText: "Buscar por identificador..."
                    enabled: vista.especieSeleccionada
                    onTextChanged: {
                        var busq    = text.toLowerCase()
                        var especie = especieCombo.currentIndex > 0
                            ? especieCombo.model[especieCombo.currentIndex] : ""
                        var filtrados = []
                        for (var i = 0; i < vista.todosAnimales.length; i++) {
                            var a = vista.todosAnimales[i]
                            if (a.sexo === "Macho" && a.especie === especie &&
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
                                if (a.sexo === "Macho" && a.especie === especie) machos.push(a)
                            }
                            padreListView.model   = machos
                            padreDropdown.visible = machos.length > 0
                        }
                    }
                }
                Rectangle {
                    id: padreDropdown; width: parent.width; height: 130
                    border.color: Theme.borde; radius: Theme.radioSm
                    clip: true; visible: false; color: Theme.superficie
                    ListView {
                        id: padreListView; anchors.fill: parent; clip: true; model: []
                        delegate: ItemDelegate {
                            width: padreListView.width
                            text: String(modelData.identificador) + " — " + modelData.especie
                            font.family: Theme.fuente; font.pixelSize: Theme.tamCuerpo
                            onClicked: {
                                padreSelectedId.text  = modelData.id
                                padreSearch.text      = String(modelData.identificador)
                                padreDropdown.visible = false
                            }
                        }
                    }
                }
                Text {
                    id: padreSelectedId; text: ""; color: "#555"
                    visible: false; width: parent.width
                }

                // Madre
                FieldLabel { texto: "Madre"; deshabilitado: !vista.especieSeleccionada }
                CampoTexto {
                    id: madreSearch; width: parent.width
                    placeholderText: "Buscar por identificador..."
                    enabled: vista.especieSeleccionada
                    onTextChanged: {
                        var busq    = text.toLowerCase()
                        var especie = especieCombo.currentIndex > 0
                            ? especieCombo.model[especieCombo.currentIndex] : ""
                        var filtradas = []
                        for (var i = 0; i < vista.todosAnimales.length; i++) {
                            var a = vista.todosAnimales[i]
                            if (a.sexo === "Hembra" && a.especie === especie &&
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
                                if (a.sexo === "Hembra" && a.especie === especie) hembras.push(a)
                            }
                            madreListView.model   = hembras
                            madreDropdown.visible = hembras.length > 0
                        }
                    }
                }
                Rectangle {
                    id: madreDropdown; width: parent.width; height: 130
                    border.color: Theme.borde; radius: Theme.radioSm
                    clip: true; visible: false; color: Theme.superficie
                    ListView {
                        id: madreListView; anchors.fill: parent; clip: true; model: []
                        delegate: ItemDelegate {
                            width: madreListView.width
                            text: String(modelData.identificador) + " — " + modelData.especie
                            font.family: Theme.fuente; font.pixelSize: Theme.tamCuerpo
                            onClicked: {
                                madreSelectedId.text  = modelData.id
                                madreSearch.text      = String(modelData.identificador)
                                madreDropdown.visible = false
                            }
                        }
                    }
                }
                Text {
                    id: madreSelectedId; text: ""; color: "#555"
                    visible: false; width: parent.width
                }

                // ── SECCIÓN: Otras fechas ──────────────────────────────────
                SectionHeader { titulo: "Otras fechas" }

                // Fecha Destete
                FieldLabel { texto: "Fecha destete"; deshabilitado: !vista.especieSeleccionada }
                Row {
                    width: parent.width; spacing: Theme.espacioSm
                    CampoTexto {
                        id: desteteField; width: parent.width - 52
                        placeholderText: "dd/mm/yyyy"; readOnly: true
                        enabled: vista.especieSeleccionada
                    }
                    AbstractButton {
                        width: 48; height: Theme.alturaCampo
                        enabled: vista.especieSeleccionada
                        onClicked: {
                            calNac.visible = false; calPar.visible = false
                            calPal.visible = false; calIns.visible = false
                            calDes.visible = !calDes.visible
                        }
                        background: Rectangle {
                            radius: Theme.radioSm; color: Theme.superficie
                            border.color: Theme.borde; border.width: 1
                        }
                        contentItem: Text {
                            text: "📅"; font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment:   Text.AlignVCenter
                        }
                    }
                }
                CalendarPopup {
                    id: calDes; width: parent.width; visible: false
                    onFechaSeleccionada: function(f) { desteteField.text = f; calDes.visible = false }
                }

                // Campos solo para hembra
                FieldLabel { texto: "Fecha último parto"; visible: vista.esHembra }
                Row {
                    width: parent.width; spacing: Theme.espacioSm
                    visible: vista.esHembra
                    CampoTexto {
                        id: ultimoPartoField; width: parent.width - 52
                        placeholderText: "dd/mm/yyyy"; readOnly: true
                    }
                    AbstractButton {
                        width: 48; height: Theme.alturaCampo
                        onClicked: {
                            calNac.visible = false; calDes.visible = false
                            calPal.visible = false; calIns.visible = false
                            calPar.visible = !calPar.visible
                        }
                        background: Rectangle {
                            radius: Theme.radioSm; color: Theme.superficie
                            border.color: Theme.borde; border.width: 1
                        }
                        contentItem: Text {
                            text: "📅"; font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment:   Text.AlignVCenter
                        }
                    }
                }
                CalendarPopup {
                    id: calPar; width: parent.width; visible: false
                    onFechaSeleccionada: function(f) { ultimoPartoField.text = f; calPar.visible = false }
                }

                FieldLabel { texto: "Fecha última palpación"; visible: vista.esHembra }
                Row {
                    width: parent.width; spacing: Theme.espacioSm
                    visible: vista.esHembra
                    CampoTexto {
                        id: ultimaPalpacionField; width: parent.width - 52
                        placeholderText: "dd/mm/yyyy"; readOnly: true
                    }
                    AbstractButton {
                        width: 48; height: Theme.alturaCampo
                        onClicked: {
                            calNac.visible = false; calDes.visible = false
                            calPar.visible = false; calIns.visible = false
                            calPal.visible = !calPal.visible
                        }
                        background: Rectangle {
                            radius: Theme.radioSm; color: Theme.superficie
                            border.color: Theme.borde; border.width: 1
                        }
                        contentItem: Text {
                            text: "📅"; font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment:   Text.AlignVCenter
                        }
                    }
                }
                CalendarPopup {
                    id: calPal; width: parent.width; visible: false
                    onFechaSeleccionada: function(f) { ultimaPalpacionField.text = f; calPal.visible = false }
                }

                FieldLabel { texto: "Fecha inseminación"; visible: vista.esHembra }
                Row {
                    width: parent.width; spacing: Theme.espacioSm
                    visible: vista.esHembra
                    CampoTexto {
                        id: inseminacionField; width: parent.width - 52
                        placeholderText: "dd/mm/yyyy"; readOnly: true
                    }
                    AbstractButton {
                        width: 48; height: Theme.alturaCampo
                        onClicked: {
                            calNac.visible = false; calDes.visible = false
                            calPar.visible = false; calPal.visible = false
                            calIns.visible = !calIns.visible
                        }
                        background: Rectangle {
                            radius: Theme.radioSm; color: Theme.superficie
                            border.color: Theme.borde; border.width: 1
                        }
                        contentItem: Text {
                            text: "📅"; font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment:   Text.AlignVCenter
                        }
                    }
                }
                CalendarPopup {
                    id: calIns; width: parent.width; visible: false
                    onFechaSeleccionada: function(f) { inseminacionField.text = f; calIns.visible = false }
                }

                // ── SECCIÓN: Foto ──────────────────────────────────────────
                SectionHeader { titulo: "Foto" }

                Row {
                    width: parent.width; spacing: Theme.espacioMd
                    opacity: vista.especieSeleccionada ? 1 : 0.4

                    Rectangle {
                        width: 110; height: 110
                        radius: Theme.radioSm
                        color: Theme.superficieAlt
                        border.color: Theme.borde; border.width: 1

                        Image {
                            id: fotoPreview; anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                            source: ""
                            visible: source !== ""
                        }

                        Column {
                            anchors.centerIn: parent; spacing: Theme.espacioXs
                            visible: fotoPreview.source === ""
                            Text { anchors.horizontalCenter: parent.horizontalCenter; text: "📷"; font.pixelSize: 28 }
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "Sin foto"
                                font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                                color: Theme.textoSecundario
                            }
                        }
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: Theme.espacioSm
                        BotonSecundario {
                            text: "Seleccionar foto"
                            width: 160
                            enabled: vista.especieSeleccionada
                            onClicked: fotoDialog.open()
                        }
                        Text {
                            text: "PNG, JPG, JPEG, BMP"
                            font.family: Theme.fuente; font.pixelSize: Theme.tamMicro
                            color: Theme.textoSecundario
                        }
                    }
                }

                Item { height: Theme.espacioMd }

                // ── Error ──────────────────────────────────────────────────
                Rectangle {
                    width: parent.width
                    height: errorText.implicitHeight + Theme.espacioSm * 2
                    radius: Theme.radioSm; color: Theme.criticoFondo
                    border.color: Theme.critico; border.width: 1
                    visible: errorText.text !== ""

                    Text {
                        id: errorText
                        anchors.centerIn: parent
                        width: parent.width - Theme.espacioMd
                        color: Theme.critico
                        font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
                        wrapMode: Text.WordWrap; horizontalAlignment: Text.AlignHCenter
                    }
                }

                // ── Botón registrar ────────────────────────────────────────
                BotonPrimario {
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
                            "especie":              especieCombo.model[especieCombo.currentIndex],
                            "identificador":        identificadorField.text,
                            "idFinca":              vista.fincasList[idxFinca].id,
                            "nacimiento":           vista.convertirFecha(nacimientoField.text),
                            "sexo":                 sexoCombo.model[sexoCombo.currentIndex],
                            "estado":               estadoCombo.model[estadoCombo.currentIndex],
                            "raza":                 razaField.text,
                            "padre":                padreSelectedId.text,
                            "madre":                madreSelectedId.text,
                            "chapeta":              chapetaField.text,
                            "fechaDestete":         vista.convertirFecha(desteteField.text),
                            "fechaUltimoParto":
                                vista.esHembra ? vista.convertirFecha(ultimoPartoField.text) : "",
                            "fechaUltimaPalpacion":
                                vista.esHembra ? vista.convertirFecha(ultimaPalpacionField.text) : "",
                            "fechaInseminacion":
                                vista.esHembra ? vista.convertirFecha(inseminacionField.text) : ""
                        }
                        if (appViewModel.createGanado(data)) {
                            if (vista.fotoBase64 !== "") {
                                var todos = appViewModel.getAllGanado()
                                for (var i = 0; i < todos.length; i++) {
                                    if (todos[i].identificador === identificadorField.text) {
                                        appViewModel.updateFoto(todos[i].id, vista.fotoBase64)
                                        break
                                    }
                                }
                            }
                            vista.animalRegistrado()
                            vista.volver()
                        }
                    }
                }

                BotonSecundario {
                    width: parent.width
                    text:  "Cancelar"
                    onClicked: vista.volver()
                }

                Item { height: Theme.espacioXl }
            }
        }
    }

    // Componentes inline para el formulario
    component SectionHeader: Rectangle {
        property string titulo: ""
        width: parent ? parent.width : 300
        height: secText.implicitHeight + Theme.espacioMd
        color: Theme.superficieAlt
        radius: Theme.radioSm

        Text {
            id: secText
            anchors { left: parent.left; verticalCenter: parent.verticalCenter; leftMargin: Theme.espacioSm }
            text:        titulo
            font.family: Theme.fuente; font.pixelSize: Theme.tamEtiqueta
            font.weight: Font.DemiBold; color: Theme.primario
        }
    }

    component FieldLabel: Text {
        property string texto:         ""
        property bool   deshabilitado: false
        text:           texto
        font.family:    Theme.fuente; font.pixelSize: Theme.tamEtiqueta
        color:          deshabilitado ? Theme.deshabilitado : Theme.textoSecundario
        width:          parent ? parent.width : 300
    }
}
