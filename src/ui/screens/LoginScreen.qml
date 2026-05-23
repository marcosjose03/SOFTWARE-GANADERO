import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    anchors.fill: parent

    ColumnLayout {
        anchors.centerIn: parent
        width: 300
        spacing: 12

        Text {
            text: "Correo electrónico"
            Layout.fillWidth: true
        }
        TextField {
            id: emailField
            Layout.fillWidth: true
            placeholderText: "correo@ejemplo.com"
        }

        Text {
            text: "Contraseña"
            Layout.fillWidth: true
        }
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