/*
 * Charge monitor (C) 2014-2017 Kimmo Lindholm
 * LICENSE MIT
 */
#include <QCoreApplication>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QStandardPaths>
#include <QDateTime>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>

#include "cmon.h"


Cmon::Cmon(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();

    deviceDetected = checkDevice();
    if (!deviceDetected)
        emit thisDeviceIsNotSupported();

    m_writeToFile = false;

    m_logFilename = QString("%1/chargemonlog.txt")
                    .arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    emit logFileNameChanged();

    propertyTimeUntilFull.reset(new ContextProperty("Battery.TimeUntilFull", this));
    propertyTimeUntilLow.reset(new ContextProperty("Battery.TimeUntilLow", this));

    QDBusConnection::sessionBus().connect("", "/com/jolla/lipstick", "com.jolla.lipstick", "coverstatus",
                          this, SLOT(handleCoverstatus(const QDBusMessage&)));

    m_coverStatus = 0;
    emit coverStatusChanged();

    /* Get initial values */
    update();
    updateInfoPage();
}

Cmon::~Cmon()
{
}

bool Cmon::checkDevice()
{
    int res = false;

    infoPageTypes.clear();
    infoPageTypes << "status";
    infoPageTypes << "charge_type";
    infoPageTypes << "health";
    infoPageTypes << "technology";
    infoPageTypes << "type";
    infoPageTypes << "current_max";

    if (!QDBusConnection::systemBus().isConnected())
    {
        printf("Cannot connect to the D-Bus systemBus\n%s\n", qPrintable(QDBusConnection::systemBus().lastError().message()));
        return false;
    }


    QDBusInterface ssuCall("org.nemo.ssu", "/org/nemo/ssu", "org.nemo.ssu", QDBusConnection::systemBus());
    ssuCall.setTimeout(1000);

    QList<QVariant> args;
    args.append(2);

    QDBusMessage ssuCallReply = ssuCall.callWithArgumentList(QDBus::Block, "displayName", args);

    if (ssuCallReply.type() == QDBusMessage::ErrorMessage)
    {
        printf("Error: %s\n", qPrintable(ssuCallReply.errorMessage()));
        return false;
    }

    QList<QVariant> outArgs = ssuCallReply.arguments();
    if (outArgs.count() == 0)
    {
        printf("Reply is epmty\n");
        return false;
    }

    deviceName = outArgs.at(0).toString();

    printf("device name is %s\n", qPrintable(deviceName));

    if (outArgs.at(0).toString() == "JP-1301") /* The one and only original Jolla phone */
    {
        generalValues.clear();
        generalValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8xxx-adc/dcin";
        generalValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8xxx-adc/usbin";
        generalValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/current_now";
        generalValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/voltage_now";
        generalValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/capacity";
        generalValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/temp";

        infoPageValues.clear();
        infoPageValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/status";
        infoPageValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/charge_type";
        infoPageValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/health";
        infoPageValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/technology";
        infoPageValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/usb/type";
        infoPageValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/usb/current_max";

        infoPageRawValues.clear();
        infoPageRawValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/charge_full";
        infoPageRawValues << "/sys/devices/platform/msm_ssbi.0/pm8038-core/pm8921-charger/power_supply/battery/charge_full_design";

        res = true;
    }
    else if (outArgs.at(0).toString() == "JT-1501") /* The one and only original Jolla tablet */
    {
        generalValues.clear();
        generalValues << "";
        generalValues << ""; /* No reliable USB voltage */
        generalValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/current_now";
        generalValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/voltage_now";
        generalValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/capacity";
        generalValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/temp";

        infoPageValues.clear();
        infoPageValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/status";
        infoPageValues << "";
        infoPageValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/health";
        infoPageValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/technology";
        infoPageValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_charger/power_supply/dollar_cove_charger/type";
        infoPageValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_charger/power_supply/dollar_cove_charger/max_charge_current";

        infoPageRawValues.clear();
        infoPageRawValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/charge_full";
        infoPageRawValues << "/sys/devices/platform/80860F41:04/i2c-5/5-0034/dollar_cove_battery/power_supply/dollar_cove_battery/charge_full_design";

        res = true;
    }
    else if (outArgs.at(0).toString() == "onyx") /* OneplusX */
    {
        generalValues.clear();
        generalValues << "";
        generalValues << "/sys/devices/msm_dwc3/power_supply/usb/voltage_now";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/current_now";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/voltage_now";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/capacity";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/temp";

        infoPageValues.clear();
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/status";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/charge_type";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/health";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/technology";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/qpnp-dc/type";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/qpnp-dc/current_max";

        infoPageRawValues.clear();
        infoPageRawValues << "/sys/devices/qpnp-charger-*/power_supply/battery/charge_full";
        infoPageRawValues << "/sys/devices/qpnp-charger-*/power_supply/battery/charge_full_design";

        res = true;
    }
    else if (outArgs.at(0).toString() == "fp2-sibon")
    {
        generalValues.clear();
        generalValues << "";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/subsystem/usb/voltage_now";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/current_now";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/voltage_now";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/capacity";
        generalValues << "/sys/devices/qpnp-charger-*/power_supply/battery/temp";

        infoPageValues.clear();
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/status";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/charge_type";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/health";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/battery/technology";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/qpnp-dc/type";
        infoPageValues << "/sys/devices/qpnp-charger-*/power_supply/qpnp-dc/current_max";

        infoPageRawValues.clear();
        infoPageRawValues << "/sys/devices/qpnp-charger-*/power_supply/battery/charge_full";
        infoPageRawValues << "/sys/devices/qpnp-charger-*/power_supply/battery/charge_full_design";

        res = true;
    }
    else if ((outArgs.at(0).toString() == "JP-1601") || /* Jolla C */
             (outArgs.at(0).toString() == "Aqua Fish") || /* Aquafish */
             (outArgs.at(0).toString() == "L500D"))  /* This is also Aquafish */
    {
        generalValues.clear();
        generalValues << "";
        generalValues << "/sys/devices/soc.0/qpnp-vadc-*/usb_in";
        generalValues << "/sys/devices/soc.0/qpnp-linear-charger-*/power_supply/battery/current_now";
        generalValues << "/sys/devices/soc.0/qpnp-linear-charger-*/power_supply/battery/voltage_now";
        generalValues << "/sys/devices/soc.0/qpnp-linear-charger-*/power_supply/battery/capacity";
        generalValues << "/sys/devices/soc.0/qpnp-linear-charger-*/power_supply/battery/temp";

        infoPageValues.clear();
        infoPageValues << "/sys/devices/soc.0/qpnp-linear-charger-*/power_supply/battery/status";
        infoPageValues << "/sys/devices/soc.0/qpnp-linear-charger-*/power_supply/battery/charge_type";
        infoPageValues << "/sys/devices/soc.0/qpnp-linear-charger-f4bca200/power_supply/battery/health";
        infoPageValues << "/sys/devices/soc.0/qpnp-vm-bms-*/power_supply/bms/battery_type";
        infoPageValues << "/sys/devices/soc.0/*.usb/power_supply/usb/type";
        infoPageValues << "/sys/devices/soc.0/*.usb/power_supply/usb/current_max";

        infoPageRawValues.clear();
        /* charge_full, charge_full_design not available on Jolla C */

        res = true;
    }
    else if (outArgs.at(0).toString() == "f5121" || /* Sony Xperia X */
             outArgs.at(0).toString() == "f5122" || /* Sony Xperia X dual SIM */
             outArgs.at(0).toString() == "f5321")  /* Sony Xperia X Compact */
    {
        generalValues.clear();
        generalValues << "";
        generalValues << "/sys/devices/soc.0/*.usb/power_supply/usb/voltage_now";
        generalValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/current_now";
        generalValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/voltage_now";
        generalValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/capacity";
        generalValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/temp";

        infoPageValues.clear();
        infoPageValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/status";
        infoPageValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/charge_type";
        infoPageValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/health";
        infoPageValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/technology";
        infoPageValues << "/sys/devices/soc.0/*.usb/power_supply/usb/type";
        infoPageValues << "/sys/devices/soc.0/*.usb/power_supply/usb/current_max";

        infoPageRawValues.clear();
        infoPageRawValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/charge_full";
        infoPageRawValues << "/sys/devices/soc.0/*-qcom,qpnp-smbcharger/power_supply/battery/charge_full_design";

        res = true;
    }

    glob(&generalValues);
    glob(&infoPageValues);
    glob(&infoPageRawValues);

    return res;
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

    if (!deviceDetected)
        return;

    if (deviceName == "JP-1301")
    {
        p_tmp = readOneLineFromFile(generalValues.at(0));
        m_dcinvoltage = p_tmp.split(QRegExp("\\W+"), QString::SkipEmptyParts).at(1).toFloat() / 1e6;
    }
    else
    {
        m_dcinvoltage = 0;
    }

    p_tmp = readOneLineFromFile(generalValues.at(1));
    if (deviceName == "JP-1301" || deviceName == "JP-1601")
        m_usbinvoltage = p_tmp.split(QRegExp("\\W+"), QString::SkipEmptyParts).at(1).toFloat() / 1e6;
    else
        m_usbinvoltage = p_tmp.toFloat() / 1e6;

    p_tmp = readOneLineFromFile(generalValues.at(2));
    m_current = p_tmp.toFloat() / 1e6;

    p_tmp = readOneLineFromFile(generalValues.at(3));
    m_voltage = p_tmp.toFloat() / 1e6;

    p_tmp = readOneLineFromFile(generalValues.at(4));
    m_capacity = p_tmp.toFloat();

    p_tmp = readOneLineFromFile(generalValues.at(5));
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

void Cmon::updateInfoPage()
{
    if (!deviceDetected)
        return;

    m_infoPage.clear();

    int i;
    for (i=0 ; i<infoPageTypes.count() && i<infoPageValues.count() ; i++)
    {
        if (!infoPageValues.at(i).isEmpty())
        {
            QString fpath = infoPageValues.at(i);
            m_infoPage.insert(infoPageTypes.at(i), readOneLineFromFile(fpath));
        }
        else
        {
            m_infoPage.insert(infoPageTypes.at(i), "None");
        }
    }

    if (infoPageRawValues.count() >= 2)
    {
        uint energy_full = readOneLineFromFile(infoPageRawValues.at(0)).toInt();
        uint energy_full_design = readOneLineFromFile(infoPageRawValues.at(1)).toInt();

        if (energy_full_design > 0)
        {
            m_infoPage.insert("capacity_left", QString::number(100*energy_full/energy_full_design) + "%");
        }
        else
        {
            m_infoPage.insert("capacity_left", "Unknown");
        }
    }
    else
    {
        m_infoPage.insert("capacity_left", "Unknown");
    }

    /* contextproperties */

    m_infoPage.insert("time_until_low", QDateTime::fromTime_t(propertyTimeUntilLow->value().toInt()).toUTC().toString("hh:mm:ss"));
    m_infoPage.insert("time_until_full", QDateTime::fromTime_t(propertyTimeUntilFull->value().toInt()).toUTC().toString("hh:mm:ss"));

    emit infoPageChanged();
}

QString Cmon::readDcinVoltage()
{
    if (deviceName == "JP-1301")
        return QString::number(m_dcinvoltage) + " V";
    else
        return QString("N/A");
}

QString Cmon::readUsbinVoltage()
{
    if (deviceName == "JT-1501")
        return QString("N/A");
    else
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

void Cmon::glob(QStringList *files)
{
    /* If the file paths contains wildcards, try to resolve them */
    int i,j;

    for (i=0; i < files->size(); i++)
    {
        if (files->at(i).contains("*"))
        {
            QStringList s = files->at(i).split("/");

            for (j=0; j<s.size(); j++)
            {
                if (s.at(j).contains("*"))
                {
                    QStringList filters;
                    filters << s.at(j);

                    QDir dir(QStringList(s.mid(0,j)).join("/"));

                    QStringList dirs = dir.entryList(filters, QDir::Dirs);

                    if (dirs.size() > 0)
                    {
                        s.replace(j, dirs.at(0));
                        files->replace(i, s.join("/"));
                    }
                }
            }
        }
    }
}
