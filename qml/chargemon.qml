
import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.chargemon 1.0

import "pages"

ApplicationWindow
{
    initialPage: Component { Chargemon { } }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    Cmon
    {
        id: cmon
    }

}


