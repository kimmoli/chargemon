/*
 * Charge monitor (C) 2014-2015 Kimmo Lindholm
 * LICENSE MIT
 */
#include <QCoreApplication>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QStandardPaths>
#include <QDateTime>
#include <QDBusConnection>
#include <QDebug>

#include "cmon.h"


Cmon::Cmon(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();

    m_writeToFile = false;

    m_logFilename = QString("%1/chargemonlog.txt")
                    .arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    emit logFileNameChanged();

    QDBusConnection::sessionBus().connect("", "/com/jolla/lipstick", "com.jolla.lipstick", "coverstatus",
                          this, SLOT(handleCoverstatus(const QDBusMessage&)));

    m_coverStatus = 0;
    emit coverStatusChanged();
}

Cmon::~Cmon()
{
}

/* Return git describe as string (see .pro file) */
QString Cmon::readVersion()
{
    return APPVERSION;
}

/* Read first line of file with Qt functions */
QString Cmon::readOneLineFromFile(QString name)
{
    QString line;

    QFile inputFile( name );

    if ( inputFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
       QTextStream in( &inputFile );
       line = in.readLine();
       inputFile.close();
    }
    else
    {
        line = QString("Error occured.");
    }

    return line;
}

void Cmon::update()
{
    QString p_tmp;

    p_tmp = readOneLineFromFile("/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8xxx-adc/dcin");
    m_dcinvoltage = p_tmp.split(QRegExp("\\W+"), QString::SkipEmptyParts).at(1).toFloat() / 1e6;

    p_tmp = readOneLineFromFile("/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8xxx-adc/usbin");
    m_usbinvoltage = p_tmp.split(QRegExp("\\W+"), QString::SkipEmptyParts).at(1).toFloat() / 1e6;

    p_tmp = readOneLineFromFile("/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/current_now");
    m_current = p_tmp.toFloat() / 1e6;

    p_tmp = readOneLineFromFile("/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/voltage_now");
    m_voltage = p_tmp.toFloat() / 1e6;

    p_tmp = readOneLineFromFile("/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/capacity");
    m_capacity = p_tmp.toFloat();

    p_tmp = readOneLineFromFile("/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/temp");
    m_temperature = p_tmp.toFloat() / 10;


    emit dcinVoltageChanged();
    emit usbinVoltageChanged();
    emit batteryVoltageChanged();
    emit batteryCurrentChanged();
    emit batteryCapacityChanged();
    emit batteryTemperatureChanged();

    if (m_writeToFile)
    {
        QDate ssDate = QDate::currentDate();
        QTime ssTime = QTime::currentTime();

        QFile file(m_logFilename);
        file.open(QIODevice::Append | QIODevice::Text);
        QTextStream logfile(&file);

        QString timestamp = QString("%1.%2.%3 %4:%5:%6.%7 ")
                .arg((int) ssDate.day(),    2, 10, QLatin1Char('0'))
                .arg((int) ssDate.month(),  2, 10, QLatin1Char('0'))
                .arg((int) ssDate.year(),   4, 10, QLatin1Char('0'))
                .arg((int) ssTime.hour(),   2, 10, QLatin1Char('0'))
                .arg((int) ssTime.minute(), 2, 10, QLatin1Char('0'))
                .arg((int) ssTime.second(), 2, 10, QLatin1Char('0'))
                .arg((int) ssTime.msec(), 3, 10, QLatin1Char('0'));

        logfile.setPadChar(' ');
        logfile.setFieldWidth(12);

        logfile << timestamp << readBatteryCapacity() << readBatteryTemperature() << readBatteryVoltage() << readBatteryCurrent() << \
                   readDcinVoltage() << readUsbinVoltage() << "\n";

        file.close();
    }
}


QString Cmon::readDcinVoltage()
{
    return QString::number(m_dcinvoltage) + " V";
}

QString Cmon::readUsbinVoltage()
{
    return QString::number(m_usbinvoltage) + " V";
}

QString Cmon::readBatteryVoltage()
{
    return QString::number(m_voltage) + " V";
}

QString Cmon::readBatteryCurrent()
{
    return QString::number(m_current * 1000.0) + " mA";
}

QString Cmon::readBatteryCapacity()
{
    return QString::number(m_capacity) + "%";
}

QString Cmon::readBatteryTemperature()
{
    return QString::number(m_temperature) + QString::fromUtf8("\u00B0C");
}


void Cmon::setWriteToFile(bool enable)
{
    m_writeToFile = enable;
}


void Cmon::handleCoverstatus(const QDBusMessage& msg)
{
    QList<QVariant> args = msg.arguments();
    m_coverStatus = args.at(0).toInt();
    emit coverStatusChanged();
}
