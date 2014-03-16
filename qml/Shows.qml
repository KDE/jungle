import QtQuick 2.1
import QtQuick.Layouts 1.0

GridView {
    id: gridView
    signal play (string url)

    width: 500
    height: 500

    model: ListModel {
        ListElement {
            cover: "/home/vishesh/lost.jpg"
            title: "Lost"
            url: "/home/vishesh/The.Big.Bang.Theory.S07E16.HDTV.x264-LOL.mp4"
        }
        ListElement {
            cover: "/home/vishesh/lost.jpg"
            title: "Lost"
            url: "/home/vishesh/The.Big.Bang.Theory.S07E16.HDTV.x264-LOL.mp4"
        }
        ListElement {
            cover: "/home/vishesh/lost.jpg"
            title: "Lost"
            url: "/home/vishesh/The.Big.Bang.Theory.S07E16.HDTV.x264-LOL.mp4"
        }
    }

    delegate: ColumnLayout {
        Image {
            source: cover
            width: cellWidth
            height: cellHeight
        }
        Text {
            text: title
            horizontalAlignment: Text.AlignHCenter

            font.pointSize: 22
        }

        anchors {
            leftMargin: 50
            rightMargin: 50
        }

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
