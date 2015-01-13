import QtQuick 2.0
import Sailfish.Silica 1.0

CoverActionList
{
    CoverAction
    {
        iconSource: coverActionLeftIcon
        onTriggered: coverActionLeft()
    }

    CoverAction
    {
        iconSource: coverActionRightIcon
        onTriggered: coverActionRight()
    }
}
