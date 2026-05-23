import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 768
    title: "Software Ganadero"

    // Cambio de pantalla reactivo
    Connections {
        target: appViewModel
        function onCurrentScreenChanged() {
            switch (appViewModel.currentScreen) {
                case "login":    stackView.replace("qrc:/SoftwareGanadero/ui/screens/LoginScreen.qml"); break
                case "register": stackView.replace("qrc:/SoftwareGanadero/ui/screens/RegisterScreen.qml"); break
                case "menu":     stackView.replace("qrc:/SoftwareGanadero/ui/screens/MenuScreen.qml"); break
                case "inventario":   stackView.replace("qrc:/SoftwareGanadero/ui/screens/InventarioScreen.qml"); break
                case "reproductivo": stackView.replace("qrc:/SoftwareGanadero/ui/screens/RegistroReproductivoScreen.qml"); break
                case "leche":    stackView.replace("qrc:/SoftwareGanadero/ui/screens/RegistroLecheScreen.qml"); break
                case "peso":     stackView.replace("qrc:/SoftwareGanadero/ui/screens/RegistroPesoScreen.qml"); break
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: appViewModel.isLoggedIn
            ? "qrc:/SoftwareGanadero/ui/screens/MenuScreen.qml"
            : "qrc:/SoftwareGanadero/ui/screens/LoginScreen.qml"
    }
}