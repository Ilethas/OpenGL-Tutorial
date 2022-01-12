import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Flow {
    padding: 25
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        lightsControl.connectToRenderer(renderer)

        gammaState.checked = Qt.binding(function() {
            return renderer.gammaCorrectionUsed
        })

    }

    ColumnLayout {
        id: columnLayout
        spacing: 25

        RowLayout {
            GroupBox {
                ColumnLayout {
                    spacing: 15

                    Label {
                        text: "Gamma value:"
                        font.pixelSize: 20

                    }
                    Slider {
                        id: gammaValue
                        from: 0.1
                        value: 1
                        to: 5
                        stepSize: 0.1

                        property real realValue: value

                        onValueChanged: {
                            currentRenderer.gamma = realValue
                        }

                    }



                    Switch {
                        id: gammaState
                        text: "Gamma correction ON/OFF"

                        onClicked: {
                            currentRenderer.gammaCorrectionUsed = !currentRenderer.gammaCorrectionUsed
                        }
                    }
                }
            }
        }

        LightControl {
            id: lightsControl
            Layout.columnSpan: 2
        }
    }
}
