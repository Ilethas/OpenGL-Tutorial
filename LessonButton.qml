import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.3

Button {
    id: lessonButton
    implicitWidth: 125
    implicitHeight: 125

    property int borderThickness: 3
    property double pressedScale: 0.95

    property color backgroundColor: "#404040"
    property color highlightColor: "#505050"
    property color glowColor: "#88ffff77"
    property int glowRadius: 30
    property int glowSamples: 120

    property alias lessonTitle: titleText
    property alias lessonImage: image

    background: Rectangle {
        color: lessonButton.hovered || lessonButton.pressed?
                   lessonButton.highlightColor : lessonButton.backgroundColor
        anchors.fill: parent
    }

    contentItem: ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        clip: false
        spacing: 5

        Item {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredWidth: image.width
            Layout.preferredHeight: image.height

            Image {
                id: image
                fillMode: Image.PreserveAspectFit
                source: "cube.png"
            }

            Glow {
                visible: lessonButton.hovered || lessonButton.pressed
                color: lessonButton.glowColor
                radius: lessonButton.glowRadius
                samples: lessonButton.glowSamples
                anchors.fill: image
                source: image
            }

            scale: lessonButton.pressed? lessonButton.pressedScale : 1
        }

        Item {
            id: item1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredWidth: parent.width * 0.9
            Layout.preferredHeight: parent.height / 3

            Text {
                id: titleText
                color: "#ffffff"
                text: "Lekcja 1"
                font.pixelSize: 14
                anchors.fill: parent
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.bold: true
                font.family: "Arial"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Glow {
                visible: lessonButton.hovered || lessonButton.pressed
                color: lessonButton.glowColor
                radius: lessonButton.glowRadius
                samples: lessonButton.glowSamples
                anchors.fill: titleText
                source: titleText
            }

            scale: lessonButton.pressed? lessonButton.pressedScale : 1
        }

        Rectangle {
            id: borderRectangle
            height: lessonButton.borderThickness
            color: "#000000"
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
            Layout.fillWidth: true
            transformOrigin: Item.Bottom
        }
    }
}
