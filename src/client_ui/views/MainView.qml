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

    minimumWidth: Screen.desktopAvailableWidth  / 4
    minimumHeight: Screen.desktopAvailableHeight / 4

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
            Rectangle {
                color: "red"
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

        GridLayout {
            id: messageGrid
            columns: 2
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            TextArea {
                id: sendMessageData
                Layout.fillWidth: true
                Layout.preferredWidth: parent.width / 3
                text: "Enter message here."
            }

            TextArea {
                id: allMessagesView
                Layout.fillWidth: true
                Layout.preferredWidth: parent.width / 2
                readOnly: true
            }
        }

        Button {
            text: "Send"
            anchors.top: messageGrid.bottom
        }
    }
}