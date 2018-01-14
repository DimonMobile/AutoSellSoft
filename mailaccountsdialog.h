#ifndef MAILACCOUNTSDIALOG_H
#define MAILACCOUNTSDIALOG_H

#include <QDialog>
#include <QSslSocket>
#include <QDebug>
#include <QHostInfo>
#include <QProgressDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QFile>
#include <QEventLoop>
#include <QSharedPointer>

#include "smtpsettings.h"

enum MailState
{
    RecievingServer,
    RecievingHelo,
    SendUsername,
    SendPassword,
    SendMailFrom,
    SendRcptTo,
    SendDataKeyword,
    SendData,
    SendQuit,
    RecieveQuit
};

namespace Ui {
class MailAccountsDialog;
}

class MailAccountsDialog : public QDialog
{
    Q_OBJECT
private:
    QSslSocket * _tcpSocket;
    QProgressDialog * _progressDialog;
    QByteArray _tempData;
    bool _isSuccess;
    QString _rcptToString;
    MailState _mailState;
    void _abortCheck();
    void _startSendMail();
public:
    SMTPSettings getSettings();
    static bool isPosisitveCode(const QString &ans);
    static bool sendMail(const QString &server, const quint16 port, const QString &from_mail, const QString &to_mail, const bool authorize, const bool use_ssl, const QString &login, const QString &password, const QString &message_text);
    void fromSettings(const SMTPSettings &settings);
    explicit MailAccountsDialog(QWidget *parent = 0);
    ~MailAccountsDialog();
private slots:
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpStateChenged(QAbstractSocket::SocketState state);
    void onTcpError(QAbstractSocket::SocketError error);
    void onTcpEncrypted();
    void onTcpReadyRead();
    void on_buttonBox_accepted();
    void on_checkBox_stateChanged(int arg1);
    void on_pushButton_clicked();
    void on_buttonBox_rejected();

private:
    Ui::MailAccountsDialog *ui;
};

#endif // MAILACCOUNTSDIALOG_H
