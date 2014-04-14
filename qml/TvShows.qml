import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import org.kde.jungle 0.1 as Jungle

GridView {
    model: Jungle.TvShowsModel {}
    delegate: MovieDelegate {}

    cellWidth: 400
    cellHeight: 600
}
