import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    // ── Paleta ──────────────────────────────────────────────
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

    // ── Fondo con gradiente ──────────────────────────────────
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0D1B0F" }
            GradientStop { position: 1.0; color: "#1A2E1C" }
        }
    }

    // ── Card central ────────────────────────────────────────
    Rectangle {
        anchors.centerIn: parent
        width: 380
        height: cardCol.implicitHeight + 56
        radius: 16
        color: cardBg
        border.color: border
        border.width: 1

        layer.enabled: true
        layer.effect: null   // sombra sin QtGraphicalEffects

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

            // Ícono
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "🐄"
                font.pixelSize: 52
            }

            // Título
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 8
                text: "Software Ganadero"
                font.pixelSize: 22
                font.bold: true
                color: textPrim
            }

            // Subtítulo
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 4
                Layout.bottomMargin: 28
                text: "Gestión integral de hato bovino"
                font.pixelSize: 12
                color: textSec
            }

            // ── Correo ──
            Text {
                text: "Correo electrónico"
                font.pixelSize: 12
                color: textSec
                Layout.bottomMargin: 4
            }
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
            Text {
                text: "Contraseña"
                font.pixelSize: 12
                color: textSec
                Layout.bottomMargin: 4
            }
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

            // Error
            Text {
                id: errorText
                color: "#EF5350"
                font.pixelSize: 12
                visible: text !== ""
                Layout.fillWidth: true
                Layout.bottomMargin: 8
                wrapMode: Text.WordWrap
            }

            // ── Botón Ingresar ──
            Rectangle {
                Layout.fillWidth: true
                Layout.topMargin: 8
                Layout.bottomMargin: 10
                height: 44
                radius: 8
                color: loginMouse.containsMousee ? accentDim : accent
                Behavior on color { ColorAnimation { duration: 120 } }

                Text {
                    anchors.centerIn: parent
                    text: "Ingresar"
                    color: "white"
                    font.pixelSize: 15
                    font.bold: true
                }

                MouseArea {
                    id: loginMouse
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
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
            }

            // ── Botón Crear cuenta ──
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
                    text: "Crear cuenta"
                    color: textSec
                    font.pixelSize: 14
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: appViewModel.goToRegister()
                }
            }
        }
    }
}
