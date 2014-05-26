import QtQuick 2.1
import QtQuick.Layouts 1.0
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

ColumnLayout {
    Image {
        id: img
        source: cover

        fillMode: Image.PreserveAspectFit

        KQuickControlsAddons.QIconItem {
            icon: "checkmark"
            visible: watched
            smooth: true

            height: 20
            width: 20

            anchors.right: img.right
            anchors.top: img.top
        }
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
        text: date.getFullYear()
        color: "white"
        opacity: 0.5

        Layout.fillWidth: true
        elide: Text.ElideRight

        font.pointSize: 8
    }

}
