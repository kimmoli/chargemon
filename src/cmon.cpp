#include "cmon.h"
#include <QSettings>
#include <QCoreApplication>
#include <QProcess>

Cmon::Cmon(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
}

Cmon::~Cmon()
{
}

/* Return git describe as string (see .pro file) */
QString Fantsu::readVersion()
{
    return GITHASH;
}

void Cmon::update()
{
    QProcess p;

    p.start("sh", QStringList() << "-c" << "cat /sys/devices/platform/msm_ssbi.0/pm8038-core/pm8xxx-adc/dcin");
    p.waitForFinished(-1);

    QString p_dcinnow = p.readAllStandardOutput();
    m_dcinvoltage = p_dcinnow.split(QRegExp("\\W+"), QString::SkipEmptyParts).at(1).toFloat() / 1e6;

    p.start("sh", QStringList() << "-c" << "cat /sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/current_now");
    p.waitForFinished(-1);

    QString p_currentNow = p.readAllStandardOutput();
    m_current = p_currentNow.toFloat() / 1e6;

    p.start("sh", QStringList() << "-c" << "cat /sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/voltage_now");
    p.waitForFinished(-1);

    QString p_voltageNow = p.readAllStandardOutput();
    m_voltage = p_voltageNow.toFloat() / 1e6;

    p.start("sh", QStringList() << "-c" << "cat /sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/capacity");
    p.waitForFinished(-1);

    QString p_capacity = p.readAllStandardOutput();
    m_capacity = p_capacity.toFloat();

    p.start("sh", QStringList() << "-c" << "cat /sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/temp");
    p.waitForFinished(-1);

    QString p_temp = p.readAllStandardOutput();
    m_temperature = p_temp.toFloat() / 10;


    emit batteryCurrentChanged();
    emit batteryVoltageChanged();
    emit dcinVoltageChanged();
    emit batteryCapacityChanged();
    emit batteryTemperatureChanged();
}


QString Cmon::readDcinVoltage()
{
    return QString::number(m_dcinvoltage) + " V";
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
