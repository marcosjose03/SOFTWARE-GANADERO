import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import SoftwareGanadero 1.0

Item {
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    Connections {
        target: appViewModel
        function onErrorOccurred(message) { errorText.text = message }
    }

    Rectangle { anchors.fill: parent; color: Theme.fondo }

    Tarjeta {
        anchors.centerIn: parent
        width:            Theme.anchoFormulario
        height:           colReg.implicitHeight + Theme.espacioXl * 2

        ColumnLayout {
            id: colReg
            anchors {
                left:    parent.left
                right:   parent.right
                top:     parent.top
                margins: Theme.espacioXl
            }
            spacing: Theme.espacioMd

            // Logo + título
            Row {
                Layout.fillWidth: true
                spacing: Theme.espacioSm

                Text {
                    text:           "🐄"
                    font.pixelSize: 28
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text:         "GanSoft"
                    font.family:  Theme.fuente
                    font.pixelSize: Theme.tamSubtitulo
                    font.weight:    Font.Bold
                    color:          Theme.primario
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Text {
                text:             "Crear cuenta"
                font.family:      Theme.fuente
                font.pixelSize:   Theme.tamTitulo
                font.weight:      Font.Bold
                color:            Theme.textoPrimario
                Layout.fillWidth: true
            }

            Text {
                text:             "Completa los datos para registrarte"
                font.family:      Theme.fuente
                font.pixelSize:   Theme.tamEtiqueta
                color:            Theme.textoSecundario
                Layout.fillWidth: true
            }

            Item { Layout.preferredHeight: Theme.espacioSm }

            // Nombre
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs

                Text {
                    text:           "Nombre completo *"
                    font.family:    Theme.fuente
                    font.pixelSize: Theme.tamEtiqueta
                    color:          Theme.textoSecundario
                }
                CampoTexto {
                    id:              nombreField
                    width:           parent.width
                    placeholderText: "Tu nombre"
                    KeyNavigation.tab: emailField
                }
            }

            // Correo
            Column {
                Layout.fillWidth: true
                spacing:          Theme.espacioXs

                Text {
                    text:           "Correo electrónico *"
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
                    text:           "Contraseña *"
                    font.family:    Theme.fuente
                    font.pixelSize: Theme.tamEtiqueta
                    color:          Theme.textoSecundario
                }
                CampoTexto {
                    id:              passField
                    width:           parent.width
                    echoMode:        TextInput.Password
                    placeholderText: "Mínimo 6 caracteres"
                    Keys.onReturnPressed: btnCrear.clicked()
                }
            }

            // Error
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
                id:               btnCrear
                text:             "Crear cuenta"
                Layout.fillWidth: true
                onClicked: {
                    errorText.text = ""
                    if (nombreField.text === "" || emailField.text === "" || passField.text === "") {
                        errorText.text = "Por favor completa todos los campos"
                        return
                    }
                    appViewModel.createAccount(nombreField.text, emailField.text, passField.text)
                }
            }

            BotonSecundario {
                text:             "← Volver al login"
                Layout.fillWidth: true
                onClicked:        appViewModel.goToLogin()
            }
        }
    }
}
