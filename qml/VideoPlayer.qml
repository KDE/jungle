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
        y: parent.y + parent.height * 0.80

        Behavior on opacity {
            NumberAnimation { duration: 500 }
        }
    }

    Timer {
        id: toolBoxHideTimer
        interval: 3000
        repeat: false

        onTriggered: {
            toolBox.opacity = 0.0
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onPositionChanged: {
            toolBox.opacity = 1.0
            toolBoxHideTimer.start()
        }

        propagateComposedEvents: true
    }
}
