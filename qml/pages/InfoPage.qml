/*
 * Charge monitor (C) 2014-2019 Kimmo Lindholm
 * LICENSE MIT
 */
import QtQuick 2.0
import Sailfish.Silica 1.0
import org.freedesktop.contextkit 1.0


Page
{
    id: page

    ContextProperty {
        id: timeUntilFull
        key: "Battery.TimeUntilFull"
        value: 0
    }

    ContextProperty {
        id: timeUntilLow
        key: "Battery.TimeUntilLow"
        value: 0
    }

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

            Column
            {
                width: page.width
                spacing: Theme.paddingSmall

                SectionHeader
                {
                    text: "Time until low"
                }

                Label
                {
                    x: Theme.paddingLarge
                    text: new Date(timeUntilLow.value * 1000).toISOString().substr(11, 8)
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraLarge
                }
            }

            Column
            {
                width: page.width
                spacing: Theme.paddingSmall

                SectionHeader
                {
                    text: "Time until full"
                }

                Label
                {
                    x: Theme.paddingLarge
                    text: new Date(timeUntilFull.value * 1000).toISOString().substr(11, 8)
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraLarge
                }
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
        ListElement { title: "Status"; param: "status" }
        ListElement { title: "Charge type"; param: "charge_type" }
        ListElement { title: "USB Connection type"; param: "type" }
        ListElement { title: "USB Max charging current"; param: "current_max" }
        ListElement { title: "Health"; param: "health" }
        ListElement { title: "Capacity left"; param: "capacity_left" }
        ListElement { title: "Technology"; param: "technology" }
    }

}
