#include "datetimepickerdialog.h"
#include "ui_datetimepickerdialog.h"

DateTimePickerDialog::DateTimePickerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateTimePickerDialog)
{
    ui->setupUi(this);
}

int DateTimePickerDialog::getDateTime()
{
    return ui->spinBox->value();
}

void DateTimePickerDialog::setDateTime(const int &dateTime)
{
    ui->spinBox->setValue(dateTime);
}

DateTimePickerDialog::~DateTimePickerDialog()
{
    delete ui;
}

void DateTimePickerDialog::on_pushButton_clicked()
{
    ui->spinBox->setValue(12);
}

void DateTimePickerDialog::on_pushButton_2_clicked()
{
    ui->spinBox->setValue(24);
}

void DateTimePickerDialog::on_pushButton_3_clicked()
{
    ui->spinBox->setValue(48);
}
