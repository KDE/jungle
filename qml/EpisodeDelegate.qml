import QtQuick 2.1
import QtQuick.Layouts 1.0

ColumnLayout {
    Image {
        id: img
        source: cover

        fillMode: Image.PreserveAspectFit
    }

    Text {
        text: display
        color: "white"

        Layout.fillWidth: true
        elide: Text.ElideRight

        font.pointSize: 8
        font.bold: true
    }

    Text {
        text: "Episode " + episodeNumber
        color: "white"
        opacity: 0.5

        Layout.fillWidth: true
        elide: Text.ElideRight

        font.pointSize: 8
    }

    Text {
        text: date.toDateString()
        color: "white"
        opacity: 0.5

        Layout.fillWidth: true
        elide: Text.ElideRight

        font.pointSize: 8
    }
}
