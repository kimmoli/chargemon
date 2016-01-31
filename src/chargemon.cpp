/*
 * Charge monitor (C) 2014-2015 Kimmo Lindholm
 * LICENSE MIT
 */
#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QtQml>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QCoreApplication>
#include "cmon.h"


int main(int argc, char *argv[])
{
    qmlRegisterType<Cmon>("harbour.chargemon", 1, 0, "Cmon");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->setSource(SailfishApp::pathTo("qml/chargemon.qml"));
    view->show();

    return app->exec();
}

