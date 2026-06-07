import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    Connections {
        target: appViewModel
        function onErrorOccurred(message) {
            errorText.text = message
        }
    }

    Rectangle { anchors.fill: parent; color: "white" }

    Rectangle {
        anchors.centerIn: parent
        width: 320
        height: loginCol.implicitHeight + 48
        color: "white"
        border.color: "#e0e0e0"
        border.width: 1
        radius: 8

        ColumnLayout {
            id: loginCol
            anchors { fill: parent; margins: 24 }
            spacing: 10

            Text {
                text: "GanSoft"
                font.pixelSize: 24
                font.bold: true
                color: "#1a1a1a"
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "Iniciar sesión"
                font.pixelSize: 13
                color: "#666666"
                Layout.alignment: Qt.AlignHCenter
                bottomPadding: 8
            }

            Text { text: "Correo electrónico"; color: "#333333"; font.pixelSize: 13; Layout.fillWidth: true }
            TextField {
                id: emailField
                Layout.fillWidth: true
                placeholderText: "correo@ejemplo.com"
            }

            Text { text: "Contraseña"; color: "#333333"; font.pixelSize: 13; Layout.fillWidth: true }
            TextField {
                id: passField
                Layout.fillWidth: true
                echoMode: TextInput.Password
                placeholderText: "••••••••"
            }

            Text {
                id: errorText
                color: "#cc0000"
                visible: text !== ""
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                font.pixelSize: 13
            }

            Button {
                text: "Ingresar"
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

            Button {
                text: "Crear cuenta"
                Layout.fillWidth: true
                onClicked: appViewModel.goToRegister()
            }
        }
    }
}
