import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import QtMultimedia 5.0

Item {
    id: container
    property MediaPlayer source: MediaPlayer {}

    Image {
        id: button
        width: 150
        source: "Alarm-Pause-icon.png"
        fillMode: Image.PreserveAspectFit
        opacity: 0.8

        anchors {
            horizontalCenter: progressBar.horizontalCenter
            top: parent.top
        }

    }

    MouseArea {
        anchors.fill: button
        onClicked: {
            if (source.playbackState == MediaPlayer.PlayingState) {
                source.pause()
            }
            else {
                source.play()
            }
        }
    }


    ProgressBar {
        id: progressBar
        width: parent.width - (anchors.leftMargin + anchors.rightMargin)
        height: 20
        radius: 10

        value: source.position
        maximum: source.duration

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
