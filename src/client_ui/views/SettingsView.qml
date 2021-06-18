/// The preferences window for the chat client

import QtQuick 2.15
import QtQuick.Window 
import QtQuick.Controls
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material

Popup {
    id: settingsView

    signal saveSettings()    

    anchors.centerIn: parent     
    modal: true  
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

        RowLayout {
            Button { 
                text: "cancel"
                onClicked: settingsView.close()
            }

            Button { 
                text: "save" 
                onClicked: saveSettings()
            }
        }
    }
}
