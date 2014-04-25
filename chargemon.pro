# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = chargemon

CONFIG += sailfishapp

#Force building to update version and build-date
system(rm $$OUT_PWD/cmon.o)

#show some info about git status
system(git --git-dir $$PWD/.git diff --name-only)

REVISION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --dirty=-dev --always)
DEFINES += "GITHASH=\\\"$${REVISION}\\\""

message($${REVISION})


SOURCES += src/chargemon.cpp \
	src/cmon.cpp
	
HEADERS += src/cmon.h

OTHER_FILES += qml/chargemon.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Chargemon.qml \
    rpm/chargemon.spec \
	chargemon.png \
    chargemon.desktop \
    qml/pages/aboutPage.qml

