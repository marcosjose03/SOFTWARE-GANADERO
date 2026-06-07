import QtQuick 2.15
import QtQuick.Controls 2.15
import "screens"

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 768
    title: "GanSoft — Software Ganadero"
    color: "white"

    palette.window:          "#ffffff"
    palette.windowText:      "#1a1a1a"
    palette.button:          "#f2f2f2"
    palette.buttonText:      "#1a1a1a"
    palette.base:            "#ffffff"
    palette.text:            "#1a1a1a"
    palette.highlight:       "#2d6a4f"
    palette.highlightedText: "#ffffff"
    palette.mid:             "#c8c8c8"
    palette.midlight:        "#e8e8e8"
    palette.dark:            "#aaaaaa"
    palette.shadow:          "#888888"

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