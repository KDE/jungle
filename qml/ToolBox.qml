import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import QtMultimedia 5.0
import QtQuick.Controls 1.1 as QtControls

// TODO: Some important things to implement - Nice progress bar
// Simple play / pause buttons
// Volume Control
// Show duration
Item {
    id: container
    property MediaPlayer source: MediaPlayer {}

    QtControls.Button {
        id: button

        width: 150
        height: 150

        iconName: source.playbackState == MediaPlayer.PlayingState ? "media-playback-pause" : "media-playback-start"
        opacity: 0.8

        anchors {
            horizontalCenter: progressBar.horizontalCenter
            top: parent.top
        }

        onClicked: {
            if (source.playbackState == MediaPlayer.PlayingState) {
                source.pause()
            } else {
                source.play()
            }
        }
    }

    ProgressBar {
        id: progressBar
        width: parent.width - (anchors.leftMargin + anchors.rightMargin)
        height: 20

        value: source.position
        maximum: source.duration

        backgroundShown: true

        anchors {
            top: button.bottom
            horizontalCenter: parent.horizontalCenter

            leftMargin: 150
            rightMargin: 150
            topMargin: 30
        }

        onClicked: {
            source.seek(source.duration * percent)
        }
    }
}
