import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import org.kde.jungle 0.1 as Jungle

GridView {
    id: gridView
    signal play (string url)

    model: Jungle.MoviesModel {}

    delegate: MovieDelegate {
        MouseArea {
            anchors.fill: parent
            onClicked: {
                gridView.play(url)
            }
        }
    }

    cellWidth: 400
    cellHeight: 600
}
