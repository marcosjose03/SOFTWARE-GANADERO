import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    anchors.fill: parent

    // Capturar errores del ViewModel
    Connections {
        target: appViewModel
        function onErrorOccurred(message) {
            errorText.text = message
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        width: 300
        spacing: 12

        Text { text: "Nombre"; Layout.fillWidth: true }
        TextField {
            id: nombreField
            Layout.fillWidth: true
            placeholderText: "Tu nombre"
        }

        Text { text: "Correo electrónico"; Layout.fillWidth: true }
        TextField {
            id: emailField
            Layout.fillWidth: true
            placeholderText: "correo@ejemplo.com"
        }

        Text { text: "Contraseña"; Layout.fillWidth: true }
        TextField {
            id: passField
            Layout.fillWidth: true
            echoMode: TextInput.Password
            placeholderText: "••••••••"
        }

        Text {
            id: errorText
            color: "red"
            visible: text !== ""
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        Button {
            text: "Crear cuenta"
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

        Button {
            text: "← Volver al login"
            Layout.fillWidth: true
            onClicked: appViewModel.goToLogin()
        }
    }
}