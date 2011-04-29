import QtQuick 1.0
import Qt.labs.folderlistmodel 1.0

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

    Grid {
        id: detailArea
        anchors.left: parent.left
        anchors.right: avatarImage.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 16
        spacing: 20
        flow: Grid.TopToBottom
        columns: 2

        Repeater {
            id: rpt
            model: ["First Name:", "Last Name:", "Phone:"]

            property int cellWidth: 0

            delegate: Text {
                color: "#afafaf"
                font.pixelSize: 26
                text: rpt.model[index]
                horizontalAlignment: Qt.AlignRight
                width: rpt.cellWidth

                /* Required for horizontal alignment to succeed */
                onPaintedSizeChanged: rpt.cellWidth = Math.max(rpt.cellWidth, paintedWidth)
            }
        }

        Repeater {
            id: rpt2
            model: ["firstName", "lastName", "phoneNumber"]

            delegate: TextInput {
                font.pixelSize: 26
                smooth: true
                selectByMouse: true
                width: detailArea.width - x

                text: contact[rpt2.model[index]]

                onTextChanged: {
                    if (contact[rpt2.model[index]] != text)
                        contact[rpt2.model[index]] = text
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

        Text {
            // TODO: this is a horrible, horrible hack
            text: "Set Avatar"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    avatarSelector.visible = true
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        clip: true
        id: avatarSelector
        visible: false

        ListView {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            model: FolderListModel {
                folder: "."
            }
            delegate: Item {
                width: img.width
                height: img.height
                Image {
                    id: img
                    source: filePath

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            contactView.contact.avatar = filePath
                            avatarSelector.visible = false
                        }
                    }
                }
            }
        }
    }


}
