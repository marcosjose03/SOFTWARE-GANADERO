import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    Connections {
        target: appViewModel
        function onErrorOccurred(message) { errorText.text = message }
    }

    // Fondo general
    Rectangle { anchors.fill: parent; color: Theme.fondo }

    // Panel verde izquierdo (decorativo)
    Rectangle {
        anchors.left:   parent.left
        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        width:          Math.max(parent.width * 0.33, 240)
        color:          Theme.primario

        Column {
            anchors.centerIn: parent
            spacing:          Theme.espacioLg

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text:           "🐄"
                font.pixelSize: 64
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text:        "GanSoft"
                font.family: Theme.fuente
                font.pixelSize: 38
                font.weight:    Font.Bold
                color:          "white"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text:                "Gestión ganadera\ninteligente"
                font.family:         Theme.fuente
                font.pixelSize:      Theme.tamCuerpo
                color:               Qt.rgba(1, 1, 1, 0.75)
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    // Tarjeta de login (derecha / centro)
    Tarjeta {
        id: cardLogin
        anchors.right:          parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin:    parent.width * 0.07
        width:                  Math.min(Theme.anchoFormulario, parent.width * 0.54)
        height:                 colLogin.implicitHeight + Theme.espacioXl * 2

        ColumnLayout {
            id: colLogin
            anchors {
                left:    parent.left
                right:   parent.right
                top:     parent.top
                margins: Theme.espacioXl
            }
            spacing: Theme.espacioMd

            // Título
            Text {
                text:             "Bienvenido de nuevo"
                font.family:      Theme.fuente
                font.pixelSize:   Theme.tamTitulo
                font.weight:      Font.Bold
                color:            Theme.textoPrimario
                Layout.fillWidth: true
            }

            Text {
                text:             "Ingresa tus credenciales para continuar"
                font.family:      Theme.fuente
                font.pixelSize:   Theme.tamEtiqueta
                color:            Theme.textoSecundario
                Layout.fillWidth: true
            }

            Item { Layout.preferredHeight: Theme.espacioSm }

            // Correo
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs

                Text {
                    text:           "Correo electrónico"
                    font.family:    Theme.fuente
                    font.pixelSize: Theme.tamEtiqueta
                    color:          Theme.textoSecundario
                }
                CampoTexto {
                    id:              emailField
                    width:           parent.width
                    placeholderText: "correo@ejemplo.com"
                    KeyNavigation.tab: passField
                }
            }

            // Contraseña
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs

                Text {
                    text:           "Contraseña"
                    font.family:    Theme.fuente
                    font.pixelSize: Theme.tamEtiqueta
                    color:          Theme.textoSecundario
                }
                CampoTexto {
                    id:              passField
                    width:           parent.width
                    echoMode:        TextInput.Password
                    placeholderText: "••••••••"
                    Keys.onReturnPressed: btnIngresar.clicked()
                }
            }

            // Mensaje de error
            Rectangle {
                Layout.fillWidth: true
                height:           errorText.implicitHeight + Theme.espacioSm * 2
                radius:           Theme.radioSm
                color:            Theme.criticoFondo
                border.color:     Theme.critico
                border.width:     1
                visible:          errorText.text !== ""

                Text {
                    id:               errorText
                    anchors.centerIn: parent
                    width:            parent.width - Theme.espacioMd * 2
                    color:            Theme.critico
                    font.family:      Theme.fuente
                    font.pixelSize:   Theme.tamEtiqueta
                    wrapMode:         Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Item { Layout.preferredHeight: Theme.espacioXs }

            BotonPrimario {
                id:               btnIngresar
                text:             "Ingresar"
                Layout.fillWidth: true
                onClicked: {
                    errorText.text = ""
                    if (emailField.text === "" || passField.text === "") {
                        errorText.text = "Por favor completa todos los campos"
                        return
                    }
                    if (!appViewModel.login(emailField.text, passField.text))
                        errorText.text = "Correo o contraseña incorrectos"
                }
            }

            BotonSecundario {
                text:             "Crear cuenta"
                Layout.fillWidth: true
                onClicked:        appViewModel.goToRegister()
            }
        }
    }
}
