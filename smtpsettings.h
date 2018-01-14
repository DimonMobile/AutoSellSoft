#ifndef SMTPSETTINGS_H
#define SMTPSETTINGS_H

#include <QString>
#include <QSettings>
#include <QApplication>
#include <QDebug>

class SMTPSettings : QObject
{
    Q_OBJECT
private:
    QString host;
    quint16 port;
    QString email;
    QString username;
    QString password;
    bool    authorize;
    bool    useSsl;
public:
    SMTPSettings(QObject * parent = nullptr);
    SMTPSettings(const SMTPSettings &settings, QObject * parent = nullptr);
    QString getHost() const;
    void setHost(const QString &value);
    quint16 getPort() const;
    void setPort(const quint16 &value);
    QString getEmail() const;
    void setEmail(const QString &value);
    QString getUsername() const;
    void setUsername(const QString &value);
    QString getPassword() const;
    void setPassword(const QString &value);
    bool getAuthorize() const;
    void setAuthorize(bool value);
    bool getUseSsl() const;
    void setUseSsl(bool value);
    void save();
    void load();
};

#endif // SMTPSETTINGS_H
