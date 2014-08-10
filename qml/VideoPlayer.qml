import QtQuick 2.0
import QtMultimedia 5.0

Item {
    id: video
    property alias source: mediaPlayer.source

    MediaPlayer {
        id: mediaPlayer
        source: "/home/vishesh/Videos/True.Detective.S01E01.HDTV.x264-2HD.mp4"
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        source: mediaPlayer
    }

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

        if (event.modifiers & Qt.ControlModifier) {
            if (event.key == Qt.Key_Up) {
                if (mediaPlayer.volume < 1.0)
                    mediaPlayer.volume += 0.05
            }
            else if (event.key == Qt.Key_Down) {
                if (mediaPlayer.volume > 0)
                    mediaPlayer.volume -= 0.05
            }
        }
       toolBox.opacity = 1.0
       toolBoxHideTimer.start()
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

    ToolBox {
        id: toolBox
        source: mediaPlayer

        width: parent.width
        anchors.bottom: parent.bottom

        Behavior on opacity {
            NumberAnimation { duration: 500 }
        }
    }

    Timer {
        id: toolBoxHideTimer
        interval: 1000
        repeat: false

        onTriggered: {
            toolBox.opacity = 0.0
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.NoButton

        onPositionChanged: {
            toolBox.opacity = 1.0

            var inToolBox = false
            if (toolBox.x <= mouse.x && mouse.x <= toolBox.x + toolBox.width &&
                toolBox.y <= mouse.y && mouse.y <= toolBox.y + toolBox.height)
                inToolBox = true;

            if (inToolBox)
                toolBoxHideTimer.stop()
            else
                toolBoxHideTimer.restart()
        }
    }
}

