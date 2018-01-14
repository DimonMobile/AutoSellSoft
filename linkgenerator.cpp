#include "linkgenerator.h"
#include "ui_linkgenerator.h"

void LinkGenerator::_updateNewLinkId()
{
    DataBaseSettings settings(this);
    settings.loadSettings();
    if (!settings.useWebInterface())
    {
        if (!QSqlDatabase::contains("main"))
        {
            QMessageBox::critical(this, "Error", "Подключение не настроено!");
            return;
        }
        QSqlDatabase db = QSqlDatabase::database("main");
        QString queryString;
        queryString = QString("SELECT AUTO_INCREMENT FROM INFORMATION_SCHEMA.tables WHERE TABLE_SCHEMA='%1' AND TABLE_NAME='%2'").arg(settings.dbName()).arg("funnel_links");
        QSqlQuery query(db);
        if (!query.exec(queryString))
        {
            QMessageBox::critical(this, "Error", "Ошибка выполнения скрипта");
            return;
        }
        query.first();
        QVariant result = query.value(0);
        if (result.isNull())
        {
            QMessageBox::critical(this, "Error", "Ошибка чтения");
            return;
        }
        bool isOk;
        _newLinkId = result.toUInt(&isOk);
        if (!isOk)
        {
            QMessageBox::critical(this, "Error", "Ошибка преобразования");
            return;
        }
        ui->label->setText(tr("Номер создаваемой ссылки: %1").arg(_newLinkId));
    }
    //Если используется WEB-интерфейс
    else
    {
        ui->label->setText(tr("При WEB-интерфейсе обнаружение недоступно!"));
    }
}

unsigned int LinkGenerator::getNewLinkId() const
{
    return _newLinkId;
}


bool LinkGenerator::generateLink(const unsigned int &client_id, const QString &domain, const QString &redirrect_url, QString *hash, QString *readyLink, unsigned int *link_id)
{
    DataBaseSettings settings;
    settings.loadSettings();
    if (!settings.useWebInterface())
    {
        if (!QSqlDatabase::contains("main"))    return false;
        QSqlDatabase db = QSqlDatabase::database("main");
        QString dbName = settings.dbName();
        QString queryString;
        queryString = QString("SELECT AUTO_INCREMENT FROM INFORMATION_SCHEMA.tables WHERE TABLE_SCHEMA='%1' AND TABLE_NAME='%2'").arg(dbName).arg("funnel_links");

        QSqlQuery query(db);
        if (!query.exec(queryString)) return false;
        if (!query.first()) return false;
        if (query.value(0).isNull()) return false;
        unsigned int newIdx = query.value(0).toUInt();
        QString md5Hash = QCryptographicHash::hash(QString("%1mebel%2").arg(newIdx).arg(client_id).toUtf8(), QCryptographicHash::Md5).toHex();
        queryString = QString("INSERT INTO %1.%2 (client_id, hash, redirrect_url, created) VALUES (%3, '%4', '%5', '%6')").arg(dbName).arg("funnel_links").arg(client_id).arg(md5Hash).arg(redirrect_url).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
        if (!query.exec(queryString)) return false;
        if (hash != nullptr)
            *hash = md5Hash;
        if (readyLink != nullptr)
            *readyLink = QString("http://%1/r.php?r=%2").arg(domain).arg(md5Hash);
        if (link_id != nullptr)
            *link_id = newIdx;
        return true;
    }
    //Web-interface
    else
    {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("client_id", QString::number(client_id));
        urlQuery.addQueryItem("redirrect_url", redirrect_url);
        urlQuery.addQueryItem("created", QDateTime::currentDateTime().toString(Qt::ISODate));
        urlQuery.addQueryItem("access_hash", settings.webInterfaceKey());
        QString queryString = urlQuery.toString();

        QNetworkAccessManager nManager;
        QNetworkReply * reply = nManager.get(QNetworkRequest(QUrl(tr("http://%1?%2").arg(settings.linkGeneratorUrl()).arg(queryString))));
        QSharedPointer<QNetworkReply> replyPtr(reply);
        QEventLoop loop;
        connect(&nManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
        loop.exec();
        if(reply->error() != QNetworkReply::NoError)    return false;
        QByteArray answerData = reply->readAll();
        if (!answerData.startsWith(QString("OK:LINK").toUtf8()))    return false;
        else
        {
            if (readyLink != nullptr)
                *readyLink = LinkGenerator::getLinkFromAnswer(answerData);
            if (link_id != nullptr)
                *link_id = LinkGenerator::getIdFromAnswer(answerData);
        }
        return true;
    }
}

QString LinkGenerator::getLinkFromAnswer(const QString &src)
{
    int startIdx = src.indexOf('{');
    ++startIdx;
    int endIdx = src.indexOf('}');
    return src.mid(startIdx, endIdx-startIdx);
}

unsigned int LinkGenerator::getIdFromAnswer(const QString &src)
{
    QString res = src.section('}', 1, 1);
    return res.toUInt();
}

LinkGenerator::LinkGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LinkGenerator)
{
    ui->setupUi(this);
    DataBaseSettings settings(this);
    settings.loadSettings();
    if (settings.useWebInterface())
        ui->lineEdit_2->setEnabled(false);
    _updateNewLinkId();
}

