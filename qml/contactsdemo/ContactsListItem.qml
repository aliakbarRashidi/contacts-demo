import QtQuick 1.0

Item {
    width: ListView.view.width
    height: 70
    id: "delegateInstance"

    Avatar {
        id: avatarImage
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        sourceSize: Qt.size(60, 60)
        width: 60

        source: "image://contactavatar/" + contactId  + "/" + avatarSerial
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

            text: phoneNumber
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: ListView.view.currentIndex = index
    }

    ListView.onRemove: SequentialAnimation {
        PropertyAction { target: delegateInstance; property: "ListView.delayRemove"; value: true }
        ParallelAnimation {
            NumberAnimation { target: delegateInstance; property: "height"; to: 0; duration: 250; easing.type: Easing.InOutQuad }
            NumberAnimation { target: delegateInstance; property: "opacity"; to: 0; duration: 200; easing.type: Easing.InOutQuad }
        }
        PropertyAction { target: delegateInstance; property: "ListView.delayRemove"; value: false }
    }
}
