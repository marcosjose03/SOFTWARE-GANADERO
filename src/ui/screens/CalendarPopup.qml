import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: calRoot
    width: 300
    height: visible ? 380 : 0
    visible: false
    clip: true

    signal fechaSeleccionada(string fecha)

    property int    selectedYear:  new Date().getFullYear()
    property int    selectedMonth: -1
    property string vista: "anio"

    function abrir() {
        vista         = "anio"
        selectedMonth = -1
        visible       = true
    }

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
        border.color: "#999"
        radius: 4

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8

            // ── Vista año ─────────────────────────────────────────────────
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: calRoot.vista === "anio"

                Text { text: "Seleccionar año"; font.bold: true }

                ListView {
                    id: yearList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: {
                        var years = []
                        for (var y = 1990; y <= new Date().getFullYear(); y++)
                            years.push(y)
                        return years
                    }
                    delegate: ItemDelegate {
                        width: yearList.width
                        text: modelData
                        highlighted: modelData === calRoot.selectedYear
                        onClicked: {
                            calRoot.selectedYear = modelData
                            calRoot.vista = "mes"
                        }
                    }
                    Component.onCompleted: positionViewAtIndex(
                        new Date().getFullYear() - 1990, ListView.Center)
                }
            }

            // ── Vista mes ─────────────────────────────────────────────────
            ColumnLayout {
                Layout.fillWidth: true
                visible: calRoot.vista === "mes"
                spacing: 8

                Text {
                    text: "Mes — " + calRoot.selectedYear
                    font.bold: true
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 3
                    columnSpacing: 6
                    rowSpacing: 6

                    Repeater {
                        model: ["Ene","Feb","Mar","Abr","May","Jun",
                                "Jul","Ago","Sep","Oct","Nov","Dic"]
                        Button {
                            Layout.fillWidth: true
                            text: modelData
                            onClicked: {
                                calRoot.selectedMonth = index + 1
                                calRoot.vista = "dia"
                            }
                        }
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: "← Año"
                    onClicked: calRoot.vista = "anio"
                }
            }

            // ── Vista día ─────────────────────────────────────────────────
            ColumnLayout {
                Layout.fillWidth: true
                visible: calRoot.vista === "dia"
                spacing: 8

                Text {
                    text: {
                        var m = ["","Ene","Feb","Mar","Abr","May","Jun",
                                 "Jul","Ago","Sep","Oct","Nov","Dic"]
                        return "Día — " + m[calRoot.selectedMonth] + " " + calRoot.selectedYear
                    }
                    font.bold: true
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 7
                    columnSpacing: 2
                    rowSpacing: 2

                    Repeater {
                        model: {
                            if (calRoot.selectedMonth < 1) return []
                            var dias = new Date(
                                calRoot.selectedYear,
                                calRoot.selectedMonth, 0).getDate()
                            var arr = []
                            for (var d = 1; d <= dias; d++) arr.push(d)
                            return arr
                        }
                        Button {
                            text: modelData
                            implicitWidth:  34
                            implicitHeight: 34
                            onClicked: {
                                var d = String(modelData).padStart(2,"0")
                                var m = String(calRoot.selectedMonth).padStart(2,"0")
                                calRoot.fechaSeleccionada(
                                    d + "/" + m + "/" + calRoot.selectedYear)
                                calRoot.visible = false
                            }
                        }
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: "← Mes"
                    onClicked: calRoot.vista = "mes"
                }
            }

            Button {
                Layout.fillWidth: true
                text: "Cancelar"
                onClicked: calRoot.visible = false
            }
        }
    }
}