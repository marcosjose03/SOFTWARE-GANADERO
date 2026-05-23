import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    anchors.fill: parent

    ColumnLayout {
        anchors.centerIn: parent
        width: 300
        spacing: 8

        Text {
            text: "Bienvenido " + appViewModel.userName
            font.pixelSize: 24
            font.bold: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            text: "Herramienta visual"
            Layout.fillWidth: true
            enabled: false
        }
        Button {
            text: "Alertas"
            Layout.fillWidth: true
            enabled: false
        }
        Button {
            text: "Proyecciones"
            Layout.fillWidth: true
            enabled: false
        }
        Button {
            text: "Inventario"
            Layout.fillWidth: true
            onClicked: appViewModel.goToInventario()
        }
        Button {
            text: "Registro Reproductivo"
            Layout.fillWidth: true
            onClicked: appViewModel.goToRegistroReproductivo()
        }
        Button {
            text: "Registro de Leche"
            Layout.fillWidth: true
            onClicked: appViewModel.goToRegistroLeche()
        }
        Button {
            text: "Registro de Peso"
            Layout.fillWidth: true
            onClicked: appViewModel.goToRegistroPeso()
        }
        Button {
            text: "Cerrar Sesión"
            Layout.fillWidth: true
            onClicked: appViewModel.logout()
        }
        Button {
            text: "Salir"
            Layout.fillWidth: true
            onClicked: Qt.quit()
        }
    }
}