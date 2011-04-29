import QtQuick 1.0

Item {
    id: contactView

    property QtObject contact

    Avatar {
        id: avatarImage
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 16
        anchors.rightMargin: 16

        source: contact.avatar
    }

    Text {
        anchors.top: avatarImage.bottom
        anchors.topMargin: 4
        anchors.left: avatarImage.left
        anchors.right: avatarImage.right
        font.pixelSize: 11
        elide: Text.ElideRight
        text: "Click to change avatar"
        horizontalAlignment: Text.AlignHCenter
        color: "#b2b2b2"
    }

    Row {
        x: 16
        y: 16
        spacing: 20

        Column {
            spacing: 20

            Repeater {
                id: rpt
                model: ["First Name:", "Last Name:", "Phone:"]

                delegate: Text {
                    color: "#afafaf"
                    font.pixelSize: 26
                    text: rpt.model[index]
                    anchors.right: parent.right
                }
            }
        }

        Column {
            spacing: 20

            Repeater {
                id: rpt2
                model: ["firstName", "lastName", "phoneNumber"]

                delegate: TextInput {
                    font.pixelSize: 26
                    smooth: true
                    selectByMouse: true

                    text: contact[rpt2.model[index]]

                    onAccepted: {
                        contact[rpt2.model[index]] = text
                    }
                }
            }
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        Text {
            text: "Delete"

            MouseArea {
                anchors.fill: parent
                onClicked: contact.remove()
            }
        }

        Text {
            text: "Save"

            MouseArea {
                anchors.fill: parent
                onClicked: contact.save()
            }
        }

        Text {
            text: "New"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    /* This is ugly, we shouldn't have to touch contactsModel here. */
                    contact = contactsModel.blankContact()
                }
            }
        }
    }
}
