#include "vkadduserdialog.h"
#include "ui_vkadduserdialog.h"

VkAddUserDialog::VkAddUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VkAddUserDialog)
{
    ui->setupUi(this);
    _vkProcessor = new VkProcessor(this);
}

VkAddUserDialog::~VkAddUserDialog()
{
    delete ui;
}

void VkAddUserDialog::on_lineEdit_textEdited(const QString &arg1)
{
    //Current
    QRegExp regExp("^https://vk.com/(\\w+)$");
    if(regExp.exactMatch(arg1))
    {
        ui->lineEdit_2->setText(regExp.cap(1));
        ui->pushButton->setEnabled(true);
    }
    else
        ui->pushButton->setEnabled(false);

}

void VkAddUserDialog::on_pushButton_clicked()
{
    VkUser user;
    ui->progressBar->setMaximum(0);
    QApplication::processEvents();
    if (!_vkProcessor->getUser(ui->lineEdit_2->text(), &user))
        ui->label->setText("<font color=red>Error</font>");
    else
        ui->label->setText(QString("<b>Id: </b>%1<br><b>Имя: </b>%2<br><b>Фамилия: </b>%3<br><b>Первый номер: </b>%4<br><b>Второй номер: </b>%5").arg(user.userId).arg(user.firstName).arg(user.lastName).arg(user.first_phone).arg(user.second_phone));
    ui->progressBar->setMaximum(1);
}
