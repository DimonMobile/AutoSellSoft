#include "mailaccountsdialog.h"
#include "ui_mailaccountsdialog.h"

void MailAccountsDialog::_abortCheck()
{
    if (_isSuccess)
    {
        ui->label->setPixmap(QPixmap(":/icons/ok.png"));
        qDebug() << "set ok icon";
    }
    else
    {
        ui->label->setPixmap(QPixmap(":/icons/fail.png"));
        qDebug() << "set fail icon";
    }
    ui->pushButton->setEnabled(true);
    if (_tcpSocket->isOpen())
        _tcpSocket->close();
    _progressDialog->hide();
}

void MailAccountsDialog::_startSendMail()
{
    _isSuccess = false;
    _tempData.clear();
    _mailState = RecievingServer;
    _progressDialog->setLabelText("Готовимся к тесту почты...");
}

SMTPSettings MailAccountsDialog::getSettings()
{
    SMTPSettings settings;
    settings.setHost(ui->lineEdit->text());
    settings.setPort(ui->lineEdit_2->text().toUShort());
    settings.setUsername(ui->lineEdit_3->text());
    settings.setPassword(ui->lineEdit_4->text());
    settings.setEmail(ui->lineEdit_5->text());
    settings.setAuthorize(ui->checkBox->isChecked());
    settings.setUseSsl(ui->checkBox_2->isChecked());
    return settings;
}

bool MailAccountsDialog::isPosisitveCode(const QString &ans)
{
    if (ans.isEmpty()) return false;
    if (ans.at(0) == '3' || ans.at(0) == '2' || ans.at(0) == '+')
        return true;
    else
        return false;
}

