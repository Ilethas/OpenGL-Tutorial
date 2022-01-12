import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import OpenGLTutorial 1.0

Window {
    id: mainWindow
    visible: true
    width: 1280
    height: 720
    minimumWidth: 640
    minimumHeight: 480
    title: qsTr("OpenGL Tutorial")

    RowLayout {
        id: mainLayout
        visible: true
        spacing: 0
        anchors.fill: parent

        RowLayout {
            property bool hidden: false
            property int panelWidth: 185

            id: lessonPanel
            spacing: 0
            Layout.fillHeight: true
            Layout.minimumWidth: (lessonPanel.hidden?
                                      lessonPanelVisiblityButton.width :
                                      lessonPanel.panelWidth + lessonScrollbar.width)
            Layout.maximumWidth: (lessonPanel.hidden?
                                      lessonPanelVisiblityButton.width :
                                      lessonPanel.panelWidth + lessonScrollbar.width)

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: !lessonPanel.hidden

                Rectangle {
                    color: "#202020"
                    anchors.fill: parent
                }

                Flickable {
                    id: lessonPanelFlickable
                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds
                    anchors.fill: parent
                    contentWidth: lessonButtonsLayout.width
                    contentHeight: lessonButtonsLayout.height

                    ScrollBar.vertical: ScrollBar {
                        id: lessonScrollbar
                        parent: lessonPanelFlickable.parent
                        anchors.top: lessonPanelFlickable.top
                        anchors.bottom: lessonPanelFlickable.bottom
                        anchors.right: lessonPanelFlickable.right
                        visible: lessonPanelFlickable.contentHeight > lessonPanelFlickable.height

                        background: Rectangle {
                            anchors.fill: parent
                            color: "#202020"
                        }

                        contentItem: Rectangle {
                            implicitWidth: 6
                            implicitHeight: 100
                            radius: width / 2
                            color: lessonScrollbar.pressed? "#6e6e6e" : (lessonScrollbar.hovered? "#8c8c8c" : "#aaaaaa")
                        }
                    }

                    ColumnLayout {
                        id: lessonButtonsLayout
                        width: lessonPanelFlickable.width - (lessonScrollbar.visible? lessonScrollbar.width : 0)
                        spacing: 0

                        LessonButton {
                            Layout.fillWidth: true
                            lessonTitle.text: "Ekran powitalny"
                            lessonImage.source: "Button icons/welcomeScreen.png"
                            glowColor: "#aafff1a5"

                            onClicked:
                            {
                                mainScreenLoader.sourceComponent = welcomeScreen
                                mainScreenLoader.item.welcomeScreenUrl = "file:///" + currentPath + "/Assets/welcomeScreen/welcomeScreen.html"
                                optionsPanel.visible = false
                            }
                        }
                        LessonButton {
                            Layout.fillWidth: true
                            lessonTitle.text: "Lekcja 1: Modele oświetlenia"
                            lessonImage.source: "Button icons/lesson1.png"
                            glowColor: "#aaffdeb7"

                            onClicked:
                            {
                                mainScreenLoader.sourceComponent = openGLView
                                mainScreenLoader.item.rendererClass = "Lesson1Renderer"
                                optionsPanel.visible = true
                            }
                        }
                        LessonButton {
                            Layout.fillWidth: true
                            lessonTitle.text: "Lekcja 2: Normal mapping"
                            lessonImage.source: "Button icons/lesson2.png"
                            glowColor: "#aaaaaa"

                            onClicked:
                            {
                                mainScreenLoader.sourceComponent = openGLView
                                mainScreenLoader.item.rendererClass = "Lesson2Renderer"
                                optionsPanel.visible = true
                            }
                        }

                        LessonButton {
                            Layout.fillWidth: true
                            lessonTitle.text: "Lekcja 3: Korekcja gamma"
                            lessonImage.source: "Button icons/lesson4.png"
                            glowColor: "#aaaacbff"

                            onClicked:
                            {
                                mainScreenLoader.sourceComponent = openGLView
                                mainScreenLoader.item.rendererClass = "Lesson4Renderer"
                                optionsPanel.visible = true
                            }

                        }
                        LessonButton {
                                                    Layout.fillWidth: true
                                                    lessonTitle.text: "Lekcja 4: HDR"
                                                    lessonImage.source: "Button icons/lesson5.png"
                                                    glowColor: "#d3795d"

                                                    onClicked: {
                                                        mainScreenLoader.sourceComponent = openGLView
                                                        mainScreenLoader.item.rendererClass = "Lesson5Renderer"
                                                        optionsPanel.visible = true
                                                    }


                                                }
                        LessonButton {
                                                    Layout.fillWidth: true
                                                    lessonTitle.text: "Lekcja 5: Bloom"
                                                    lessonImage.source: "Button icons/lesson6.png"

                                                    onClicked:
                                                    {
                                                        mainScreenLoader.sourceComponent = openGLView
                                                        mainScreenLoader.item.rendererClass = "Lesson6Renderer"
                                                        optionsPanel.visible = true
                                                    }
                                                }
                        LessonButton {
                            Layout.fillWidth: true
                            lessonTitle.text: "Lekcja 6: Toon outline shader"
                            lessonImage.source: "Button icons/lesson7.png"
                            glowColor: "#a89577"

                            onClicked:
                            {
                                mainScreenLoader.sourceComponent = openGLView
                                mainScreenLoader.item.rendererClass = "Lesson7Renderer"
                                optionsPanel.visible = true
                            }
                        }
                        LessonButton {
                            Layout.fillWidth: true
                            lessonTitle.text: "Lekcja 7: Symulacja wody – fale gerstnera"
                            lessonImage.source: "Button icons/lesson8.png"
                            glowColor: "#aad1d4ff"

                            onClicked:
                            {
                                mainScreenLoader.sourceComponent = openGLView
                                mainScreenLoader.item.rendererClass = "Lesson8Renderer"
                                optionsPanel.visible = true
                            }
                        }
                        LessonButton {
                                                    Layout.fillWidth: true
                                                    lessonTitle.text: "Lekcja 8: Bufor szablonowy – zaznaczanie obiektów"
                                                    lessonImage.source: "Button icons/lesson9.png"
                                                    glowColor: "#bb7fc46f"
                                                    onClicked:
                                                    {
                                                        mainScreenLoader.sourceComponent = openGLView
                                                        mainScreenLoader.item.rendererClass = "Lesson9Renderer"
                                                        optionsPanel.visible = true
                                                    }
                                                }
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.minimumWidth: lessonPanelVisiblityButton.width
                Layout.maximumWidth: lessonPanelVisiblityButton.height

                Rectangle {
                    color: "#101010"
                    anchors.fill: parent

                }

                Button {
                    property color normalColor: "#808080"
                    property color hoverColor: "#a0a0a0"

                    id: lessonPanelVisiblityButton
                    width: 10
                    height: 60
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    background: Rectangle {
                        color: lessonPanelVisiblityButton.hovered?
                                   lessonPanelVisiblityButton.hoverColor :
                                   lessonPanelVisiblityButton.normalColor
                    }

                    contentItem: Image {
                        rotation: lessonPanel.hidden? 0 : 180
                        anchors.fill: parent
                        source: "Button icons/arrow.png"
                        fillMode: Image.PreserveAspectFit
                    }

                    onClicked: {
                        lessonPanel.hidden = !lessonPanel.hidden
                    }
                }
            }
        }

        Item {
            id: contentPanel
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                color: "#cfcfcf"
                anchors.fill: parent
            }

            Item {
                anchors.bottomMargin: parent.height - optionsPanel.anchors.topMargin
                anchors.fill: parent

                Loader {
                    id: mainScreenLoader
                    focus: true
                    anchors.fill: parent
                    sourceComponent: welcomeScreen

                    onLoaded: {
                        forceActiveFocus()
                    }
                    onFocusChanged: console.log("focus changed to: " + focus)

                }

                Component {
                    id: openGLView
                    TutorialOpenGLView {
                        optionsReference: options
                    }
                }

                Component {
                    id: welcomeScreen
                    WelcomeScreen {
                        welcomeScreenUrl: "file:///" + currentPath + "/Assets/welcomeScreen/welcomeScreen.html"
                    }
                }
            }

            ColumnLayout {
                property bool hidden: false
                property int minPanelHeight: 50
                property int panelHeight: Math.min(mainWindow.height, 450)

                id: optionsPanel
                visible: false
                anchors.topMargin: optionsPanel.visible? (Math.max(optionsPanel.hidden?
                                       mainWindow.height - optionsPanelVisiblityButton.height :
                                       mainWindow.height - optionsPanel.panelHeight, 0)) : mainWindow.height
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillWidth: true
                    Layout.minimumHeight: optionsPanelVisiblityButton.height
                    Layout.maximumHeight: optionsPanelVisiblityButton.height

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: (!optionsPanel.hidden)? Qt.SplitVCursor : Qt.ArrowCursor
                        drag {
                            target: parent.parent
                            axis: Drag.YAxis
                        }

                        onClicked: {
                            console.log(optionsPanel.x.toString() + " " + optionsPanel.y.toString() +
                                        " " + optionsPanel.height.toString())
                        }

                        onMouseYChanged: {
                            if (drag.active && !optionsPanel.hidden) {
                                optionsPanel.panelHeight -= mouseY;

                                if (optionsPanel.panelHeight < optionsPanel.minPanelHeight)
                                {
                                    optionsPanel.panelHeight = optionsPanel.minPanelHeight
                                }
                                else if (optionsPanel.panelHeight > mainWindow.height)
                                {
                                    optionsPanel.panelHeight = mainWindow.height
                                }
                            }
                        }
                    }

                    Rectangle {
                        color: "#101010"
                        anchors.fill: parent
                    }

                    Button {
                        property color normalColor: "#808080"
                        property color hoverColor: "#a0a0a0"

                        id: optionsPanelVisiblityButton
                        height: 10
                        width: 60
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter

                        background: Rectangle {
                            color: optionsPanelVisiblityButton.hovered?
                                       optionsPanelVisiblityButton.hoverColor :
                                       optionsPanelVisiblityButton.normalColor
                        }

                        contentItem: Image {
                            rotation: optionsPanel.hidden? -90 : 90
                            anchors.fill: parent
                            source: "Button icons/arrow.png"
                            fillMode: Image.PreserveAspectFit
                        }

                        onClicked: {
                            optionsPanel.hidden = !optionsPanel.hidden
                        }
                    }
                }

                GenericOptions {
                    id: options
                    visible: !optionsPanel.hidden
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
    }
}
