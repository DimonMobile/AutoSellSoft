#ifndef VKSETTINGS_H
#define VKSETTINGS_H

#include <QSettings>
#include <QString>

class VkSettings
{
private:
    QString _sourceLink;
    QString _accessToken;
    QString _userEmail;
    qint64  _userId;
public:
    VkSettings();
    void save() const;
    void load();
    QString accessToken() const;
    void setAccessToken(const QString &accessToken);
    QString userEmail() const;
    void setUserEmail(const QString &userEmail);
    qint64 userId() const;
    void setUserId(const qint64 &userId);
    QString sourceLink() const;
    void setSourceLink(const QString &sourceLink);
};

#endif // VKSETTINGS_H
