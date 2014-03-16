import QtQuick 2.0
import QtMultimedia 5.0

Video {
    id: video
    width : 1800
    height : 1600
    source: "/home/vishesh/Videos/True.Detective.S01E01.HDTV.x264-2HD.mp4"

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
    Keys.onLeftPressed: video.seek(video.position - 5000)
    Keys.onRightPressed: video.seek(video.position + 5000)
}
