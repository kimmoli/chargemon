/*
    Charge Monitor (C) 2014 Kimmo Lindholm
*/

#include <QCoreApplication>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include "cmon.h"

Cmon::Cmon(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
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
