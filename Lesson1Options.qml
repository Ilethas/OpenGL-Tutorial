import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Flow {
    padding: 15
    property var currentRenderer: undefined
    function connectToRenderer(renderer) {
        currentRenderer = renderer
        lightsControl.connectToRenderer(renderer)

        updateActions()
        renderer.onCurrentInterpolationTypeChanged.connect(updateActions)
        shininess.placeholderText = Qt.binding(function() { return renderer.shininess })
        renderer.currentInterpolationType = Qt.binding(function() {
            var type = interpolationTypeGroup.checkedAction.text
            switch(type)
            {
                case "Flat": return 0;
                case "Smooth": return 1;
                case "Gouraud": return 2;
            }
        })
        renderer.currentShadingType = Qt.binding(function() {
            var type = shadingTypeGroup.checkedAction.text
            switch(type)
            {
                case "Phong": return 0;
                case "Blinn Phong": return 1;
            }
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

    ActionGroup {
        id: interpolationTypeGroup
        Action {
            id: flat
            checked: true
            checkable: true
            text: "Flat"
        }
        Action {
            id: smooth
            checkable: true
            text: "Smooth"
        }
        Action {
            id: gouraud
            checkable: true
            text: "Gouraud"
        }
    }
    ActionGroup {
        id: shadingTypeGroup
        Action {
            id: phong
            checked: true
            checkable: true
            text: "Phong"
        }
        Action {
            id: blinnPhong
            checked: true
            checkable: true
            text: "Blinn Phong"
        }
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
                                text: "Interpolation type"
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                padding: 10

                                background: Rectangle {
                                    color: "#000000"
                                    radius: 15
                                    anchors.fill: parent

                                }
                            }

                            RadioButton {
                                checked: flat.checked
                                onClicked: {
                                    flat.checked = true
                                }
                            }
                            Text {
                                text: flat.text
                                font.pointSize: 10
                                horizontalAlignment: Text.AlignHCenter
                            }
                            RadioButton {
                                checked: smooth.checked
                                onClicked: {
                                    smooth.checked = true
                                }
                            }
                            Text {
                                text: smooth.text
                                font.pointSize: 10
                                horizontalAlignment: Text.AlignHCenter
                            }
                            RadioButton {
                                checked: gouraud.checked
                                onClicked: {
                                    gouraud.checked = true
                                }
                            }
                            Text {
                                text: gouraud.text
                                font.pointSize: 10
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }

                    GroupBox {
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        GridLayout {
                            columns: 2
                            rows: 4
                            Label {
                                color: "#ffffff"
                                Layout.columnSpan: 2
                                font.pointSize: 12
                                text: "Shading type"
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                padding: 10

                                background: Rectangle {
                                    color: "#000000"
                                    radius: 15
                                    anchors.fill: parent

                                }
                            }

                            RadioButton {
                                checked: phong.checked
                                onClicked: {
                                    phong.checked = true
                                }
                            }
                            Text {
                                text: phong.text
                                font.pointSize: 10
                                horizontalAlignment: Text.AlignHCenter
                            }
                            RadioButton {
                                checked: blinnPhong.checked
                                onClicked: {
                                    blinnPhong.checked = true
                                }
                            }
                            Text {
                                text: blinnPhong.text
                                font.pointSize: 10
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Text {
                                text: "Shininess"
                                font.pointSize: 10
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: shininess
                                Layout.maximumWidth: 80
                                Layout.minimumWidth: 80
                                maximumLength: 10
                                font.pointSize: 8
                                validator: DoubleValidator {
                                    bottom: 1
                                }
                                selectByMouse: true
                                font.family: "Arial"

                                onEditingFinished: {
                                    currentRenderer.shininess = Number(shininess.text.replace(',', '.'))
                                    text = ""
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
