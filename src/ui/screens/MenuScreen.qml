import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width:  parent ? parent.width  : 1024
    height: parent ? parent.height : 768

    Rectangle { anchors.fill: parent; color: Theme.fondo }

    ColumnLayout {
        anchors.fill: parent
        spacing:      0

        // ── Barra superior ────────────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            height:           Theme.alturaEncabezado
            color:            Theme.superficie

            Rectangle {
                anchors.left:   parent.left
                anchors.right:  parent.right
                anchors.bottom: parent.bottom
                height:         1
                color:          Theme.borde
            }

            RowLayout {
                anchors.fill:         parent
                anchors.leftMargin:   Theme.espacioLg
                anchors.rightMargin:  Theme.espacioMd
                spacing:              Theme.espacioSm

                Text {
                    text:           "🐄"
                    font.pixelSize: 22
                }
                Text {
                    text:         "GanSoft"
                    font.family:  Theme.fuente
                    font.pixelSize: Theme.tamSubtitulo
                    font.weight:    Font.Bold
                    color:          Theme.primario
                }

                Item { Layout.fillWidth: true }

                // Saludo al usuario
                Text {
                    text:           "Hola, " + appViewModel.userName
                    font.family:    Theme.fuente
                    font.pixelSize: Theme.tamCuerpo
                    color:          Theme.textoSecundario
                }

                // Botón cerrar sesión
                AbstractButton {
                    id: btnLogout
                    implicitWidth:  36
                    implicitHeight: 36
                    onClicked: appViewModel.logout()

                    background: Rectangle {
                        radius: Theme.radioSm
                        color:  btnLogout.hovered ? Theme.criticoFondo : "transparent"
                        border.color: btnLogout.hovered ? Theme.critico : "transparent"
                        border.width: 1
                        Behavior on color { ColorAnimation { duration: 100 } }
                    }

                    contentItem: Text {
                        text:                "⏻"
                        font.pixelSize:      16
                        color:               btnLogout.hovered ? Theme.critico : Theme.textoSecundario
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:   Text.AlignVCenter
                    }
                }
            }
        }

        // ── Contenido principal ───────────────────────────────────────────
        Flickable {
            Layout.fillWidth:  true
            Layout.fillHeight: true
            contentWidth:      width
            contentHeight:     contenido.implicitHeight + Theme.espacioXl * 2
            clip:              true
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            Column {
                id:              contenido
                anchors.top:     parent.top
                anchors.left:    parent.left
                anchors.right:   parent.right
                anchors.margins: Theme.espacioXl
                anchors.topMargin: Theme.espacioXl
                spacing:         Theme.espacioXl

                // Sección: Gestión del hato
                Column {
                    width:   parent.width
                    spacing: Theme.espacioMd

                    Text {
                        text:           "Gestión del hato"
                        font.family:    Theme.fuente
                        font.pixelSize: Theme.tamEtiqueta
                        font.weight:    Font.DemiBold
                        color:          Theme.textoSecundario
                        leftPadding:    Theme.espacioXs
                    }

                    GridLayout {
                        width:       parent.width
                        columns:     Math.max(2, Math.floor(parent.width / 220))
                        columnSpacing: Theme.espacioMd
                        rowSpacing:    Theme.espacioMd

                        TarjetaModulo {
                            icono:  "🐄"
                            titulo: "Inventario"
                            Layout.fillWidth: true
                            onClicked: appViewModel.goToInventario()
                        }
                        TarjetaModulo {
                            icono:  "📋"
                            titulo: "Registro Reproductivo"
                            Layout.fillWidth: true
                            onClicked: appViewModel.goToRegistroReproductivo()
                        }
                        TarjetaModulo {
                            icono:  "🥛"
                            titulo: "Registro de Leche"
                            Layout.fillWidth: true
                            onClicked: appViewModel.goToRegistroLeche()
                        }
                        TarjetaModulo {
                            icono:  "⚖️"
                            titulo: "Registro de Peso"
                            Layout.fillWidth: true
                            onClicked: appViewModel.goToRegistroPeso()
                        }
                    }
                }

                // Sección: Análisis
                Column {
                    width:   parent.width
                    spacing: Theme.espacioMd

                    Text {
                        text:           "Análisis y proyecciones"
                        font.family:    Theme.fuente
                        font.pixelSize: Theme.tamEtiqueta
                        font.weight:    Font.DemiBold
                        color:          Theme.textoSecundario
                        leftPadding:    Theme.espacioXs
                    }

                    GridLayout {
                        width:        parent.width
                        columns:      Math.max(2, Math.floor(parent.width / 220))
                        columnSpacing: Theme.espacioMd
                        rowSpacing:    Theme.espacioMd

                        TarjetaModulo {
                            icono:           "📊"
                            titulo:          "Herramienta Visual"
                            proximamente:    true
                            Layout.fillWidth: true
                        }
                        TarjetaModulo {
                            icono:           "🔔"
                            titulo:          "Alertas"
                            proximamente:    true
                            Layout.fillWidth: true
                        }
                        TarjetaModulo {
                            icono:           "📈"
                            titulo:          "Proyecciones"
                            proximamente:    true
                            Layout.fillWidth: true
                        }
                    }
                }

                // Acciones secundarias
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: Theme.espacioMd

                    BotonSecundario {
                        text:  "Salir de la app"
                        width: 180
                        onClicked: Qt.quit()
                    }
                }
            }
        }
    }

    // Componente interno: tarjeta de módulo
    component TarjetaModulo: AbstractButton {
        id: modBtn

        property string icono:       ""
        property string titulo:      ""
        property bool   proximamente: false

        implicitHeight: 110
        enabled:        !proximamente

        background: Rectangle {
            radius:       Theme.radio
            color:        modBtn.proximamente  ? Theme.superficieAlt
                        : modBtn.pressed       ? Theme.primarioClaro
                        : modBtn.hovered       ? Theme.superficieAlt
                                               : Theme.superficie
            border.color: modBtn.pressed || modBtn.hovered
                          ? (modBtn.proximamente ? Theme.borde : Theme.primario)
                          : Theme.borde
            border.width: 1
            Behavior on color        { ColorAnimation { duration: 100 } }
            Behavior on border.color { ColorAnimation { duration: 100 } }
        }

        contentItem: Item {
            Column {
                anchors.centerIn: parent
                spacing:          Theme.espacioSm

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text:           modBtn.icono
                    font.pixelSize: 32
                    opacity:        modBtn.proximamente ? 0.45 : 1
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text:                modBtn.titulo
                    font.family:         Theme.fuente
                    font.pixelSize:      Theme.tamCuerpo
                    font.weight:         Font.DemiBold
                    color:               modBtn.proximamente
                                         ? Theme.deshabilitado : Theme.textoPrimario
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode:            Text.WordWrap
                    width:               parent.parent.width - Theme.espacioLg
                }
            }

            // Badge "Próximamente"
            Rectangle {
                anchors.top:   parent.top
                anchors.right: parent.right
                anchors.topMargin:   Theme.espacioSm
                anchors.rightMargin: Theme.espacioSm
                visible: modBtn.proximamente
                width:   contenidoBadge.implicitWidth + Theme.espacioSm
                height:  20
                radius:  10
                color:   Theme.borde

                Text {
                    id: contenidoBadge
                    anchors.centerIn: parent
                    text:           "Próximamente"
                    font.family:    Theme.fuente
                    font.pixelSize: Theme.tamMicro
                    color:          Theme.textoSecundario
                }
            }
        }
    }
}