bool MailAccountsDialog::sendMail(const QString &server, const quint16 port, const QString &from_mail, const QString &to_mail, const bool authorize, const bool use_ssl, const QString &login, const QString &password, const QString &message_text)
{
    QSharedPointer<QSslSocket> socket(new QSslSocket());
    try
    {
        QByteArray currentData;
        qDebug() << "connecting to" << server << port;
        if (use_ssl)
            socket->connectToHostEncrypted(server, port);
        else
            socket->connectToHost(server, port);
        if(!socket->waitForConnected())
            throw tr("Ошибка соединения");
        qDebug() << "Connected to host";
        while(1)
        {
            if (!socket->waitForReadyRead())
                throw tr("Ошибка получения инфы о серве");
            currentData.append(socket->readAll());
            if (currentData.contains("\r\n")) break;
        }
        qDebug() << currentData;
        if (!isPosisitveCode(currentData))
            throw tr("Ошибка получения информации о сервере");
        QTextStream textStream(&currentData);
        int code;
        QString remoteServer;
        textStream >> code >> remoteServer;
        if (remoteServer.isEmpty())
            throw tr("Ошибка получения самого серера");

        socket->write(QString("EHLO %1\r\n").arg(remoteServer).toUtf8());
        if (!socket->waitForBytesWritten())
            throw tr("Ошибка отправки данных");
        currentData.clear();
        while(1)
        {
            if (!socket->waitForReadyRead())
                throw tr("Ошибка получения инфы о серве");
            currentData.append(socket->readAll());
            if (currentData.contains("\r\n")) break;
        }
        qDebug() << currentData;
        if (!isPosisitveCode(currentData))
            throw tr("Ошибка второго шага(код не положительный)");
        if (authorize)//Если нужна авторизация
        {
            //write
            socket->write(QString("AUTH LOGIN\r\n").toUtf8());
            if (!socket->waitForBytesWritten())
                throw tr("Ошибка отправки запроса на авторизацию");
            //read
            currentData.clear();
            while(1)
            {
                if (!socket->waitForReadyRead())
                    throw tr("Ошибка получения ответа об авторизации");
                currentData.append(socket->readAll());
                if (currentData.contains("\r\n")) break;
            }
            qDebug() << currentData;
            if (!isPosisitveCode(currentData))
                throw tr("Сервер плохо принял запроса на авторизацию");

            //write
            socket->write(QString("%1\r\n").arg(QString(login.toUtf8().toBase64())).toUtf8());
            if (!socket->waitForBytesWritten())
                throw tr("Ошибка отправки логина");
            //read
            currentData.clear();
            while(1)
            {
                if (!socket->waitForReadyRead())
                    throw tr("Ошибка получения ответа об отправке логина");
                currentData.append(socket->readAll());
                if (currentData.contains("\r\n")) break;
            }
            qDebug() << currentData;
            if (!isPosisitveCode(currentData))
                throw tr("Сервер плохо принял логин");

            //write
            socket->write(QString("%1\r\n").arg(QString(password.toUtf8().toBase64())).toUtf8());
            if (!socket->waitForBytesWritten())
                throw tr("Ошибка отправки пароля");
            //read
            currentData.clear();
            while(1)
            {
                if (!socket->waitForReadyRead())
                    throw tr("Ошибка получения ответа об отправке пароля");
                currentData.append(socket->readAll());
                if (currentData.contains("\r\n")) break;
            }
            qDebug() << currentData;
            if (!isPosisitveCode(currentData))
                throw tr("Сервер плохо принял пароль");
            else
                qDebug() << "Успешная авторизация";
        }
        //write
        socket->write(QString("MAIL FROM: %1\r\n").arg(from_mail).toUtf8());
        if (!socket->waitForBytesWritten())
            throw tr("Ошибка отправки информации об отправителе");
        //read
        currentData.clear();
        while(1)
        {
            if (!socket->waitForReadyRead())
                throw tr("Ошибка получения ответа от сервере при отправке информации об отправителе");
            currentData.append(socket->readAll());
            if (currentData.contains("\r\n")) break;
        }
        qDebug() << currentData;
        if (!isPosisitveCode(currentData))
            throw tr("Сервер плохо принял информацию об отправителе");

        //write
        socket->write(QString("RCPT TO: %1\r\n").arg(to_mail).toUtf8());
        if (!socket->waitForBytesWritten())
            throw tr("Ошибка отправки информации о получателе");
        //read
        currentData.clear();
        while(1)
        {
            if (!socket->waitForReadyRead())
                throw tr("Ошибка получения ответа от сервере при отправке информации о получателе");
            currentData.append(socket->readAll());
            if (currentData.contains("\r\n")) break;
        }
        qDebug() << currentData;
        if (!isPosisitveCode(currentData))
            throw tr("Сервер плохо принял информацию о получателе");

        //write
        socket->write(QString("DATA\r\n").toUtf8());
        if (!socket->waitForBytesWritten())
            throw tr("Ошибка отправки запроса на отправку данных");
        //read
        currentData.clear();
        while(1)
        {
            if (!socket->waitForReadyRead())
                throw tr("Ошибка получения ответа на готовность приема данных от сервера");
            currentData.append(socket->readAll());
            if (currentData.contains("\r\n")) break;
        }
        qDebug() << currentData;
        if (!isPosisitveCode(currentData))
            throw tr("Сервер плохо принял информацию о готовности сервера к приему данных");

        //write
        socket->write(QString("%1\r\n\r\n.\r\n").arg(message_text).toUtf8());
        if (!socket->waitForBytesWritten())
            throw tr("Ошибка отправки данных");
        //read
        currentData.clear();
        while(1)
        {
            if (!socket->waitForReadyRead())
                throw tr("Ошибка получения ответа об успехе передавчи данных");
            currentData.append(socket->readAll());
            if (currentData.contains("\r\n")) break;
        }
        qDebug() << currentData;
        if (!isPosisitveCode(currentData))
            throw tr("Сервер плохо принял данные");

        //write
        socket->write(QString("QUIT\r\n").toUtf8());
        if (!socket->waitForBytesWritten())
            throw tr("Ошибка завершения сеанса");
        //read
        currentData.clear();
        while(1)
        {
            if (!socket->waitForReadyRead())
                throw tr("Ошибка получения ответа от сервера о завершении сеанса");
            currentData.append(socket->readAll());
            if (currentData.contains("\r\n")) break;
        }
        qDebug() << currentData;
        if (!isPosisitveCode(currentData))
            throw tr("Сервер плохо принял информацию о завершении сеанса");
        qDebug() << "Успешный конец функции об отправке email сообщения";
    }
    catch(QString &e)
    {

        qWarning() << "MailSendError:" << e;
        return false;
    }
    return true;
}

void MailAccountsDialog::fromSettings(const SMTPSettings &settings)
{
    ui->lineEdit->setText(settings.getHost());
    ui->lineEdit_2->setText(QString::number(settings.getPort()));
    ui->lineEdit_3->setText(settings.getUsername());
    ui->lineEdit_4->setText(settings.getPassword());
    ui->lineEdit_5->setText(settings.getEmail());
    ui->checkBox->setChecked(settings.getAuthorize());
    ui->checkBox_2->setChecked(settings.getUseSsl());
}

