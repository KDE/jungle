import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import org.kde.jungle 0.1 as Jungle

GridView {
    id: gridView
    signal play (string url)

    model: Jungle.SortModel {
        sourceModel: Jungle.VideosModel {}
    }

    delegate: MovieDelegate {
        MouseArea {
            anchors.fill: parent
            onClicked: gridView.play(url)
        }

        Keys.onReturnPressed: gridView.play(url)
        Keys.onSpacePressed: gridView.play(url)
    }

    highlightMoveDuration: 0
    highlight: Rectangle {
        color: "#2874CC"
    }
    boundsBehavior: Flickable.StopAtBounds

    cellWidth: 400
    cellHeight: 600
}
