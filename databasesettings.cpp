#include "databasesettings.h"

QString DataBaseSettings::userName() const
{
    return _userName;
}

void DataBaseSettings::setUserName(const QString &userName)
{
    _userName = userName;
}

QString DataBaseSettings::password() const
{
    return _password;
}

void DataBaseSettings::setPassword(const QString &password)
{
    _password = password;
}

QString DataBaseSettings::host() const
{
    return _host;
}

void DataBaseSettings::setHost(const QString &host)
{
    _host = host;
}

quint16 DataBaseSettings::port() const
{
    return _port;
}

void DataBaseSettings::setPort(const quint16 &port)
{
    _port = port;
}

QString DataBaseSettings::dbName() const
{
    return _dbName;
}

void DataBaseSettings::setDbName(const QString &dbName)
{
    _dbName = dbName;
}

void DataBaseSettings::loadSettings()
{
    QSettings settings;
    settings.beginGroup("Settings");
    _useWebInterface = settings.value("useWebInterface", false).toBool();
    settings.beginGroup("MySQL");
    _userName = settings.value("userName").toString();
    _password = settings.value("password").toString();
    _host = settings.value("host").toString();
    _port = settings.value("port").toUInt();
    _dbName = settings.value("dbName").toString();
    settings.endGroup();
    settings.beginGroup("Web-interface");
    _linkGeneratorUrl = settings.value("linkGeneratorUrl").toString();
    _linkCheckerUrl = settings.value("linkCheckerUrl").toString();
    _webInterfaceKey = settings.value("key").toString();
    settings.endGroup();
    settings.endGroup();
}

void DataBaseSettings::saveSettings()
{
    QSettings settings;
    settings.beginGroup("Settings");
    settings.setValue("useWebInterface", _useWebInterface);
    settings.beginGroup("MySQL");
    settings.setValue("userName", _userName);
    settings.setValue("password", _password);
    settings.setValue("host", _host);
    settings.setValue("port", _port);
    settings.setValue("dbName", _dbName);
    settings.endGroup();
    settings.beginGroup("Web-interface");
    settings.setValue("linkGeneratorUrl", _linkGeneratorUrl);
    settings.setValue("linkCheckerUrl", _linkCheckerUrl);
    settings.setValue("key", _webInterfaceKey);
    settings.endGroup();
    settings.endGroup();
}

QString DataBaseSettings::linkGeneratorUrl() const
{
    return _linkGeneratorUrl;
}

void DataBaseSettings::setLinkGeneratorUrl(const QString &linkGeneratorUrl)
{
    _linkGeneratorUrl = linkGeneratorUrl;
}

bool DataBaseSettings::useWebInterface() const
{
    return _useWebInterface;
}

void DataBaseSettings::setUseWebInterface(bool useWebInterface)
{
    _useWebInterface = useWebInterface;
}

QString DataBaseSettings::webInterfaceKey() const
{
    return _webInterfaceKey;
}

void DataBaseSettings::setWebInterfaceKey(const QString &webInterfaceKey)
{
    _webInterfaceKey = webInterfaceKey;
}

QString DataBaseSettings::linkCheckerUrl() const
{
    return _linkCheckerUrl;
}

void DataBaseSettings::setLinkCheckerUrl(const QString &linkCheckerUrl)
{
    _linkCheckerUrl = linkCheckerUrl;
}

DataBaseSettings::DataBaseSettings(QObject *parent) : QObject(parent)
{
    //empty c-tor
}

DataBaseSettings::DataBaseSettings(const DataBaseSettings &settings, QObject *parent) : QObject(parent)
{
    _userName = settings._userName;
    _password = settings._password;
    _host = settings._host;
    _port = settings._port;
    _dbName = settings._dbName;
    _linkGeneratorUrl = settings._linkGeneratorUrl;
    _useWebInterface = settings._useWebInterface;
    _webInterfaceKey = settings._webInterfaceKey;
}
