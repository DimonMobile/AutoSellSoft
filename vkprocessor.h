#ifndef VKPROCESSOR_H
#define VKPROCESSOR_H

#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "vksettings.h"

enum VKPermissions{
    notify = 1,
    friends = 2,
    photos = 4,
    audio = 8,
    video = 16,
    pages = 128,
    status = 1024,
    notes = 2048,
    messages = 4096,
    wall = 8192,
    ads = 32768,
    offline = 65536,
    docs = 131072,
    groups = 262144,
    notifications = 524288,
    stats = 1048576,
    email = 4194304,
    market = 134217728
};

struct VkUser{
    qint64 userId;
    QString firstName, lastName, first_phone, second_phone;
};

class VkProcessor : QObject
{
    Q_OBJECT
private:
    VkSettings _settings;
    int _permissions;
public:
    VkProcessor(QObject * parent = nullptr);
    void loadSettings();
    bool permissionMessages();
    bool permissionOffline();

    bool getUser(const QString &alias, VkUser *usr = nullptr);
    bool sendMessage(const qint64 &toId, const QString &text);
    int getPersmissions();
};

#endif // VKPROCESSOR_H
