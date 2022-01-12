import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Flow {
    padding: 15
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        lightsControl.connectToRenderer(renderer)

        mappingState.checked = Qt.binding(function() {
            return renderer.normalMappingUsed
        })
        displayAsColors.checked = Qt.binding(function() {
            return renderer.displayAsColor
        })
        displayVectors.checked = Qt.binding(function() {
            return renderer.displayVectors
        })
    }

    ColumnLayout {
        id: columnLayout
        spacing: 15

        RowLayout {
            GroupBox {
                ColumnLayout {
                    Switch {
                        id: mappingState
                        text: "Normal mapping ON/OFF"

                        onClicked: {
                            currentRenderer.normalMappingUsed = !currentRenderer.normalMappingUsed
                        }
                    }
                    Switch {
                        id: displayAsColors
                        text: "Display normal maps as color"

                        onClicked: {
                            currentRenderer.displayAsColor = !currentRenderer.displayAsColor
                        }
                    }
                    Switch {
                        id: displayVectors
                        text: "Display vertex normals"

                        onClicked: {
                            currentRenderer.displayVectors = !currentRenderer.displayVectors
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
