import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Flow {
    padding: 15
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        lightsControl.connectToRenderer(renderer)

        bloomState.checked = Qt.binding(function() {
            return renderer.bloomUsed;
        })
    }

    function updateActions()
    {
        flat.checked       = currentRenderer.currentInterpolationType === 0;
        smooth.checked     = currentRenderer.currentInterpolationType === 1;
        gouraud.checked    = currentRenderer.currentInterpolationType === 2;

        phong.checked      = currentRenderer.currentShadingType === 0;
        blinnPhong.checked = currentRenderer.currentShadingType === 1;
    }

    ColumnLayout {
        id: columnLayout
        spacing: 15

        RowLayout {
            GroupBox {
                GridLayout {
                    id: gridLayout
                    rows: 1
                    columns: 4

                    GroupBox {
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        GridLayout {
                            columns: 2
                            rows: 4
                            Label {
                                color: "#ffffff"
                                Layout.columnSpan: 2
                                font.pointSize: 12
                                text: "Bloom"
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                padding: 10

                                background: Rectangle {
                                    color: "#000000"
                                    radius: 15
                                    anchors.fill: parent

                                }
                            }

                            Label {
                                text: "Blur:"
                                font.pixelSize: 20

                            }
                            Slider {
                                id: blurValue
                                from: 0
                                value: 20
                                to: 50
                                stepSize: 1

                                property real realValue: value

                                onValueChanged: {
                                    currentRenderer.blur = realValue
                                }

                            }

                            Label {
                                text: "Threshold:"
                                font.pixelSize: 20

                            }
                            Slider {
                                id: thresholdValue
                                from: 0.0
                                value: 1.0
                                to: 1.0
                                stepSize: 0.05

                                property real realValue: value

                                onValueChanged: {
                                    currentRenderer.threshold = realValue
                                }

                            }


                            Switch {
                                id: bloomState
                                text: "Bloom ON/OFF"

                                onClicked: {
                                    currentRenderer.bloomUsed = !currentRenderer.bloomUsed
                                }
                            }


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
