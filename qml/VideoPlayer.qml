import QtQuick 2.0
import QtMultimedia 5.0

Item {
    id: video
    property alias source: mediaPlayer.source

    MediaPlayer {
        id: mediaPlayer
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        source: mediaPlayer
    }

    Keys.onSpacePressed: mediaPlayer.playbackState == MediaPlayer.PlayingState ? mediaPlayer.pause() : mediaPlayer.play()

    Keys.onPressed: {
        var amount = 3000;
        if (event.modifiers & Qt.ControlModifier) {
            amount = 60000
        }
        else if (event.modifiers & Qt.AltModifier) {
            amount = 10000
        }

        if (event.key == Qt.Key_Left) {
            mediaPlayer.seek(mediaPlayer.position - amount)
        }
        else if (event.key == Qt.Key_Right) {
            mediaPlayer.seek(mediaPlayer.position + amount)
        }

        var vol = mediaPlayer.volume
        if (event.key == Qt.Key_Up) {
            vol += 0.05
        } else if (event.key == Qt.Key_Down) {
            vol -= 0.05
        }
        vol = Math.max(0.0, Math.min(1.0, vol));
        mediaPlayer.volume = vol

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

    // The VideoPlayer should always have a black background
    Rectangle {
        anchors.fill: parent
        color: "black"
        z: -1
    }
}