LinkGenerator::~LinkGenerator()
{
    delete ui;
}

void LinkGenerator::on_pushButton_clicked()
{
    int clientId = ui->spinBox->value();
    DataBaseSettings dbSettings(this);
    dbSettings.loadSettings();
    QString redirrectUrl = ui->lineEdit->text();
    QString domain = ui->lineEdit_2->text();
    QString md5Hash = QCryptographicHash::hash(QString("%1mebel%2").arg(_newLinkId).arg(clientId).toUtf8(), QCryptographicHash::Md5).toHex();
    if (!dbSettings.useWebInterface())
    {
        ui->lineEdit_3->setText(md5Hash);
    }
    else
        ui->lineEdit_3->setText("Предпросчёт хеша при WEB-интерфейсе невозможен");

    if (!dbSettings.useWebInterface())
        ui->lineEdit_4->setText(QString("http://%1/r.php?r=%2").arg(domain).arg(md5Hash));
    else
        ui->lineEdit_4->setText("Ссылка будет получена при её регистрации");
    ui->label_4->setText(QString::number(clientId));
    ui->lineEdit_5->setText(redirrectUrl);
    if (QSqlDatabase::contains("main") || dbSettings.useWebInterface() )
        ui->pushButton_2->setEnabled(true);
}

void LinkGenerator::on_toolButton_clicked()
{
    QClipboard * clp = QApplication::clipboard();
    clp->setText(ui->lineEdit_4->text());
    QToolTip::showText(ui->lineEdit_4->mapToGlobal(QPoint(0,0)) , "Ссылка успешно скопирована в буфер обмена", ui->lineEdit_4, QRect(), 1000);
}

void LinkGenerator::on_pushButton_2_clicked()
{
    QString queryString;
    DataBaseSettings settings(this);
    settings.loadSettings();
    bool useWebInterFace = settings.useWebInterface();
    if (!useWebInterFace)
    {
        queryString = QString("INSERT INTO %1.%2 (client_id, hash, redirrect_url, created) VALUES (%3, '%4', '%5', '%6')").arg(settings.dbName()).arg("funnel_links").arg(ui->label_4->text().toUInt()).arg(ui->lineEdit_3->text()).arg(ui->lineEdit_5->text()).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    }
    else
    {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("client_id", ui->label_4->text());
        urlQuery.addQueryItem("redirrect_url", ui->lineEdit_5->text());
        urlQuery.addQueryItem("created", QDateTime::currentDateTime().toString(Qt::ISODate));
        urlQuery.addQueryItem("access_hash", settings.webInterfaceKey());
        queryString = urlQuery.toString();
    }
    //Если используется прямой интерфейс
    if (!useWebInterFace)
    {
        QSqlDatabase db = QSqlDatabase::database("main");
        QSqlQuery query(db);
        if (!query.exec(queryString))
        {
            QMessageBox::critical(this, "Error", "Ошибка записи на сервер!\n" + query.lastError().databaseText());
            return;
        }
        QMessageBox::information(this, "Успех", "Успешная регистрация ссылки на сервере");
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->pushButton_2->setEnabled(false);
        _updateNewLinkId();
    }
    //Если используется WEB-интерфейс
    else
    {
        QNetworkAccessManager nManager(this);
        QNetworkReply * reply = nManager.get(QNetworkRequest(QUrl(tr("http://%1?%2").arg(settings.linkGeneratorUrl()).arg(queryString))));
        QSharedPointer<QNetworkReply> replyPtr(reply);
        //qDebug() << tr("http://%1?%2").arg(settings.linkGeneratorUrl()).arg(queryString);
        QEventLoop loop(this);
        connect(&nManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
        loop.exec();
        if(reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::critical(this, "Error", reply->errorString());
            return;
        }
        QByteArray answerData = reply->readAll();
        if (!answerData.startsWith(QString("OK:LINK").toUtf8()))
            QMessageBox::critical(this, "Error", answerData);
        else
            ui->lineEdit_4->setText(LinkGenerator::getLinkFromAnswer(answerData));
        ui->label->setText(tr("Сгенерирована ссылка: %1").arg(LinkGenerator::getIdFromAnswer(answerData)));
    }
}