MailAccountsDialog::MailAccountsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MailAccountsDialog)
{
    ui->setupUi(this);
    _tcpSocket = new QSslSocket(this);
    connect(_tcpSocket, SIGNAL(connected()), SLOT(onTcpConnected()));
    connect(_tcpSocket, SIGNAL(disconnected()), SLOT(onTcpDisconnected()));
    connect(_tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(onTcpStateChenged(QAbstractSocket::SocketState)));
    connect(_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onTcpError(QAbstractSocket::SocketError)));
    connect(_tcpSocket, SIGNAL(encrypted()), SLOT(onTcpEncrypted()));
    connect(_tcpSocket, SIGNAL(readyRead()), SLOT(onTcpReadyRead()));

    SMTPSettings settings;
    settings.load();
    fromSettings(settings);

    _progressDialog = new QProgressDialog(this);
    _progressDialog->setMinimum(0);
    _progressDialog->setMaximum(0);
    _progressDialog->setAutoClose(0);
    _progressDialog->reset();
}

MailAccountsDialog::~MailAccountsDialog()
{
    delete ui;
}

void MailAccountsDialog::onTcpConnected()
{
    qDebug() << "Connected to host";
    if(ui->checkBox_2->isChecked())
        _progressDialog->setLabelText("Соединение произошло успешно\nВыполняется обмен ключами...");
    else
    {
        _progressDialog->setLabelText("Соединение произошло успешно");
        _startSendMail();
    }
}

void MailAccountsDialog::onTcpDisconnected()
{
    qDebug() << "Disconnected from host";
    _abortCheck();
}

void MailAccountsDialog::onTcpStateChenged(QAbstractSocket::SocketState state)
{
    qDebug() << "Tcp state changed" << state;
    switch(state)
    {
    case QAbstractSocket::HostLookupState:
        _progressDialog->setLabelText("Поиск хоста...");
        break;
    case QAbstractSocket::ConnectingState:
        _progressDialog->setLabelText("Соединение...");
        break;
    case QAbstractSocket::UnconnectedState:
        _abortCheck();
        break;
    default:
        qDebug() << "Undefined state:" << state;
    }
}

void MailAccountsDialog::onTcpError(QAbstractSocket::SocketError error)
{
    qDebug() << "Tcp error:" << error << _tcpSocket->errorString();
    _abortCheck();
}

void MailAccountsDialog::onTcpEncrypted()
{
    qDebug() << "encrypted";
    _startSendMail();
}

