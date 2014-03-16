import QtQuick 2.1
import QtQuick.Layouts 1.0

Rectangle {
    color: "black"

    width: 1800
    height: 1600

    Shows {
        id: shows

        anchors.fill: parent

        onPlay: {
            videoPlayer.source = url
            videoPlayer.visible = true
            shows.visible = false

            videoPlayer.play()
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
        shows.visible = true
    }
}
