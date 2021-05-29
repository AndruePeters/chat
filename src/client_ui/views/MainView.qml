
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

    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin
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
            MenuItem { text: "Preferences" }
        }   
    }
            
    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: appWindow.margin

        GroupBox {
            id: serverSettings
            title: "Server Settings"
            Layout.fillWidth: true
            Layout.minimumWidth: serverGridLayout.Layout.minimumWidth + 30

            GridLayout {
                id: serverGridLayout
                rows: 2
                flow: GridLayout.TopToBottom
                anchors.fill: parent

                Label { text: "Address" }
                Label { text: "Port" }

                TextField {
                    id:addressBox
                    Layout.fillWidth: true
                }
                TextField {
                    id:portBox 
                    Layout.fillWidth: true
                }
              }
        }

        GroupBox {
            id: messageBox
            title: "Message Settings"
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.minimumWidth: messageBoxGrid.Layout.minimumWidth + 30


            GridLayout {
                id: messageBoxGrid
                rows: 2
                flow: GridLayout.TopToBottom
                anchors.fill: parent

                Label { text: "From"}
                Label { text: "To" }

                TextField { Layout.fillWidth: true }
                TextField { Layout.fillWidth: true }
            }
        }

        GroupBox {
            title: "Message Box"
            Layout.fillWidth: true
            Layout.fillHeight: true
            TextArea {
                id: messageContent
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
            }
            Button {
                text: "submit message"
                anchors.top: messageContent.bottom
            }
        }
    }
}