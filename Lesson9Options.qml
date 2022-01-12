import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Flow {
    padding: 25
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        lightsControl.connectToRenderer(renderer)

        stencilState.checked = Qt.binding(function() {
            return renderer.outliningUsed
        })
        cubeStencilState.checked = Qt.binding(function() {
            return renderer.cubeOutliningUsed
        })
        cubeStencilState2.checked = Qt.binding(function() {
            return renderer.cubeOutlining2Used
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
                        id: stencilState
                        text: "Temple outlining ON/OFF"

                        onClicked: {
                            currentRenderer.outliningUsed = !currentRenderer.outliningUsed
                        }
                    }
                    Switch {
                        id:cubeStencilState
                        text: "Lower cube outlining ON/OFF"

                        onClicked: {
                            currentRenderer.cubeOutliningUsed = !currentRenderer.cubeOutliningUsed
                        }

                    }

                    Switch{
                        id:cubeStencilState2
                        text: "Upper cube outlining ON/OFF"

                        onClicked: {
                            currentRenderer.cubeOutlining2Used = !currentRenderer.cubeOutlining2Used
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
