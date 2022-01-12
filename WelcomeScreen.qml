import QtQuick 2.0
import QtWebEngine 1.5

WebEngineView {
    property url welcomeScreenUrl: ""

    id: welcomeScreen
    anchors.fill: parent
    url: welcomeScreenUrl
}
