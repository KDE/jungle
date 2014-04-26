import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import QtMultimedia 5.0
import QtQuick.Controls 1.1 as QtControls
import org.kde.kcoreaddons 1.0 as KCoreAddons

// TODO: Some important things to implement - Nice progress bar
// Simple play / pause buttons
// Volume Control
// Show duration
Item {
    id: container

    property MediaPlayer source: MediaPlayer {}
    height: childrenRect.height

    Rectangle {
        color: "black"
        anchors.fill: parent
        z: -10
    }

    ProgressBar {
        id: progressBar
        width: parent.width - (anchors.leftMargin + anchors.rightMargin)
        height: 20

        value: source.position
        maximum: source.duration

        backgroundShown: true

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        onClicked: {
            source.seek(source.duration * percent)
        }
    }

    RowLayout {
        id: rowLayout
        anchors.top: progressBar.bottom

        QtControls.Button {
            id: button

            iconName: source.playbackState == MediaPlayer.PlayingState ? "media-playback-pause" : "media-playback-start"
            opacity: 0.8

            onClicked: {
                if (source.playbackState == MediaPlayer.PlayingState) {
                    source.pause()
                } else {
                    source.play()
                }
            }
        }

        KCoreAddons.Formats {
            id: formats
        }

        Text {
            text: formats.formatDuration(source.position)
            color: "white"
        }

        Text {
            text: " / "
            color: "white"
        }

        Text {
            text: formats.formatDuration(source.duration)
            color: "white"
        }
    }

}
