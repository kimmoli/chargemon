
import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground
{

    Image
    {
        source: "charger.png"
        opacity: 0.15
        anchors.centerIn: parent
        width: parent.width
        height: sourceSize.height * width / sourceSize.width
    }


    Column
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 6
        anchors.leftMargin: 8
        width: parent.width - (2 * anchors.leftMargin)

        Label
        {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Theme.fontSizeMedium
            color: Theme.highlightColor
            text: "Charge monitor"
        }

        Rectangle
        {
            height: 3
            width: parent.width
            color: "transparent"
        }

        Row
        {
            width: parent.width

            Label
            {
                width: parent.width/2
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
                anchors.verticalCenter: parent.verticalCenter
                text: "Capacity"
            }

            Label
            {
                width: parent.width/2
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                text: cmon.batteryCapacity
            }
        }

        Row
        {
            width: parent.width

            Label
            {
                width: parent.width/2
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
                anchors.verticalCenter: parent.verticalCenter
                text: "Temperature"
            }
            Label
            {
                width: parent.width/2
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                text: cmon.batteryTemperature
            }
        }

        Row
        {
            width: parent.width

            Label
            {
                width: parent.width/2
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
                anchors.verticalCenter: parent.verticalCenter
                text: "Voltage"
            }
            Label
            {
                width: parent.width/2
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                text: cmon.batteryVoltage
            }
        }

        Row
        {
            width: parent.width

            Label
            {
                width: parent.width/2
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
                anchors.verticalCenter: parent.verticalCenter
                text: "Current"
            }
            Label
            {
                width: parent.width/2
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                text: cmon.batteryCurrent
            }
        }

        Row
        {
            width: parent.width

            Label
            {
                width: parent.width/2
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
                anchors.verticalCenter: parent.verticalCenter
                text: "DC in"
            }
            Label
            {
                width: parent.width/2
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                text: cmon.dcinVoltage
            }
        }

        Row
        {
            width: parent.width

            Label
            {
                width: parent.width/2
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
                anchors.verticalCenter: parent.verticalCenter
                text: "USB in"
            }
            Label
            {
                width: parent.width/2
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                text: cmon.usbinVoltage
            }
        }
    }

    CoverActionList
    {

        CoverAction
        {
            iconSource: coverActionLeftIcon
            onTriggered: coverActionLeft()
        }

        CoverAction
        {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: coverActionRight()
        }
    }
}

