#
# QtCreator project for harbour-chargemon, Charge Monitor
#
# (C) 2014-2019 Kimmo Lindholm, LICENSE MIT
#
TARGET = harbour-chargemon

CONFIG += sailfishapp
QT += dbus

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

message($${DEFINES})

SAILFISHAPP_ICONS += 86x86 108x108 128x128 256x256

SOURCES += src/chargemon.cpp \
	src/cmon.cpp
	
HEADERS += src/cmon.h

OTHER_FILES += qml/chargemon.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Chargemon.qml \
    rpm/chargemon.spec \
    harbour-chargemon.desktop \
    icons/108x108/harbour-chargemon.png \
    icons/128x128/harbour-chargemon.png \
    icons/256x256/harbour-chargemon.png \
    icons/86x86/harbour-chargemon.png \
    qml/pages/AboutPage.qml \
    qml/cover/charger.png \
    qml/cover/oneaction.qml \
    qml/cover/twoactions.qml \
    qml/pages/Messagebox.qml \
    qml/pages/InfoPage.qml

