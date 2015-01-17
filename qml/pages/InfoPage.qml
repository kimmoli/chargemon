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

        contentHeight: column.height

        Column
        {
            id: column

            width: page.width
            spacing: Theme.paddingSmall
            PageHeader
            {
                title: "Other info"
            }
            Repeater
            {
                model: infolist
                delegate: Column
                {
                    width: page.width
                    spacing: Theme.paddingSmall

                    SectionHeader
                    {
                        text: title
                    }

                    Label
                    {
                        x: Theme.paddingLarge
                        text: cmon.infoPage[param]
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }
                }
            }
        }
    }

    Timer
    {
        interval: 500
        running: (applicationActive && page.status === PageStatus.Active)
        repeat: true
        triggeredOnStart: true
        onTriggered:
        {
            cmon.updateInfoPage()
        }
    }

    ListModel
    {
        id: infolist
        ListElement { title: "Time until low"; param: "time_until_low" }
        ListElement { title: "Time until full"; param: "time_until_full" }
        ListElement { title: "Status"; param: "battery/status" }
        ListElement { title: "Charge type"; param: "battery/charge_type" }
        ListElement { title: "USB Connection type"; param: "usb/type" }
        ListElement { title: "USB Max charging current"; param: "usb/current_max" }
        ListElement { title: "Health"; param: "battery/health" }
        ListElement { title: "Technology"; param: "battery/technology" }
    }

}
