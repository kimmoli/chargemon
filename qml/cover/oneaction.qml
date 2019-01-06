/*
 * Charge monitor (C) 2014-2019 Kimmo Lindholm
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
}
