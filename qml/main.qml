import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.1

ApplicationWindow {
    id: applicationWindow
    color: "black"
    title: "Jungle"

    width: 1800
    height: 1600

    visible: true

    Item {
        id: mainItem
        anchors.fill: parent

        Sidebar {
            id: sidebar

            width: 400
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom

                topMargin: 50
                leftMargin: 20
                rightMargin: 20
            }

            onMoviesActivated: {
                movies.visible = true
                tvshows.visible = false
            }

            onTvShowsActivated: {
                movies.visible = false
                tvshows.visible = true
                tvshows.reset()
            }
        }

        Movies {
            id: movies
            anchors.left: sidebar.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onPlay: {
                videoPlayer.source = url
                videoPlayer.visible = true
                mainItem.visible = false

                videoPlayer.play()
            }
        }

        TvShows {
            id: tvshows
            visible: false

            anchors.left: sidebar.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onPlay: {
                videoPlayer.source = url
                videoPlayer.visible = true
                mainItem.visible = false

                videoPlayer.play()
            }
        }
    }

    VideoPlayer {
        id: videoPlayer
        visible: false

        anchors.fill: parent

        Keys.onEscapePressed: {
            videoPlayer.stop()
            videoPlayer.visible = false
            mainItem.visible = true
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        onDoubleClicked: toggleFullScreen()
    }

    function toggleFullScreen() {
        if (applicationWindow.visibility != Window.FullScreen) {
            applicationWindow.visibility = Window.FullScreen
        } else {
            applicationWindow.visibility = Window.AutomaticVisibility
        }
    }
}
