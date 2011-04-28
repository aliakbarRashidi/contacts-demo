import QtQuick 1.0

Image {
    fillMode: Image.Stretch

    BorderImage {
        anchors.fill: parent
        anchors.margins: -2

        source: "avatar-border.png"
        asynchronous: false
        smooth: true

        border.left: 3
        border.top: 3
        border.right: 3
        border.bottom: 3
    }
}
