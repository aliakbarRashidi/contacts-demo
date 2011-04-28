import QtQuick 1.0

Item {
    width: ListView.view.width
    height: 70

    Avatar {
        id: avatarImage
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        sourceSize: Qt.size(60, 60)
        width: 60

        source: avatar
    }

    Column {
        anchors.left: avatarImage.right
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 12

        Text {
            id: nameText
            width: parent.width
            font.pixelSize: 16
            elide: Text.ElideRight

            text: firstName + " " + lastName
        }

        Text {
            x: 2
            font.pixelSize: 10
            width: parent.width
            elide: Text.ElideRight
            color: "#7e7e7e"

            text: phone
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: ListView.view.currentIndex = index
    }
}
