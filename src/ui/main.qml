import QtQuick 2.15
import QtQuick.Controls 2.15
import "screens"

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 768
    title: "Software Ganadero"
    color: "#1A2E1C"

    Connections {
        target: appViewModel
        function onCurrentScreenChanged() {
            switch (appViewModel.currentScreen) {
                case "login":        stackView.replace(loginComp);        break
                case "register":     stackView.replace(registerComp);     break
                case "menu":         stackView.replace(menuComp);         break
                case "inventario":   stackView.replace(inventarioComp);   break
                case "reproductivo": stackView.replace(reproductivoComp); break
                case "leche":        stackView.replace(lecheComp);        break
                case "peso":         stackView.replace(pesoComp);         break
            }
        }
    }

    Component { id: loginComp;        LoginScreen {}               }
    Component { id: registerComp;     RegisterScreen {}            }
    Component { id: menuComp;         MenuScreen {}                }
    Component { id: inventarioComp;   InventarioScreen {}          }
    Component { id: reproductivoComp; RegistroReproductivoScreen {}}
    Component { id: lecheComp;        RegistroLecheScreen {}       }
    Component { id: pesoComp;         RegistroPesoScreen {}        }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: appViewModel.isLoggedIn ? menuComp : loginComp
    }
}