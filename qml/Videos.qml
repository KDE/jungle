import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import org.kde.jungle 0.1 as Jungle
import org.kde.plasma.components 2.0 as PlasmaComponents

GridView {
    id: gridView
    signal play (string url)

    model: Jungle.SortModel {
        sourceModel: Jungle.VideosModel {}
    }

    delegate: MovieDelegate {
        subtext: model.date
        MouseArea {
            anchors.fill: parent
            onClicked: gridView.play(url)
        }

        Keys.onReturnPressed: gridView.play(url)
        Keys.onSpacePressed: gridView.play(url)
    }

    highlightMoveDuration: 0
    highlight: PlasmaComponents.Highlight {}
    boundsBehavior: Flickable.StopAtBounds

    cellWidth: 400
    cellHeight: 600
}
