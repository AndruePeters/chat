import QtQuick 2.15

import QtQuick.Controls.Material


Item {
    id: messageCard

    property int marginSize: parent.width * 0.05


    property int maximumWidth: parent.width * 0.4 + marginSize
    property int maximumHeight: messageText.implicitHeight + 20


    property string text
    property int role

    /// shift the margins of the entire message card
    anchors.margins: marginSize

    /// want width to be a quarter of the parent width max
    width: (maximumWidth > messageText.implicitWidth) ? maximumWidth : messageText.implicitWidth;
    height: messageText.implicitHeight + 5

    enum Role {
        Sender,
        Receiver
    }

    Rectangle {
        anchors.fill: parent
        color: Material.color(Material.Grey)
    }
    
    Text {
        id: messageText
        text: parent.text
        anchors {
            leftMargin: 5
            rightMargin: 5
            topMargin: 5
            bottomMargin: 5
        }
    }


    function setAnchors() {
        if (role == MessageCard.Role.Sender) {
            this.anchors.left = parent.left;
        } else if (role == MessageCard.Role.Receiver) {
            this.anchors.right =  parent.right;
        }
    }

    Component.onCompleted:  setAnchors()
    
}