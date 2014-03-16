import QtQuick 2.0
import QtMultimedia 5.0

Item {
    id: video
    property alias source: mediaPlayer.source

    MediaPlayer {
        id: mediaPlayer
        source: "/home/vishesh/Videos/True.Detective.S01E01.HDTV.x264-2HD.mp4"

        onPositionChanged: {
            progressBar.value = position
            progressBar.maximum = duration
        }
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        source: mediaPlayer
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (mediaPlayer.playbackState == MediaPlayer.PlayingState) {
                mediaPlayer.pause()
            }
            else {
                mediaPlayer.play()
            }
        }
    }

    focus: true
    Keys.onSpacePressed: mediaPlayer.playbackState == MediaPlayer.PlayingState ? mediaPlayer.pause() : mediaPlayer.play()

    Keys.onPressed: {
        var amount = 0;
        if (event.modifiers & Qt.ControlModifier) {
            amount = 60000
        }
        else if (event.modifiers & Qt.AltModifier) {
            amount = 10000
        }
        else if (event.modifiers & Qt.ShiftModifier) {
            amount = 3000
        }

        if (event.key == Qt.Key_Left) {
            mediaPlayer.seek(mediaPlayer.position - amount)
        }
        else if (event.key == Qt.Key_Right) {
            mediaPlayer.seek(mediaPlayer.position + amount)
        }
    }

    function play() {
        mediaPlayer.play()
    }

    function stop() {
        mediaPlayer.stop()
    }

    function pause() {
        mediaPlayer.pause()
    }

    ProgressBar {
        id: progressBar
        width: parent.width - (anchors.leftMargin + anchors.rightMargin)
        height: 20
        radius: 10

        y: parent.y + parent.height * 0.85
        anchors {
            horizontalCenter: parent.horizontalCenter

            leftMargin: 150
            rightMargin: 150
        }
    }
}
