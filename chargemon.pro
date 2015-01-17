#
# QtCreator project for harbour-chargemon, Charge Monitor
#
# (C) 2014-2015 Kimmo Lindholm, LICENSE MIT
#
TARGET = harbour-chargemon

CONFIG += sailfishapp
PKGCONFIG += contextkit-statefs
QT += dbus

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

message($${DEFINES})

SOURCES += src/chargemon.cpp \
	src/cmon.cpp
	
HEADERS += src/cmon.h

OTHER_FILES += qml/chargemon.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Chargemon.qml \
    rpm/chargemon.spec \
    harbour-chargemon.desktop \
    harbour-chargemon.png \
    qml/pages/AboutPage.qml \
    qml/cover/charger.png \
    qml/cover/oneaction.qml \
    qml/cover/twoactions.qml \
    qml/pages/Messagebox.qml \
    qml/pages/InfoPage.qml

