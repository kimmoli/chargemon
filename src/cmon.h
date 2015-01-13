#ifndef CMON_H
#define CMON_H
#include <QObject>
#include <QTextStream>
#include <QDBusMessage>


class Cmon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ readVersion NOTIFY versionChanged())

    Q_PROPERTY(QString dcinVoltage READ readDcinVoltage NOTIFY dcinVoltageChanged())
    Q_PROPERTY(QString usbinVoltage READ readUsbinVoltage NOTIFY usbinVoltageChanged())
    Q_PROPERTY(QString batteryVoltage READ readBatteryVoltage NOTIFY batteryVoltageChanged())
    Q_PROPERTY(QString batteryCurrent READ readBatteryCurrent NOTIFY batteryCurrentChanged())
    Q_PROPERTY(QString batteryCapacity READ readBatteryCapacity NOTIFY batteryCapacityChanged())
    Q_PROPERTY(QString batteryTemperature READ readBatteryTemperature NOTIFY batteryTemperatureChanged())
    Q_PROPERTY(QString logFileName READ readLogFileName NOTIFY logFileNameChanged())
    Q_PROPERTY(int coverStatus READ readCoverStatus NOTIFY coverStatusChanged())

public:
    explicit Cmon(QObject *parent = 0);
    ~Cmon();

    QString readVersion();

    QString readDcinVoltage();
    QString readUsbinVoltage();
    QString readBatteryVoltage();
    QString readBatteryCurrent();
    QString readBatteryCapacity();
    QString readBatteryTemperature();
    QString readLogFileName() { return m_logFilename; }
    int readCoverStatus() { return m_coverStatus; }

    Q_INVOKABLE void update();

    Q_INVOKABLE void setWriteToFile(bool enable);

public slots:
    void handleCoverstatus(const QDBusMessage& msg);

signals:
    void versionChanged();

    void dcinVoltageChanged();
    void usbinVoltageChanged();
    void batteryVoltageChanged();
    void batteryCurrentChanged();
    void batteryCapacityChanged();
    void batteryTemperatureChanged();
    void logFileNameChanged();

    void coverStatusChanged();

private:
    QString readOneLineFromFile(QString name);
    float m_dcinvoltage;
    float m_usbinvoltage;
    float m_voltage;
    float m_current;
    float m_capacity;
    float m_temperature;

    bool m_writeToFile;
    QString m_logFilename;
    int m_coverStatus;
};


#endif // CMON_H

