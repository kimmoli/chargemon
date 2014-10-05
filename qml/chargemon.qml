
import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.chargemon 1.0


ApplicationWindow
{

    id: chargemonitor

    property string coverActionLeftIcon: "image://theme/icon-cover-play"

    initialPage: Qt.resolvedUrl("pages/Chargemon.qml")

    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    function coverActionLeft()
    {
        if (!refreshTimer.running)
        {
            coverActionLeftIcon = "image://theme/icon-cover-pause"
            refreshTimer.start()
        }
        else
        {
            coverActionLeftIcon = "image://theme/icon-cover-play"
            refreshTimer.stop()
        }
    }

    function coverActionRight()
    {
        if (!refreshTimer.running)
        {
            cmon.update()
        }
    }

    Timer
    {
        id: refreshTimer
        interval: 500
        running: false
        repeat: true
        onTriggered:
            cmon.update()
    }

    Cmon
    {
        id: cmon
    }

}


