/*
 * Charge monitor (C) 2014-2019 Kimmo Lindholm
 * LICENSE MIT
 */
import QtQuick 2.0
import Sailfish.Silica 1.0


Page
{
    property string name
    property string year
    property string version
    property string imagelocation

    id: page

    SilicaFlickable
    {
        anchors.fill: parent

        contentHeight: column.height

        Column
        {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader
            {
                title: "About " + name
            }
            Label
            {
                x: Theme.paddingLarge
                text: name
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Rectangle
            {
                visible: imagelocation.length > 0
                anchors.horizontalCenter: parent.horizontalCenter
                height: Theme.iconSizeLauncher
                width: Theme.iconSizeLauncher
                color: "transparent"

                Image
                {
                    visible: imagelocation.length > 0
                    source: imagelocation
                    anchors.fill: parent
                }
            }
			
            Label
            {
                x: Theme.paddingLarge
                text: "(C) " + year + " kimmoli"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label
            {
                x: Theme.paddingLarge
                text: "Version: " + version
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}


