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

        contact: contactView.contact
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
                    avatarSelector.state = "visible"
                }
            }
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height
        x: 0
        clip: true
        id: avatarSelector
        state: "hidden"

        GridView {
            anchors.fill: parent
            cellWidth: 150
            cellHeight: 150
            anchors.margins: 20

            model: FolderListModel {
                folder: "../../thumbs/"
            }
            delegate: Item {
                width: GridView.view.cellWidth
                height: GridView.view.cellHeight
                id: avatarSelectorDelegate

                Column {
                    Avatar {
                        id: img
                        source: filePath
                        fillMode: Image.PreserveAspectFit
                        height: 128
                        width: 128

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                contactView.contact.setAvatar(filePath)
                                avatarSelector.state = "hidden"
                            }
                        }
                    }

                    Text {
                        anchors.top: avatarSelectorDelegate.bottom
                        text: fileName
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }


        states: [
            State {
                name: "hidden"
                PropertyChanges {
                    target: avatarSelector
                    y: +parent.height
                }
                PropertyChanges {
                    target: avatarSelector
                    visible: false
                }
            },
            
            State {
                name: "visible"
                PropertyChanges {
                    target: avatarSelector
                    y: 0
                }
                PropertyChanges {
                    target: avatarSelector
                    visible: true
                }
            }
        ]

        transitions: [
            Transition {
                from: "hidden"
                to: "visible"

                SequentialAnimation {
                    PropertyAction {
                        target: avatarSelector
                        property: "visible"
                        value: "true"
                    }

                    PropertyAnimation {
                        target: avatarSelector
                        property: "y"
                        easing.type: Easing.OutBounce
                        duration: 800
                    }
                }
            },

            Transition {
                from: "visible"
                to: "hidden"

                SequentialAnimation {
                    PropertyAnimation {
                        target: avatarSelector;
                        property: "y";
                        easing.type: Easing.OutBack
                        duration: 600
                    }
                    PropertyAction {
                        target: avatarSelector;
                        property: "visible";
                        value: "false"
                    }
                }
            }
        ]
    }


}
