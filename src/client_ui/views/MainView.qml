// The main view for the chat client

import QtQuick 2.15
import QtQuick.Window 
import QtQuick.Controls
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material

ApplicationWindow {
    id: appWindow
    Material.theme: Material.Dark
    
    visible: true
    title: "Chat Server"
    property int margin: 11

    minimumWidth: Screen.desktopAvailableWidth  / 2
    minimumHeight: Screen.desktopAvailableHeight / 1.5

    property SettingsView settingsView: SettingsView {objectName: "settingsView" }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }

        Menu {
            title: "Edit"
            MenuItem { 
                text: "Preferences" 
                onTriggered: settingsView.open()
            }
        }   
    }
            
    Column {
        id: sideBar
        width: parent.width / 5
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }

        Item {
            implicitHeight: parent.height
            implicitWidth: parent.width
            Rectangle {
                color: "red"
                anchors.fill: parent
            }
        }

    }

    GroupBox {
        id: messageGroupBox
        title: "Message Box"
        Layout.fillWidth: true
        Layout.fillHeight: true
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
            left: sideBar.right
        }
        

        Column {
            id: messageGrid
            anchors.fill: parent

            Item {
                Layout.fillWidth: true  
                Layout.preferredWidth: parent.width
                height: parent.height * 0.80
                width: parent.width

                Rectangle {
                    color: Material.color(Material.Grey, Material.Shade500)
                    //anchors.fill: parent
                }

                Column {
                    Layout.fillHeight: true
                    width: parent.width
                    spacing: 10

                    MessageCard {
                        text: "first message"
                        role: MessageCard.Role.Sender
                    }

                    MessageCard {
                        text: "second message"
                        role: MessageCard.Role.Receiver
                    }

                    MessageCard {
                        text: "This is a really long message.\n\nMultiple newlines............................................................................."
                        role: MessageCard.Role.Sender
                    }

                    Rectangle {
                        width: parent.width
                        color: "pink"
                        height: 40
                    }
                }


            }


            Item {
                Layout.fillWidth: true
                height: parent.height * 0.2
                width: parent.width * 1

                anchors.horizontalCenter: parent.horizontalCenter

                TextArea {
                    id: sendMessageData
                    width: parent.width
                    text: "Enter message here."
                }

                Button {
                    anchors.top: sendMessageData.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Send"
                }
            }
        }
    }
}