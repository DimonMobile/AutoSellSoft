#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QString>
#include <QObject>
#include <QSettings>

class DataBaseSettings : QObject
{
    Q_OBJECT
private:
    QString _userName;
    QString _password;
    QString _host;
    quint16 _port;
    QString _dbName;
    QString _linkGeneratorUrl;
    QString _linkCheckerUrl;
    QString _webInterfaceKey;
    bool _useWebInterface;
public:
    DataBaseSettings(QObject * parent = nullptr);
    DataBaseSettings(const DataBaseSettings &settings, QObject * parent = nullptr);
    QString userName() const;
    void setUserName(const QString &userName);
    QString password() const;
    void setPassword(const QString &password);
    QString host() const;
    void setHost(const QString &host);
    quint16 port() const;
    void setPort(const quint16 &port);
    QString dbName() const;
    void setDbName(const QString &dbName);
    QString linkGeneratorUrl() const;
    void setLinkGeneratorUrl(const QString &linkGeneratorUrl);
    bool useWebInterface() const;
    void setUseWebInterface(bool useWebInterface);
    QString webInterfaceKey() const;
    void setWebInterfaceKey(const QString &webInterfaceKey);
    QString linkCheckerUrl() const;
    void setLinkCheckerUrl(const QString &linkCheckerUrl);
    //
    void loadSettings();
    void saveSettings();

};

#endif // DATABASESETTINGS_H
