import QtQuick 2.1
import QtQuick.Layouts 1.0

Rectangle {
    color: "black"

    width: 1800
    height: 1600

    Item {
        id: mainItem
        anchors.fill: parent

        Sidebar {
            id: sidebar

            width: 400
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom

                topMargin: 50
                leftMargin: 20
                rightMargin: 20
            }
        }

        Shows {
            id: shows

            anchors.left: sidebar.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onPlay: {
                videoPlayer.source = url
                videoPlayer.visible = true
                mainItem.visible = false

                videoPlayer.play()
            }
        }
    }

    VideoPlayer {
        id: videoPlayer
        visible: false

        anchors.fill: parent
    }

    Keys.onEscapePressed: {
        videoPlayer.stop()
        videoPlayer.visible = false
        mainItem.visible = true
    }
}
