/*
 * Charge monitor (C) 2014-2015 Kimmo Lindholm
 * LICENSE MIT
 */
import QtQuick 2.0
import Sailfish.Silica 1.0


Page
{
    id: page

    SilicaFlickable
    {
        anchors.fill: parent

        PullDownMenu
        {
            busy: writelog

            MenuItem
            {
                text: "About"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"),
                                          { "version": cmon.version,
                                              "year": "2014",
                                              "name": "Charge monitor",
                                              "imagelocation": "/usr/share/icons/hicolor/86x86/apps/harbour-chargemon.png"} )
            }

            MenuItem
            {
                text: writelog ? "Stop logging" : "Start logging"
                onClicked:
                {
                    writelog = !writelog
                    cmon.setWriteToFile(writelog)
                    if (writelog)
                        messagebox.showMessage("Logging to:\n" + cmon.logFileName)
                }
            }
        }

        Component.onCompleted: scrollToBottom()

        contentHeight: column.height

        Column
        {
            id: column

            width: page.width
            spacing: Theme.paddingSmall
            PageHeader
            {
                title: "Chargemon"
            }

            SectionHeader
            {
                text: "Battery capacity"
            }

            Label
            {
                x: Theme.paddingLarge
                text: cmon.batteryCapacity
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            SectionHeader
            {
                text: "Battery temperature"
            }

            Label
            {
                x: Theme.paddingLarge
                text: cmon.batteryTemperature
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            SectionHeader
            {
                text: "Battery voltage"
            }

            Label
            {
                x: Theme.paddingLarge
                text: cmon.batteryVoltage
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            SectionHeader
            {
                text: "Battery current"
            }

            Label
            {
                x: Theme.paddingLarge
                text: cmon.batteryCurrent
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            SectionHeader
            {
                text: "DC input voltage"
                visible: cmon.dcinVoltage !== "N/A"
            }

            Label
            {
                visible: cmon.dcinVoltage !== "N/A"
                x: Theme.paddingLarge
                text: cmon.dcinVoltage
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            SectionHeader
            {
                text: "USB input voltage"
            }

            Label
            {
                x: Theme.paddingLarge
                text: cmon.usbinVoltage
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }


        }

        Timer
        {
            interval: 500
            running: (applicationActive && page.status === PageStatus.Active) || writelog
            repeat: true
            triggeredOnStart: true
            onTriggered:
            {
                cmon.update()
            }
        }

        Timer
        {
            /* Is this the only way? */
            interval: 500
            running: true
            repeat: false
            onTriggered: pageStack.pushAttached(Qt.resolvedUrl("InfoPage.qml"))
        }
    }

}