void MailAccountsDialog::onTcpReadyRead()
{
    _tempData.append(_tcpSocket->readAll());
    if(!_tempData.contains("\r\n")) return;
    qDebug() << _tempData;
    switch(_mailState)
    {
    case RecievingServer:
        if (!isPosisitveCode(_tempData))
        {
            _abortCheck();
            QMessageBox::critical(this, "error", _tempData);
            return;
        }
        else
        {
            QTextStream strm(_tempData);
            int code;
            QString server;
            strm >> code >> server;
            if (server.isEmpty())
            {
                _abortCheck();
                QMessageBox::critical(this, "error", "Что-то невероятое");
                return;
            }
            if (ui->checkBox->isChecked())
                _mailState = RecievingHelo;
            else
                _mailState = SendMailFrom;
            qDebug() << "server:" << server;
            _tempData.clear();
            _tcpSocket->write(QString("EHLO %1\r\n").arg(server).toUtf8());
            _progressDialog->setLabelText(QString("Получаем информацию о сервере(%1)...").arg(server));
        }
        break;
    case RecievingHelo:
    {
        //Если авторизация включена
        _progressDialog->setLabelText(QString("Начинаем авторизацию..."));
        QString tmpStr = _tempData;
        if (tmpStr.isEmpty() || tmpStr.at(0) != '2')
        {
            _abortCheck();
            QMessageBox::critical(this, "error", "Сервер не хочет здороваться((");
            return;
        }
        _mailState = SendUsername;
        _tempData.clear();
        _tcpSocket->write("AUTH LOGIN\r\n");
        break;
    }
    case SendUsername:
    {
        _progressDialog->setLabelText(QString("Отправка данных..."));
        SMTPSettings settings = getSettings();
        QByteArray username = settings.getUsername().toUtf8().toBase64();
        username.append("\r\n");
        qDebug() << "username:" << username;
        _tempData.clear();
        _mailState = SendPassword;
        _tcpSocket->write(username);
        break;
    }
    case SendPassword:
    {
        _progressDialog->setLabelText(QString("Проверка авторизации..."));
        SMTPSettings settings = getSettings();
        QByteArray password = settings.getPassword().toUtf8().toBase64();
        password.append("\r\n");
        qDebug() << "password:" << password;
        _tempData.clear();
        _mailState = SendMailFrom;
        _tcpSocket->write(password);
        break;
    }
    case SendMailFrom:
    {
        SMTPSettings settings = getSettings();
        //Проверить авторизацию тут нужно
        if (_tempData.isEmpty())
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", "Ошибка авторизации!");
            return;
        }
        if (!isPosisitveCode(_tempData))
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", _tempData);
            return;
        }
        qDebug() << "Успешная авторизация";
        _tempData.clear();
        _mailState = SendRcptTo;
        _progressDialog->setLabelText("Успешная авторизация на сервере\nЗадаем отправителя...");
        _tcpSocket->write(QString("MAIL FROM: %1\r\n").arg(settings.getEmail()).toUtf8());
        break;
    }
    case SendRcptTo:
    {
        SMTPSettings settings = getSettings();
        //Проверить авторизацию тут нужно
        if (_tempData.isEmpty())
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", "Неопознанная ошибка!");
            return;
        }
        if (!isPosisitveCode(_tempData))
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", _tempData);
            return;
        }
        _tempData.clear();
        _mailState = SendDataKeyword;
        _progressDialog->setLabelText("Задаем получателя сообщения...");
        qDebug() << QString("RCPT TO: %1\r\n").arg(_rcptToString);
        _tcpSocket->write(QString("RCPT TO: %1\r\n").arg(_rcptToString).toUtf8());
        break;
    }
    case SendDataKeyword:
    {
        if (_tempData.isEmpty())
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", "Неопознанная ошибка!");
            return;
        }
        if (!isPosisitveCode(_tempData))
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", _tempData);
            return;
        }
        _tempData.clear();
        _mailState = SendData;
        _progressDialog->setLabelText("Делаем запрос на отправку данных...");
        _tcpSocket->write(QString("DATA\r\n").toUtf8());
        break;
    }
    case SendData:
    {
        if (_tempData.isEmpty())
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", "Неопознанная ошибка!");
            return;
        }
        if (!isPosisitveCode(_tempData))
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", _tempData);
            return;
        }
        _tempData.clear();
        _mailState = SendQuit;
        _progressDialog->setLabelText("Отправляем данные...");
        SMTPSettings settings;
        settings.load();
        _tcpSocket->write(QString("SUBJECT: LineFunnel Test\r\nFrom: Mobile <%2>\r\nContent-type: text/html;charset=UTF-8\r\n\r\n%1<br><br><br><font color=blue>Данно сообщение сгенерировано автоматически и отвечать на него не нужно.</font>\r\n.\r\n").arg("textData").arg(settings.getEmail()).toUtf8());
        break;
    }
    case SendQuit:
    {
        if (_tempData.isEmpty())
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", "Неопознанная ошибка!");
            return;
        }
        if (!isPosisitveCode(_tempData))
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", _tempData);
            return;
        }
        _tempData.clear();
        _mailState = RecieveQuit;
        _progressDialog->setLabelText("Заканчиваем передачу");
        _tcpSocket->write(QString("QUIT\r\n").toUtf8());
        break;
    }
    case RecieveQuit:
    {
        if (_tempData.isEmpty())
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", "Неопознанная ошибка!");
            return;
        }
        if (!isPosisitveCode(_tempData))
        {
            _abortCheck();
            QMessageBox::critical(this, "Error", "Сервер не хочет заканчивать!");
            return;
        }
        _isSuccess = true;
        _abortCheck();
        QMessageBox::information(this, "Информация", "Сообщение успешно отправлено!");
        break;
    }
    default:
        QMessageBox::warning(this, "Warning", "Что-то случилось в наших лесах");
        break;
    }
}

void MailAccountsDialog::on_buttonBox_accepted()
{
    SMTPSettings settings = getSettings();
    settings.save();
    accept();
}

void MailAccountsDialog::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->groupBox->setEnabled(true);
    else
        ui->groupBox->setEnabled(false);
}

void MailAccountsDialog::on_pushButton_clicked()
{
    bool isOk;
    _rcptToString = QInputDialog::getText(this, "Email", "Укажите email адрес получателя для теста", QLineEdit::Normal, "", &isOk);
    qDebug() << "Rcpt:" << _rcptToString;
    if (!isOk) return;
    _tempData.clear();
    _isSuccess = false;
    ui->pushButton->setEnabled(false);
    _progressDialog->setLabelText("Подготовка...");
    _progressDialog->show();
    SMTPSettings settings(getSettings());
    if (ui->checkBox_2->isChecked())
        _tcpSocket->connectToHostEncrypted(settings.getHost(), settings.getPort());
    else
        _tcpSocket->connectToHost(settings.getHost(), settings.getPort());
}

void MailAccountsDialog::on_buttonBox_rejected()
{
    reject();
}
