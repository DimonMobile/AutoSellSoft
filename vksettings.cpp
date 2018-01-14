#include "vksettings.h"

QString VkSettings::accessToken() const
{
    return _accessToken;
}

void VkSettings::setAccessToken(const QString &accessToken)
{
    _accessToken = accessToken;
}

QString VkSettings::userEmail() const
{
    return _userEmail;
}

void VkSettings::setUserEmail(const QString &userEmail)
{
    _userEmail = userEmail;
}

qint64 VkSettings::userId() const
{
    return _userId;
}

void VkSettings::setUserId(const qint64 &userId)
{
    _userId = userId;
}

QString VkSettings::sourceLink() const
{
    return _sourceLink;
}

void VkSettings::setSourceLink(const QString &sourceLink)
{
    _sourceLink = sourceLink;
}

VkSettings::VkSettings()
{

}

void VkSettings::save() const
{
    QSettings settings;
    settings.beginGroup("Settings");
    settings.beginGroup("Vk");
    settings.setValue("accessToken", _accessToken);
    settings.setValue("userId", _userId);
    settings.setValue("userEmail", _userEmail);
    settings.setValue("source", _sourceLink);
    settings.endGroup();
    settings.endGroup();
}

void VkSettings::load()
{
    QSettings settings;
    settings.beginGroup("Settings");
    settings.beginGroup("Vk");
    _accessToken = settings.value("accessToken").toString();
    _userId = settings.value("userId").toLongLong();
    _userEmail = settings.value("userEmail").toString();
    _sourceLink = settings.value("source").toString();
    settings.endGroup();
    settings.endGroup();
}
