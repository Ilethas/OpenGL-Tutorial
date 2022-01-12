import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Flow {
    padding: 15
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        lightsControl.connectToRenderer(renderer)
    }

    ColumnLayout {
        id: columnLayout
        spacing: 15

        Label {
            text: "Levels:"
            font.pixelSize: 20
        }

        Slider {
            id: toonLevel
            from: 1.0
            value: 5.0
            to: 7.0
            stepSize: 1.0

            property real realValue: value

            onValueChanged: {
                currentRenderer.levels = realValue
            }
        }

        LightControl {
            id: lightsControl
            Layout.columnSpan: 2
        }
    }
}
