#include "vkprocessor.h"

VkProcessor::VkProcessor(QObject *parent) :
    QObject(parent),
    _permissions(0)
{
    _settings.load();
}

void VkProcessor::loadSettings()
{
    _settings.load();
}

bool VkProcessor::permissionMessages()
{
    if ((_permissions & messages) == messages) return true;
    else return false;
}

bool VkProcessor::permissionOffline()
{
    if ((_permissions & offline) == offline) return true;
    else return false;
}

bool VkProcessor::getUser(const QString &alias, VkUser *usr)
{
    /*if (usr == nullptr)
        return false;*/
    QNetworkAccessManager manager(this);
    QUrlQuery query;
    QUrl url;
    url.setScheme("https");
    url.setHost("api.vk.com");
    url.setPath("/method/users.get");
    query.addQueryItem("v", "5.68");
    query.addQueryItem("access_token", _settings.accessToken());
    query.addQueryItem("user_ids", alias);
    query.addQueryItem("fields", "contacts");
    url.setQuery(query);
    qDebug() << "Request url";
    qDebug() << url.toString();
    manager.get(QNetworkRequest(url));
    QEventLoop loop(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QSharedPointer<QNetworkReply> replyPtr( manager.get(QNetworkRequest(url)));
    loop.exec();
    if (replyPtr->error() != QNetworkReply::NoError)
    {
        qCritical() << "VkProcessor: Reply error";
        qCritical() << replyPtr->errorString();
        return false;
    }
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyPtr->readAll(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError)
        return false;
    //Outputing full jsonObject
    QJsonObject jsonMainObject(jsonDocument.object());
    if (!jsonMainObject["response"].isArray())
        return false;
    QJsonArray jsonResponseArray = jsonMainObject["response"].toArray();
    if (jsonResponseArray.size() != 1)
        return false;
    if (!jsonResponseArray[0].isObject())
        return false;
    QJsonObject jsonUserObject = jsonResponseArray[0].toObject();
    qDebug() << jsonUserObject;
    if (usr != nullptr)
    {
        if (jsonUserObject["id"].isDouble())
            usr->userId = jsonUserObject["id"].toVariant().toLongLong();
        if (jsonUserObject["first_name"].isString())
            usr->firstName = jsonUserObject["first_name"].toString();
        if (jsonUserObject["last_name"].isString())
            usr->lastName = jsonUserObject["last_name"].toString();
        if (jsonUserObject["home_phone"].isString())
            usr->first_phone = jsonUserObject["home_phone"].toString();
        if (jsonUserObject["mobile_phone"].isString())
            usr->second_phone = jsonUserObject["mobile_phone"].toString();
    }
    qDebug() << "End of http";
    return true;
}

bool VkProcessor::sendMessage(const qint64 &toId, const QString &text)
{
    QNetworkAccessManager manager(this);
    QUrlQuery query;
    QUrl url;
    url.setScheme("https");
    url.setHost("api.vk.com");
    url.setPath("/method/messages.send");
    query.addQueryItem("v", "5.68");
    query.addQueryItem("access_token", _settings.accessToken());
    query.addQueryItem("user_id", QString::number(toId));
    query.addQueryItem("message", text);
    url.setQuery(query);
    QEventLoop loop(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QSharedPointer<QNetworkReply> replyPtr( manager.get(QNetworkRequest(url)));
    loop.exec();
    if (replyPtr->error() != QNetworkReply::NoError)
    {
        qCritical() << "VkProcessor: Reply error";
        qCritical() << replyPtr->errorString();
        return false;
    }
    QJsonDocument jsonDocument(QJsonDocument::fromJson(replyPtr->readAll()));
    QJsonObject jsonMainObject(jsonDocument.object());
    if (jsonMainObject["response"].isNull())
        return false;
    return true;
}

int VkProcessor::getPersmissions()
{
    QNetworkAccessManager manager(this);
    QUrlQuery query;
    QUrl url;
    url.setScheme("https");
    url.setHost("api.vk.com");
    url.setPath("/method/account.getAppPermissions");
    query.addQueryItem("v", "5.68");
    query.addQueryItem("access_token", _settings.accessToken());
    url.setQuery(query);
    QEventLoop loop(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QSharedPointer<QNetworkReply> replyPtr( manager.get(QNetworkRequest(url)));
    loop.exec();
    if (replyPtr->error() != QNetworkReply::NoError)
    {
        qCritical() << "VkProcessor: Reply error";
        qCritical() << replyPtr->errorString();
        return _permissions = -1;
    }
    QJsonDocument jsonDocument(QJsonDocument::fromJson(replyPtr->readAll()));
    QJsonObject jsonMainObject(jsonDocument.object());
    if (!jsonMainObject["response"].isNull())
        _permissions = jsonMainObject["response"].toInt();
    else
        return _permissions = -1;
    return _permissions;
}
