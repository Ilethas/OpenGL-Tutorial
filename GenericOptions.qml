import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtWebEngine 1.5

Page
{
    id: page
    property url lessonOptionsUrl: ""
    property url lessonDescriptionUrl: ""
    property alias optionsControl: concreteLessonOptionsLoader.item

    onLessonOptionsUrlChanged: {
        console.log(lessonOptionsUrl)
    }

    header: TabBar {
        id: tabBar
        currentIndex: 0

        TabButton {
            text: "Opcje"
        }
        TabButton {
            text: "Opis"
        }
    }

    SwipeView
    {
        id: swipeView
        currentIndex: tabBar.currentIndex
        anchors.fill: parent
        interactive: false
        clip: true

        Item {
            id: optionsPage

            Rectangle {
                anchors.fill: parent
                color: "#cfcfcf"
            }

            Flickable {
                id: optionsFlickable
                anchors.fill: optionsPage
                boundsBehavior: Flickable.StopAtBounds
                contentWidth: concreteLessonOptionsLoader.width
                contentHeight: concreteLessonOptionsLoader.height
                clip: true

                ScrollBar.vertical: ScrollBar {
                    id: verticalScrollbar
                    parent: optionsFlickable.parent
                    anchors.top: optionsFlickable.top
                    anchors.bottom: optionsFlickable.bottom
                    anchors.right: optionsFlickable.right
                    anchors.bottomMargin: scrollbarCorner.visible? scrollbarCorner.height : 0
                    visible: optionsFlickable.contentHeight > optionsFlickable.height

                    background: Rectangle {
                        anchors.fill: parent
                        color: "#202020"
                    }

                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: 100
                        radius: width / 2
                        color: verticalScrollbar.pressed? "#6e6e6e" : (verticalScrollbar.hovered? "#8c8c8c" : "#aaaaaa")
                    }
                }
                ScrollBar.horizontal: ScrollBar {
                    id: horizontalScrollbar
                    parent: optionsFlickable.parent
                    anchors.left: optionsFlickable.left
                    anchors.right: optionsFlickable.right
                    anchors.bottom: optionsFlickable.bottom
                    anchors.rightMargin: scrollbarCorner.visible? scrollbarCorner.width : 0
                    visible: optionsFlickable.contentWidth > optionsFlickable.width

                    background: Rectangle {
                        anchors.fill: parent
                        color: "#202020"
                    }

                    contentItem: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 6
                        radius: height / 2
                        color: horizontalScrollbar.pressed? "#6e6e6e" : (horizontalScrollbar.hovered? "#8c8c8c" : "#aaaaaa")
                    }
                }

                Loader {
                    id: concreteLessonOptionsLoader
                    source: lessonOptionsUrl
                }
            }

            Rectangle {
                id: scrollbarCorner
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: Math.max(horizontalScrollbar.height, verticalScrollbar.width)
                height: width
                visible: horizontalScrollbar.visible && verticalScrollbar.visible
                color: "#202020"
            }
        }

        Item {
            id: descriptionPage
            implicitHeight: page.height - header.height

            WebEngineView {
                id: webEngineView
                anchors.fill: parent
                url: lessonDescriptionUrl
            }
        }
    }

//    property int flag: 0
//    footer: Button {
//        text: "Button"
//        onClicked: {
//            flag = (flag + 1) % 3
//            switch(flag)
//            {
//                case 0:
//                    lessonOptionsUrl = "Lesson1Options.qml"
//                    break

//                case 1:
//                    lessonOptionsUrl = "Lesson2Options.qml"
//                    break

//                case 2:
//                    lessonOptionsUrl = ""
//                    break
//            }
//        }
//    }
}
