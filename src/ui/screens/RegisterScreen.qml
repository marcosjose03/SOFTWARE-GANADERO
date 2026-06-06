import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    readonly property color bg:        "#1A2E1C"
    readonly property color cardBg:    "#243327"
    readonly property color accent:    "#4CAF50"
    readonly property color accentDim: "#2E7D32"
    readonly property color inputBg:   "#1F2E21"
    readonly property color border:    "#3A5540"
    readonly property color textPrim:  "#E8F5E9"
    readonly property color textSec:   "#A5C8A8"

    Connections {
        target: appViewModel
        function onErrorOccurred(message) { errorText.text = message }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0D1B0F" }
            GradientStop { position: 1.0; color: "#1A2E1C" }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: 380
        height: cardCol.implicitHeight + 56
        radius: 16
        color: cardBg
        border.color: border
        border.width: 1

        ColumnLayout {
            id: cardCol
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                topMargin: 36
                leftMargin: 36
                rightMargin: 36
            }
            spacing: 0

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "🐄"
                font.pixelSize: 44
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 8
                text: "Crear cuenta"
                font.pixelSize: 22
                font.bold: true
                color: textPrim
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 4
                Layout.bottomMargin: 28
                text: "Regístrate para comenzar"
                font.pixelSize: 12
                color: textSec
            }

            // ── Nombre ──
            Text { text: "Nombre completo"; font.pixelSize: 12; color: textSec; Layout.bottomMargin: 4 }
            Rectangle {
                Layout.fillWidth: true
                Layout.bottomMargin: 16
                height: 42
                radius: 8
                color: inputBg
                border.color: nombreField.activeFocus ? accent : border
                border.width: nombreField.activeFocus ? 2 : 1
                TextInput {
                    id: nombreField
                    anchors { fill: parent; leftMargin: 12; rightMargin: 12; verticalCenter: parent.verticalCenter }
                    verticalAlignment: TextInput.AlignVCenter
                    color: textPrim
                    font.pixelSize: 14
                    clip: true
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        text: "Tu nombre"
                        color: "#4A6B4C"
                        font.pixelSize: 14
                        visible: !nombreField.text && !nombreField.activeFocus
                    }
                }
            }

            // ── Correo ──
            Text { text: "Correo electrónico"; font.pixelSize: 12; color: textSec; Layout.bottomMargin: 4 }
            Rectangle {
                Layout.fillWidth: true
                Layout.bottomMargin: 16
                height: 42
                radius: 8
                color: inputBg
                border.color: emailField.activeFocus ? accent : border
                border.width: emailField.activeFocus ? 2 : 1
                TextInput {
                    id: emailField
                    anchors { fill: parent; leftMargin: 12; rightMargin: 12; verticalCenter: parent.verticalCenter }
                    verticalAlignment: TextInput.AlignVCenter
                    color: textPrim
                    font.pixelSize: 14
                    clip: true
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        text: "correo@ejemplo.com"
                        color: "#4A6B4C"
                        font.pixelSize: 14
                        visible: !emailField.text && !emailField.activeFocus
                    }
                }
            }

            // ── Contraseña ──
            Text { text: "Contraseña"; font.pixelSize: 12; color: textSec; Layout.bottomMargin: 4 }
            Rectangle {
                Layout.fillWidth: true
                Layout.bottomMargin: 8
                height: 42
                radius: 8
                color: inputBg
                border.color: passField.activeFocus ? accent : border
                border.width: passField.activeFocus ? 2 : 1
                TextInput {
                    id: passField
                    anchors { fill: parent; leftMargin: 12; rightMargin: 12; verticalCenter: parent.verticalCenter }
                    verticalAlignment: TextInput.AlignVCenter
                    echoMode: TextInput.Password
                    color: textPrim
                    font.pixelSize: 14
                    clip: true
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        text: "••••••••"
                        color: "#4A6B4C"
                        font.pixelSize: 14
                        visible: !passField.text && !passField.activeFocus
                    }
                }
            }

            Text {
                id: errorText
                color: "#EF5350"
                font.pixelSize: 12
                visible: text !== ""
                Layout.fillWidth: true
                Layout.bottomMargin: 8
                wrapMode: Text.WordWrap
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.topMargin: 8
                Layout.bottomMargin: 10
                height: 44
                radius: 8
                color: accent

                Text {
                    anchors.centerIn: parent
                    text: "Crear cuenta"
                    color: "white"
                    font.pixelSize: 15
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        errorText.text = ""
                        if (nombreField.text === "" || emailField.text === "" || passField.text === "") {
                            errorText.text = "Por favor completa todos los campos"
                            return
                        }
                        appViewModel.createAccount(nombreField.text, emailField.text, passField.text)
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.bottomMargin: 4
                height: 44
                radius: 8
                color: "transparent"
                border.color: border
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: "← Volver al login"
                    color: textSec
                    font.pixelSize: 14
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: appViewModel.goToLogin()
                }
            }
        }
    }
}
