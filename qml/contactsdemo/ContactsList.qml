import QtQuick 1.0

ListView {
    property QtObject currentContact

    property list<QtObject> fakeData: [
        QtObject { property string firstName: "Robin"; property string lastName: "Burchell";
            property string phone: "+1 888 501 4931"; property string avatar: "usertile17.bmp" },
        QtObject { property string firstName: "John"; property string lastName: "Brooks";
            property string phone: "+1 000 000 0000"; property string avatar: "usertile21.bmp" },
        QtObject { property string firstName: "Robin"; property string lastName: "Burchell";
            property string phone: "+1 888 501 4931"; property string avatar: "usertile43.bmp" }
    ]
    model: contactsModel

    delegate: Qt.createComponent("ContactsListItem.qml")
    spacing: 5

    Rectangle {
        anchors.fill: parent
        z: -2
        color: "#f1f1f1"
    }

    Image {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        z: -1

        source: "list-border.png"
        fillMode: Image.TileVertically
    }

    highlight: Rectangle {
        color: "white"

        Image {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.top

            source: "selected-border.png"
            fillMode: Image.TileHorizontally
        }

        Image {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.bottom

            source: "selected-border-2.png"
            fillMode: Image.TileHorizontally
        }
    }
    highlightMoveDuration: 350

    onCurrentIndexChanged: {
        if (currentIndex < 0)
            currentContact = null
        else
            currentContact = contactsModel.contactFor(currentIndex)
    }
}
