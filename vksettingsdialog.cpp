#include "vksettingsdialog.h"
#include "ui_vksettingsdialog.h"

void VkSettingsDialog::_loadFromSettings(const VkSettings &set)
{
    ui->lineEdit->setText(set.sourceLink());
}

void VkSettingsDialog::_saveToSettings(VkSettings *set) const
{
    set->setSourceLink(ui->lineEdit->text());
    set->setAccessToken(ui->lineEdit_2->text());
    set->setUserId(ui->lineEdit_4->text().toLongLong());
    set->setUserEmail(ui->lineEdit_5->text());
}

VkSettingsDialog::VkSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VkSettingsDialog)
{
    ui->setupUi(this);
    VkSettings set;
    set.load();
    _loadFromSettings(set);
}

VkSettingsDialog::~VkSettingsDialog()
{
    delete ui;
}

void VkSettingsDialog::focusInEvent(QFocusEvent *event)
{
    QClipboard * clp = QApplication::clipboard();
    QString text(clp->text(QClipboard::Clipboard));
    if (!text.isEmpty() && ui->lineEdit->text().isEmpty())
        if (QMessageBox::question(this, "Буфер обмена", tr("Ваш буфер обмена содержит текст:\n%1\nВставить его?").arg(text)) == QMessageBox::Yes)
            ui->lineEdit->setText(text);
    event->accept();
}

void VkSettingsDialog::on_lineEdit_textChanged(const QString &arg1)
{
    QRegExp regExp("^.*access_token=([0-9,a-z]+)&.*user_id=(\\d+)&.*email=(.*)$");
    ui->buttonBox->setEnabled(regExp.exactMatch(arg1));
    ui->pushButton->setEnabled(ui->buttonBox->isEnabled());
    ui->lineEdit_2->setText(regExp.cap(1));
    ui->lineEdit_4->setText(regExp.cap(2));
    ui->lineEdit_5->setText(regExp.cap(3));
}

void VkSettingsDialog::on_buttonBox_accepted()
{
    VkSettings set;
    _saveToSettings(&set);
    set.save();
}

void VkSettingsDialog::on_pushButton_clicked()
{
    if (QMessageBox::question(this, "Предупреждение", "При тестировании текущие настройки будут сохранены, а старые удалены. Вы хотите продолжить?") == QMessageBox::Yes)
    {
        VkProcessor proc(this);
        VkSettings set;
        _saveToSettings(&set);
        set.save();
        proc.loadSettings();
        proc.getPersmissions();
        if (proc.permissionMessages() && proc.permissionOffline())
            QMessageBox::information(this, "Успех", "Наличие прав для отправки сообщений и работы в offline режиме успешно проверены");
        else
            QMessageBox::critical(this, "Error", "Для работы приложения необходимы права messages и offline");
    }
}
