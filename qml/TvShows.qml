import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.1 as QtControls

import org.kde.jungle 0.1 as Jungle

Item {
    id: topElem
    signal play (string url)

    function reset() {
        tvshows.visible = true
        tvepisodes.visible = false
    }

    GridView {
        id: tvshows
        model: Jungle.TvShowsModel {}
        delegate: MovieDelegate {
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    epModel.showId = showId
                    tvshows.visible = false
                    tvepisodes.visible = true
                }
            }
        }

        cellWidth: 400
        cellHeight: 600

        anchors.fill: parent
    }

    GridView {
        id: tvepisodes
        visible: false
        model: Jungle.TvEpisodeModel {
            id: epModel
        }

        delegate: EpisodeDelegate {
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    topElem.play(url)
                }
            }
        }

        cellWidth: 400
        cellHeight: 350

        anchors.fill: parent
    }
}
