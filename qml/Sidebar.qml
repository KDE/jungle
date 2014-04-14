import QtQuick 2.1
import QtQuick.Layouts 1.0

Column {
    signal moviesActivated
    signal tvShowsActivated

    Text {
        text: "Movies"
        color: "white"
        opacity: 0.5

        font.pointSize: 18
        MouseArea {
            anchors.fill: parent
            onClicked: moviesActivated()
        }
    }
    Text {
        text: "TV Shows"
        color: "white"
        opacity: 0.5

        font.pointSize: 18
        MouseArea {
            anchors.fill: parent
            onClicked: tvShowsActivated()
        }
    }
}
