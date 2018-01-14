#ifndef CLIENTCARD_H
#define CLIENTCARD_H

#include <QString>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QHash>
#include <QToolButton>
#include <QPushButton>
#include <QDateTime>

class ClientCard
{
private:
    //Ид в системе
    qint32 _id;
    //Id последней сгенерированой ссылки
    quint32 _lastLinkId;
    //Время в которое выполнился последний блок
    QDateTime _blockExecDateTime;
    //Номер блока в воронке
    qint32 _blockId;
    //Список ИД клиента в сервисах
    QHash<QString, qint64> _serviceId;
    //Номера телефонов
    QHash<QString, QString> _phoneNumber;
    //Email адреса
    QHash<QString, QString> _emailAddress;
    //ФИО Контактного лица
    QString _contactFace;
    //Название организации
    QString _organizationName;
    //Отображаемы текст
    QString _displayText;
    //Путь к файлу воронки
    QString _funnelFileName;
public:
    //Controls
    QPushButton * resetFunnelBtn;
    QLabel * funnelFileNameLbl;
    QLineEdit * funnelFileNameEdt;
    QToolButton * funnelFileNameTBtn;
    QLabel * idLbl;
    QLabel * displayLbl;
    QLabel * contactFaceLbl;
    QLineEdit * contactFaceEdit;
    QLabel * organizationNameLbl;
    QLineEdit * organizationNameEdit;
    QLabel * servicesLbl;
    QVector< QPair<QLineEdit*, QLineEdit*> > servicesEditList;
    QLabel * phonesLbl;
    QVector< QPair<QLineEdit*, QLineEdit*> > phonesEditList;
    QLabel * emailsLbl;
    QVector< QPair<QLineEdit*, QLineEdit*> > emailEditList;
    //C-tor
    ClientCard(const int id, const QString &displaytext);
    ClientCard();
    //Названия сервисов и их ид
    void addServiceId(const QString &key, const qint64 &val);
    qint64 getServiceId(const QString &key, bool *result = nullptr) const;
    QVector<QPair<QString, qint64> > getServiceList() const;
    void setServiceList(const QVector<QPair<QString, qint64> > &ptr);
    void addEmptyServiceNode();
    void loadServiceFromEdits();
    //Номера телефонов
    void addPhoneNumber(const QString &key, const QString &val);
    QString getPhoneNumber(const QString &key, bool *result = nullptr) const;
    QVector<QPair<QString, QString> > getPhoneList() const;
    void setPhoneList(const QVector<QPair<QString, QString> > &par);
    void addEmptyPhoneNode();
    void loadPhoneFromEdits();
    //Email адреса
    void addEmailAddress(const QString &key, const QString &val);
    QString getEmailAddress(const QString &key, bool *result = nullptr) const;
    QVector<QPair<QString, QString> > getEmailList() const;
    void setEmailList(const QVector<QPair<QString, QString> > &par);
    void addEmptyEmailNode();
    void loadEmailFromEdits();
    //Getters and setters
    int getId() const;
    void setId(const int id);
    QString getContactFace() const;
    void setContactFace(const QString &contactFace);
    QString getOrganizationName() const;
    void setOrganizationName(const QString &organizationName);
    QString getDisplayText() const;
    void setDisplayText(const QString &displayText);
    QString getFunnelFileName() const;
    void setFunnelFileName(const QString &funnelFileName);
    qint32 getBlockId() const;
    void setBlockId(const qint32 &blockId);
    QDateTime getBlockExecDateTime() const;
    void setBlockExecDateTime(const QDateTime &blockExecDateTime);
    quint32 getLastLinkId() const;
    void setLastLinkId(const quint32 &lastLinkId);
};

#endif // CLIENTCARD_H
