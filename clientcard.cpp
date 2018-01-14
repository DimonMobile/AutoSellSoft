#include "clientcard.h"

QString ClientCard::getContactFace() const
{
    return _contactFace;
}

void ClientCard::setContactFace(const QString &contactFace)
{
    _contactFace = contactFace;
}

QString ClientCard::getOrganizationName() const
{
    return _organizationName;
}

void ClientCard::setOrganizationName(const QString &organizationName)
{
    _organizationName = organizationName;
}

QString ClientCard::getDisplayText() const
{
    return _displayText;
}

void ClientCard::setDisplayText(const QString &displayText)
{
    _displayText = displayText;
}

QString ClientCard::getFunnelFileName() const
{
    return _funnelFileName;
}

void ClientCard::setFunnelFileName(const QString &funnelFileName)
{
    _funnelFileName = funnelFileName;
}

qint32 ClientCard::getBlockId() const
{
    return _blockId;
}

void ClientCard::setBlockId(const qint32 &blockId)
{
    _blockId = blockId;
}

QDateTime ClientCard::getBlockExecDateTime() const
{
    return _blockExecDateTime;
}

void ClientCard::setBlockExecDateTime(const QDateTime &blockExecDateTime)
{
    _blockExecDateTime = blockExecDateTime;
}

quint32 ClientCard::getLastLinkId() const
{
    return _lastLinkId;
}

void ClientCard::setLastLinkId(const quint32 &lastLinkId)
{
    _lastLinkId = lastLinkId;
}

ClientCard::ClientCard(const int id, const QString &displaytext) :
    _blockId(-1)
{
    this->_id = id;
    this->_displayText = displaytext;
    
    //    this->_phoneNumber.insert("Life:)", "+375259631757");
//    this->_phoneNumber.insert("МТС", "+375339341661");
//    this->_phoneNumber.insert("Velcom", "+375299594375");

//    this->_emailAddress.insert("gmail", "mobik.dimka@gmail.com");
//    this->_emailAddress.insert("mail.ru", "mobile.dimon@mail.ru");

//    this->_serviceId.insert("VK", 533432413);
    //    this->_serviceId.insert("Facebook", 4321234);
}

ClientCard::ClientCard() :
    _id(0),
    _blockId(-1),
    _displayText("New simple client")
{

}

void ClientCard::addServiceId(const QString &key, const qint64 &val)
{
    _serviceId.insert(key, val);
}

qint64 ClientCard::getServiceId(const QString &key, bool *result) const
{
    QHash<QString, qint64>::const_iterator it = _serviceId.find(key);
    if (it == _serviceId.end())
    {
        if (result != nullptr)
            *result = false;
        return 0;
    }
    else
    {
        if (result != nullptr)
            *result = true;
        return *it;
    }
}

QVector<QPair<QString, qint64> > ClientCard::getServiceList() const
{
    QVector<QPair<QString, qint64> > result;
    for(QHash<QString, qint64>::ConstIterator i = _serviceId.begin(); i != _serviceId.end(); ++i)
    {
        QPair<QString, qint64> element;
        element.first = i.key();
        element.second = i.value();
        result.push_back(element);
    }
    return result;
}

void ClientCard::setServiceList(const QVector<QPair<QString, qint64> > &ptr)
{
    _serviceId.clear();
    for(QVector<QPair<QString, qint64> >::const_iterator i = ptr.begin(); i != ptr.end(); ++i)
        _serviceId.insert(i->first, i->second);
}

void ClientCard::addEmptyServiceNode()
{
    _serviceId.insert("", 0);
}

void ClientCard::loadServiceFromEdits()
{
    _serviceId.clear();
    for(QVector<QPair<QLineEdit*, QLineEdit*> >::const_iterator i = servicesEditList.begin(); i != servicesEditList.end(); ++i)
        _serviceId.insert(i->first->text(), i->second->text().toLongLong());
}

void ClientCard::addPhoneNumber(const QString &key, const QString &val)
{
    _phoneNumber.insert(key, val);
}

QString ClientCard::getPhoneNumber(const QString &key, bool *result) const
{
    QHash<QString, QString>::const_iterator it = _phoneNumber.find(key);
    //Не нашли такой
    if (it == _phoneNumber.end())
    {
        if (result != nullptr)
            *result = false;
        return QString();
    }
    //А вот нашли
    else
    {
        if (result != nullptr)
            *result = true;
        return *it;
    }
}

QVector<QPair<QString, QString> > ClientCard::getPhoneList() const
{
    QVector<QPair<QString, QString> > result;
    for(QHash<QString, QString>::ConstIterator i = _phoneNumber.begin(); i != _phoneNumber.end(); ++i)
    {
        QPair<QString, QString> element;
        element.first = i.key();
        element.second = i.value();
        result.push_back(element);
    }
    return result;
}

void ClientCard::setPhoneList(const QVector<QPair<QString, QString> > &par)
{
    _phoneNumber.clear();
    for(QVector<QPair<QString, QString> >::const_iterator i = par.begin(); i != par.end(); ++i)
        _phoneNumber.insert(i->first, i->second);
}

void ClientCard::addEmptyPhoneNode()
{
    _phoneNumber.insert("", "+375");
}

void ClientCard::loadPhoneFromEdits()
{
    _phoneNumber.clear();
    for(QVector<QPair<QLineEdit*, QLineEdit*> >::const_iterator i = phonesEditList.begin(); i != phonesEditList.end(); ++i)
    {
        QString fs = i->first->text();
        QString ss = i->second->text();
        _phoneNumber.insert(fs, ss);
    }
}

void ClientCard::addEmailAddress(const QString &key, const QString &val)
{
    _emailAddress.insert(key, val);
}

QString ClientCard::getEmailAddress(const QString &key, bool *result) const
{
    QHash<QString, QString>::const_iterator it = _emailAddress.find(key);
    //Не нашли
    if (it == _emailAddress.end())
    {
        if (result != nullptr)
            *result = false;
        return QString();
    }
    //Нашли
    else
    {
        if (result != nullptr)
            *result = true;
        return *it;
    }
}

QVector<QPair<QString, QString> > ClientCard::getEmailList() const
{
    QVector<QPair<QString, QString> > result;
    for(QHash<QString, QString>::ConstIterator i = _emailAddress.begin(); i != _emailAddress.end(); ++i)
    {
        QPair<QString, QString> element;
        element.first = i.key();
        element.second = i.value();
        result.push_back(element);
    }
    return result;
}

void ClientCard::setEmailList(const QVector<QPair<QString, QString> > &par)
{
    _emailAddress.clear();
    for(QVector<QPair<QString, QString> >::const_iterator i = par.begin(); i != par.end(); ++i)
        _emailAddress.insert(i->first, i->second);
}

void ClientCard::addEmptyEmailNode()
{
    _emailAddress.insert("", "");
}

void ClientCard::loadEmailFromEdits()
{
    _emailAddress.clear();
    for(QVector<QPair<QLineEdit*, QLineEdit*> >::const_iterator i = emailEditList.begin(); i != emailEditList.end(); ++i)
        _emailAddress.insert(i->first->text(), i->second->text());
}

int ClientCard::getId() const
{
    return _id;
}

void ClientCard::setId(const int id)
{
    _id = id;
}
