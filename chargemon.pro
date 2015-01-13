# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-chargemon

CONFIG += sailfishapp
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
    qml/pages/Messagebox.qml

