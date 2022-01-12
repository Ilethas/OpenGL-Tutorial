import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0
import LightModel 1.0

RowLayout {
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        listView.model = renderer.lightModel
    }

    function humanize(x, precision) {
        return x.toFixed(precision).replace(/\.?0*$/, '');
    }

    function bindControls() {
        //=========================
        //===== Light type
        //=========================
        lightType.currentIndex = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? listView.model.getLightProperty(listView.currentIndex, LightModel.LightType) : -1
        })

        //=========================
        //===== Light visibility
        //=========================
        lightVisibility.checked = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? listView.model.getLightProperty(listView.currentIndex, LightModel.Visibility) : false
        })

        //=========================
        //===== Light pilot mode
        //=========================
        lightPilotMode.checked = Qt.binding(function() {
            return currentRenderer.lightPilotMode
        })

        //=========================
        //===== Light direction
        //=========================
        directionGridLayout.visible = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? listView.model.getLightProperty(listView.currentIndex, LightModel.LightType) !== Light.PointLight : true
        })
        directionX.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.Direction).x, 5) : ""
        })
        directionY.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.Direction).y, 5) : ""
        })
        directionZ.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.Direction).z, 5) : ""
        })

        //=========================
        //===== Light position
        //=========================
        positionX.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.Position).x, 5) : ""
        })
        positionY.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.Position).y, 5) : ""
        })
        positionZ.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.Position).z, 5) : ""
        })

        //=========================
        //===== Light color
        //=========================
        colorRectangle.color = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? listView.model.getLightProperty(listView.currentIndex, LightModel.Color) : "black"
        })
        red.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? Math.floor(listView.model.getLightProperty(listView.currentIndex, LightModel.Color).r * 255) : ""
        })
        green.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? Math.floor(listView.model.getLightProperty(listView.currentIndex, LightModel.Color).g * 255) : ""
        })
        blue.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? Math.floor(listView.model.getLightProperty(listView.currentIndex, LightModel.Color).b * 255) : ""
        })

        //=========================
        //===== Light brightness
        //=========================
        brightness.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.Brightness), 5) : ""
        })

        //=========================
        //===== Light attenuation radius
        //=========================
        attenuation.visible = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? listView.model.getLightProperty(listView.currentIndex, LightModel.LightType) !== Light.DirectionalLight : true
        })
        attenuationLabel.visible = Qt.binding(function() { return attenuation.visible })
        attenuation.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.AttenuationRadius), 5) : ""
        })

        //=========================
        //===== Light inner cone angle
        //=========================
        innerConeAngle.visible = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? listView.model.getLightProperty(listView.currentIndex, LightModel.LightType) === Light.SpotLight : true
        })
        innerConeAngleLabel.visible = Qt.binding(function() { return innerConeAngle.visible })
        innerConeAngle.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.InnerConeAngle), 5) : ""
        })

        //=========================
        //===== Light outer cone angle
        //=========================
        outerConeAngle.visible = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? listView.model.getLightProperty(listView.currentIndex, LightModel.LightType) === Light.SpotLight : true
        })
        outerConeAngleLabel.visible = Qt.binding(function() { return outerConeAngle.visible })
        outerConeAngle.placeholderText = Qt.binding(function() {
            var counter = listView.model.updateCounter
            return listView.currentItem != undefined? humanize(listView.model.getLightProperty(listView.currentIndex, LightModel.OuterConeAngle), 5) : ""
        })
    }

    Rectangle {
        id: lightsCreatedPanel
        Layout.preferredWidth: lightPropertiesPanel.width
        Layout.preferredHeight: 500
        color: "#858585"
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop

        ListModel {
            id: testModel
            ListElement {
                name: "Sun"
            }
            ListElement {
                name: "Light 1"
            }
            ListElement {
                name: "Light 2"
            }
        }
        ColumnLayout {
            spacing: 0
            anchors.fill: parent
            Rectangle {
                id: header
                implicitHeight: rowLayout.implicitHeight + 10
                color: "#707070"
                Layout.fillWidth: true
                RowLayout {
                    id: rowLayout
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.margins: 5
                    TextField {
                        id: lightName
                        text: ""
                        placeholderText: "Light name"
                        Layout.maximumWidth: 160
                        Layout.minimumWidth: 160
                        maximumLength: 20
                        font.pointSize: 8
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            addButton.clicked()
                        }
                    }
                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    Button {
                        id: addButton
                        background: Image {
                            id: addButtonImage
                            sourceSize.height: 25
                            sourceSize.width: 25
                            source: "Button icons/plus.png"
                        }
                        Glow {
                            visible: addButton.hovered || addButton.pressed
                            color: "#ff88bb88"
                            radius: 20
                            samples: 40
                            anchors.fill: addButton
                            source: addButtonImage
                        }
                        scale: addButton.pressed? 0.9 : 1.0

                        onClicked: {
                            if (listView.model.rowCount() < 25 && lightName.text != "")
                            {
                                listView.model.append({ name: lightName.text })
                                lightName.text = ""
                            }
                        }
                    }
                    Button {
                        id: removeButton
                        background: Image {
                            id: removeButtonImage
                            sourceSize.height: 25
                            sourceSize.width: 25
                            source: "Button icons/minus.png"
                        }
                        Glow {
                            visible: removeButton.hovered || removeButton.pressed
                            color: "#ffbb8888"
                            radius: 20
                            samples: 40
                            anchors.fill: removeButton
                            source: removeButtonImage
                        }
                        scale: removeButton.pressed? 0.9 : 1.0

                        onClicked: {
                            if (listView.currentItem != undefined)
                            {
                                listView.model.remove(listView.currentIndex)
                            }
                        }
                    }
                }
            }

            ListView {
                id: listView
                Layout.fillHeight: true
                Layout.fillWidth: true
                boundsBehavior: Flickable.StopAtBounds
                clip: true
                spacing: 1

                delegate: Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 25
                    color: "#959595aa"
                    Flow {
                        anchors.fill: parent
                        padding: 5
                        Text {
                            text: name
                            font.pointSize: 8
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listView.currentIndex = index
                            currentRenderer.currentLight = index
                            bindControls()
                        }
                    }
                }
                highlight: Rectangle {
                    color: "#c5c5c5"
                }
            }
        }
    }

    GroupBox {
        id: lightPropertiesPanel
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        ColumnLayout {
            spacing: 10

            GridLayout {
                flow: GridLayout.TopToBottom
                rows: 2
                columns: 3

                Text {
                    text: "Light type"
                    font.pointSize: 8
                }
                ComboBox {
                    id: lightType
                    model: ["Point light", "Directional light", "Spot light"]
                    currentIndex: -1

                    onCurrentIndexChanged: {
                        if (listView.currentItem != undefined)
                        {
                            listView.model.setLightProperty(listView.currentIndex, currentIndex, LightModel.LightType)
                            bindControls()
                        }
                    }
                }
                CheckBox {
                    id: lightVisibility
                    text: "Visibile"
                    Layout.columnSpan: 2

                    onClicked: {
                        if (listView.currentItem != undefined)
                        {
                            listView.model.setLightProperty(listView.currentIndex, checked, LightModel.Visibility)
                        }
                    }
                }

                Switch {
                    id: lightPilotMode
                    text: "Pilot light"
                    Layout.columnSpan: 2

                    ToolTip.visible: hovered
                    ToolTip.delay: 1000
                    ToolTip.text: "Ustawia pozycję i kierunek aktualnie wybranego światła na podstawie pozycji oraz rotacji kamery.\n" +
                                  "Użyj tej opcji, aby szybciej rozstawić oświetlenie na scenie 3D."

                    onClicked: {
                        currentRenderer.lightPilotMode = !currentRenderer.lightPilotMode
                    }
                }
            }

            GridLayout {
                columns: 6
                rows: 2

                Text {
                    text: "Position"
                    padding: 15
                    Layout.columnSpan: 6
                    font.pointSize: 8
                }

                Text {
                    text: "X"
                    font.pointSize: 8
                }
                TextField {
                    id: positionX
                    Layout.maximumWidth: 80
                    Layout.minimumWidth: 80
                    maximumLength: 10
                    font.pointSize: 8
                    validator: DoubleValidator {}
                    selectByMouse: true
                    font.family: "Arial"

                    onEditingFinished: {
                        if (listView.currentItem != undefined)
                        {
                            var previousPosition = listView.model.getLightProperty(listView.currentIndex, LightModel.Position)
                            var x = Number(positionX.text.replace(',', '.'))
                            listView.model.setLightProperty(listView.currentIndex, Qt.vector3d(x, previousPosition.y, previousPosition.z), LightModel.Position)
                            text = ""
                        }
                    }
                }

                Text {
                    text: "Y"
                    font.pointSize: 8
                }
                TextField {
                    id: positionY
                    Layout.maximumWidth: 80
                    Layout.minimumWidth: 80
                    maximumLength: 10
                    font.pointSize: 8
                    validator: DoubleValidator {}
                    selectByMouse: true
                    font.family: "Arial"

                    onEditingFinished: {
                        if (listView.currentItem != undefined)
                        {
                            var previousPosition = listView.model.getLightProperty(listView.currentIndex, LightModel.Position)
                            var y = Number(positionY.text.replace(',', '.'))
                            listView.model.setLightProperty(listView.currentIndex, Qt.vector3d(previousPosition.x, y, previousPosition.z), LightModel.Position)
                            text = ""
                        }
                    }
                }

                Text {
                    text: "Z"
                    font.pointSize: 8
                }
                TextField {
                    id: positionZ
                    Layout.maximumWidth: 80
                    Layout.minimumWidth: 80
                    maximumLength: 10
                    font.pointSize: 8
                    validator: DoubleValidator {}
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    selectByMouse: true
                    font.family: "Arial"

                    onEditingFinished: {
                        if (listView.currentItem != undefined)
                        {
                            var previousPosition = listView.model.getLightProperty(listView.currentIndex, LightModel.Position)
                            var z = Number(positionZ.text.replace(',', '.'))
                            listView.model.setLightProperty(listView.currentIndex, Qt.vector3d(previousPosition.x, previousPosition.y, z), LightModel.Position)
                            text = ""
                        }
                    }
                }
            }

            GridLayout {
                id: directionGridLayout
                columns: 6
                rows: 2

                Text {
                    text: "Direction"
                    Layout.columnSpan: 6
                    font.pointSize: 8

                }

                Text {
                    text: "X"
                    font.pointSize: 8
                }
                TextField {
                    id: directionX
                    Layout.maximumWidth: 80
                    Layout.minimumWidth: 80
                    maximumLength: 10
                    font.pointSize: 8
                    validator: DoubleValidator {}
                    selectByMouse: true
                    font.family: "Arial"

                    onEditingFinished: {
                        if (listView.currentItem != undefined)
                        {
                            var previousDirection = listView.model.getLightProperty(listView.currentIndex, LightModel.Direction)
                            var x = Number(directionX.text.replace(',', '.'))
                            listView.model.setLightProperty(listView.currentIndex, Qt.vector3d(x, previousDirection.y, previousDirection.z), LightModel.Direction)
                            text = ""
                        }
                    }
                }

                Text {
                    text: "Y"
                    font.pointSize: 8
                }
                TextField {
                    id: directionY
                    Layout.maximumWidth: 80
                    Layout.minimumWidth: 80
                    maximumLength: 10
                    font.pointSize: 8
                    validator: DoubleValidator {}
                    selectByMouse: true
                    font.family: "Arial"

                    onEditingFinished: {
                        if (listView.currentItem != undefined)
                        {
                            var previousDirection = listView.model.getLightProperty(listView.currentIndex, LightModel.Direction)
                            var y = Number(directionY.text.replace(',', '.'))
                            listView.model.setLightProperty(listView.currentIndex, Qt.vector3d(previousDirection.x, y, previousDirection.z), LightModel.Direction)
                            text = ""
                        }
                    }
                }

                Text {
                    text: "Z"
                    font.pointSize: 8
                }
                TextField {
                    id: directionZ
                    Layout.maximumWidth: 80
                    Layout.minimumWidth: 80
                    maximumLength: 10
                    font.pointSize: 8
                    validator: DoubleValidator {}
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    selectByMouse: true
                    font.family: "Arial"

                    onEditingFinished: {
                        if (listView.currentItem != undefined)
                        {
                            var previousDirection = listView.model.getLightProperty(listView.currentIndex, LightModel.Direction)
                            var z = Number(directionZ.text.replace(',', '.'))
                            listView.model.setLightProperty(listView.currentIndex, Qt.vector3d(previousDirection.x, previousDirection.y, z), LightModel.Direction)
                            text = ""
                        }
                    }
                }
            }

            RowLayout {
                ColumnLayout {
                    Text {
                        text: "Color"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.columnSpan: 1
                        font.pointSize: 8
                    }
                    Button {
                        background: Rectangle {
                            id: colorRectangle
                            border.width: 1
                        }
                        Layout.preferredHeight: 50
                        Layout.preferredWidth: 50
                        ToolTip.visible: hovered
                        ToolTip.delay: 1000
                        ToolTip.text: "Kliknij lewym przyciskiem myszy,\naby otworzyć narzędzie do wybierania koloru."

                        onClicked: {
                            colorDialog.open()
                        }
                    }
                }

                Item {
                    Layout.preferredWidth: 15
                }

                GridLayout {
                    columns: 2
                    rows: 3

                    Text {
                        text: "Red"
                        font.pointSize: 8
                    }
                    TextField {
                        id: red
                        Layout.maximumWidth: 80
                        Layout.minimumWidth: 80
                        maximumLength: 10
                        font.pointSize: 8
                        validator: DoubleValidator {
                            top: 255
                            bottom: 0
                        }
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            if (listView.currentItem != undefined)
                            {
                                var previousColor = listView.model.getLightProperty(listView.currentIndex, LightModel.Color)
                                var r = Number(red.text.replace(',', '.')) / 255
                                listView.model.setLightProperty(listView.currentIndex, Qt.rgba(r, previousColor.g, previousColor.b, 1), LightModel.Color)
                                text = ""
                            }
                        }
                    }

                    Text {
                        text: "Green"
                        font.pointSize: 8
                    }
                    TextField {
                        id: green
                        Layout.maximumWidth: 80
                        Layout.minimumWidth: 80
                        maximumLength: 10
                        font.pointSize: 8
                        validator: DoubleValidator {
                            top: 255
                            bottom: 0
                        }
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            if (listView.currentItem != undefined)
                            {
                                var previousColor = listView.model.getLightProperty(listView.currentIndex, LightModel.Color)
                                var g = Number(green.text.replace(',', '.')) / 255
                                listView.model.setLightProperty(listView.currentIndex, Qt.rgba(previousColor.r, g, previousColor.b, 1), LightModel.Color)
                                text = ""
                            }
                        }
                    }

                    Text {
                        text: "Blue"
                        font.pointSize: 8
                    }
                    TextField {
                        id: blue
                        Layout.maximumWidth: 80
                        Layout.minimumWidth: 80
                        maximumLength: 10
                        font.pointSize: 8
                        validator: DoubleValidator {
                            top: 255
                            bottom: 0
                        }
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            if (listView.currentItem != undefined)
                            {
                                var previousColor = listView.model.getLightProperty(listView.currentIndex, LightModel.Color)
                                var b = Number(blue.text.replace(',', '.')) / 255
                                listView.model.setLightProperty(listView.currentIndex, Qt.rgba(previousColor.r, previousColor.g, b, 1), LightModel.Color)
                                text = ""
                            }
                        }
                    }
                }

                ColorDialog {
                    id: colorDialog
                    title: "Please choose a color"

                    onCurrentColorChanged: {
                        if (listView.currentItem != undefined)
                        {
                            listView.model.setLightProperty(listView.currentIndex, colorDialog.currentColor, LightModel.Color)
                        }
                    }
                }

                Item {
                    Layout.preferredWidth: 15

                }

                ColumnLayout {
                    spacing: 1
                    Text {
                        id: innerConeAngleLabel
                        text: "Inner cone\nangle"
                        horizontalAlignment: Text.AlignHCenter
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.columnSpan: 1
                        font.pointSize: 8
                    }
                    TextField {
                        id: innerConeAngle
                        Layout.maximumWidth: 80
                        Layout.minimumWidth: 80
                        maximumLength: 10
                        font.pointSize: 8
                        validator: DoubleValidator {
                            top: 360
                            bottom: 0
                        }
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            if (listView.currentItem != undefined)
                            {
                                var value = Number(innerConeAngle.text.replace(',', '.'))
                                listView.model.setLightProperty(listView.currentIndex, value, LightModel.InnerConeAngle)
                                text = ""
                            }
                        }
                    }
                    Text {
                        id: outerConeAngleLabel
                        text: "Outer cone\nangle"
                        horizontalAlignment: Text.AlignHCenter
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.columnSpan: 1
                        font.pointSize: 8
                    }
                    TextField {
                        id: outerConeAngle
                        Layout.maximumWidth: 80
                        Layout.minimumWidth: 80
                        maximumLength: 10
                        font.pointSize: 8
                        validator: DoubleValidator {
                            top: 360
                            bottom: 0
                        }
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            if (listView.currentItem != undefined)
                            {
                                var value = Number(outerConeAngle.text.replace(',', '.'))
                                listView.model.setLightProperty(listView.currentIndex, value, LightModel.OuterConeAngle)
                                text = ""
                            }
                        }
                    }

                    Text {
                        text: "Brightness"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.columnSpan: 1
                        font.pointSize: 8
                    }
                    TextField {
                        id: brightness
                        Layout.maximumWidth: 80
                        Layout.minimumWidth: 80
                        maximumLength: 10
                        font.pointSize: 8
                        validator: DoubleValidator {
                            bottom: 0
                        }
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            if (listView.currentItem != undefined)
                            {
                                var value = Number(brightness.text.replace(',', '.'))
                                listView.model.setLightProperty(listView.currentIndex, value, LightModel.Brightness)
                                text = ""
                            }
                        }
                    }

                    Text {
                        id: attenuationLabel
                        text: "Attenuation\nradius"
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.columnSpan: 1
                        font.pointSize: 8
                    }
                    TextField {
                        id: attenuation
                        Layout.maximumWidth: 80
                        Layout.minimumWidth: 80
                        maximumLength: 10
                        font.pointSize: 8
                        validator: DoubleValidator {
                            bottom: 0
                        }
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        selectByMouse: true
                        font.family: "Arial"

                        onEditingFinished: {
                            if (listView.currentItem != undefined)
                            {
                                var value = Number(attenuation.text.replace(',', '.'))
                                listView.model.setLightProperty(listView.currentIndex, value, LightModel.AttenuationRadius)
                                text = ""
                            }
                        }
                    }
                }
            }
        }
    }
}
