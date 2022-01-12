import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Flow {
    padding: 25
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        lightsControl.connectToRenderer(renderer)

        hdrState.checked = Qt.binding(function() {
            return renderer.hdrUsed
        })

    }

    ColumnLayout {
        id: columnLayout
        spacing: 25

        RowLayout {
            GroupBox {
                ColumnLayout {
                    spacing: 15

                    Switch {
                        id: hdrState
                        text: "HDR ON/OFF"

                        onClicked: {
                            currentRenderer.hdrUsed = !currentRenderer.hdrUsed
                        }
                    }

                    Label {
                        text: "Gamma value:"
                        font.pixelSize: 20

                    }
                    Slider {
                        id: gammaValue
                        from: 0.8
                        value: 1
                        to: 2.2
                        stepSize: 0.1

                        property real realValue: value

                        onValueChanged: {
                            currentRenderer.gamma = realValue
                        }
                    }

                    Label {
                        text: "Exposure value:"
                        font.pixelSize: 20

                    }
                    Slider {
                        id: exposureValue
                        from:0.1
                        value:1
                        to:5
                        stepSize: 0.1

                        property real realValue: value

                        onValueChanged: {
                            currentRenderer.exposure = realValue
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
