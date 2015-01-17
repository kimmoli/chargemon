/*
 * Charge monitor (C) 2014-2015 Kimmo Lindholm
 * LICENSE MIT
 */
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
