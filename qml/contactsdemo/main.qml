import QtQuick 1.0

Item {
    width: 1000
    height: 450

    ContactsList {
        id: contactsList
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: 300
        focus: true

        onCurrentContactChanged: {
            contactView.contact = currentContact
        }
    }

    ContactView {
        id: contactView
        anchors.left: contactsList.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
}
