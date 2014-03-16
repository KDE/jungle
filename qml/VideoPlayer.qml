import QtQuick 2.0
import QtMultimedia 5.0

Video {
    id: video

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (video.playbackState == MediaPlayer.PlayingState) {
                video.pause()
            }
            else {
                video.play()
            }
        }
    }

    focus: true
    Keys.onSpacePressed: video.playbackState == MediaPlayer.PlayingState ? video.pause() : video.play()

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
            video.seek(video.position - amount)
        }
        else if (event.key == Qt.Key_Right) {
            video.seek(video.position + amount)
        }
    }
}
