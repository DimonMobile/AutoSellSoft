#include "smtpsettings.h"

QString SMTPSettings::getHost() const
{
    return host;
}

void SMTPSettings::setHost(const QString &value)
{
    host = value;
}

quint16 SMTPSettings::getPort() const
{
    return port;
}

void SMTPSettings::setPort(const quint16 &value)
{
    port = value;
}

QString SMTPSettings::getEmail() const
{
    return email;
}

void SMTPSettings::setEmail(const QString &value)
{
    email = value;
}

QString SMTPSettings::getUsername() const
{
    return username;
}

void SMTPSettings::setUsername(const QString &value)
{
    username = value;
}

QString SMTPSettings::getPassword() const
{
    return password;
}

void SMTPSettings::setPassword(const QString &value)
{
    password = value;
}

bool SMTPSettings::getAuthorize() const
{
    return authorize;
}

void SMTPSettings::setAuthorize(bool value)
{
    authorize = value;
}

bool SMTPSettings::getUseSsl() const
{
    return useSsl;
}

void SMTPSettings::setUseSsl(bool value)
{
    useSsl = value;
}

void SMTPSettings::save()
{
    QSettings settings;
    settings.beginGroup("Settings");
    settings.beginGroup("SMTP");

    settings.setValue("username", username);
    settings.setValue("password", password);
    settings.setValue("host", host);
    settings.setValue("port", port);
    settings.setValue("authorize", authorize);
    settings.setValue("usessl", useSsl);
    settings.setValue("email", email);

    qDebug() << "SMTP settings saved to" << settings.fileName();

    settings.endGroup();
    settings.endGroup();
}

void SMTPSettings::load()
{
    QSettings settings;
    settings.beginGroup("Settings");
    settings.beginGroup("SMTP");

    username = settings.value("username").toString();
    password = settings.value("password").toString();
    host = settings.value("host").toString();
    port = settings.value("port").toUInt();
    authorize = settings.value("authorize").toBool();
    useSsl = settings.value("usessl").toBool();
    email = settings.value("email").toString();

    settings.endGroup();
    settings.endGroup();
}

SMTPSettings::SMTPSettings(QObject *parent) : QObject(parent)
{

}

SMTPSettings::SMTPSettings(const SMTPSettings &settings, QObject *parent) : QObject(parent)
{
    username = settings.username;
    password = settings.password;
    host = settings.host;
    port = settings.port;
    email = settings.email;
    authorize = settings.authorize;
    useSsl = settings.useSsl;
}
