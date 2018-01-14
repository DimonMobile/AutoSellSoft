#ifndef LINKGENERATOR_H
#define LINKGENERATOR_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QClipboard>
#include <QToolTip>
#include <QCursor>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>


#include "databasesettings.h"

namespace Ui {
class LinkGenerator;
}

class LinkGenerator : public QDialog
{
    Q_OBJECT
private:
    unsigned int _newLinkId;
    void _updateNewLinkId();
    static QString getLinkFromAnswer(const QString &src);
    static unsigned int getIdFromAnswer(const QString &src);
public:
    unsigned int getNewLinkId() const;
    static bool generateLink(const unsigned int &client_id, const QString &domain, const QString &redirrect_url, QString * hash = nullptr, QString * readyLink = nullptr, unsigned int * link_id = nullptr );
    explicit LinkGenerator(QWidget *parent = 0);
    ~LinkGenerator();
private slots:
    void on_pushButton_clicked();
    void on_toolButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::LinkGenerator *ui;
};

#endif // LINKGENERATOR_H
