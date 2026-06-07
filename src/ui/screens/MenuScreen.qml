import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width: parent ? parent.width : 1024
    height: parent ? parent.height : 768

    Rectangle { anchors.fill: parent; color: "white" }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ── Barra superior ────────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            height: 56
            color: "#fafafa"

            Rectangle {
                anchors.left: parent.left; anchors.right: parent.right
                anchors.bottom: parent.bottom; height: 1; color: "#e0e0e0"
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 24
                anchors.rightMargin: 24
                spacing: 12

                Text {
                    text: "GanSoft"
                    font.pixelSize: 18
                    font.bold: true
                    color: "#1a1a1a"
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: "Bienvenido, " + appViewModel.userName
                    font.pixelSize: 13
                    color: "#555555"
                }

                Button {
                    text: "Cerrar Sesión"
                    onClicked: appViewModel.logout()
                }

                Button {
                    text: "Salir"
                    onClicked: Qt.quit()
                }
            }
        }

        // ── Área de contenido ─────────────────────────────────────────────
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.centerIn: parent
                width: Math.min(560, parent.width - 48)
                spacing: 6

                Text {
                    text: "Gestión del hato"
                    font.pixelSize: 12
                    color: "#888888"
                    Layout.fillWidth: true
                    bottomPadding: 4
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 10
                    rowSpacing: 10

                    Button {
                        Layout.fillWidth: true
                        implicitHeight: 52
                        text: "Inventario"
                        onClicked: appViewModel.goToInventario()
                    }
                    Button {
                        Layout.fillWidth: true
                        implicitHeight: 52
                        text: "Registro Reproductivo"
                        onClicked: appViewModel.goToRegistroReproductivo()
                    }
                    Button {
                        Layout.fillWidth: true
                        implicitHeight: 52
                        text: "Registro de Leche"
                        onClicked: appViewModel.goToRegistroLeche()
                    }
                    Button {
                        Layout.fillWidth: true
                        implicitHeight: 52
                        text: "Registro de Peso"
                        onClicked: appViewModel.goToRegistroPeso()
                    }
                }

                Item { height: 16 }

                Text {
                    text: "Análisis y proyecciones"
                    font.pixelSize: 12
                    color: "#888888"
                    Layout.fillWidth: true
                    bottomPadding: 4
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 3
                    columnSpacing: 10
                    rowSpacing: 10

                    Button { Layout.fillWidth: true; implicitHeight: 52; text: "Herramienta visual"; enabled: false }
                    Button { Layout.fillWidth: true; implicitHeight: 52; text: "Alertas";            enabled: false }
                    Button { Layout.fillWidth: true; implicitHeight: 52; text: "Proyecciones";       enabled: false }
                }
            }
        }
    }
}
