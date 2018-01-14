#include "databasedialog.h"
#include "ui_databasedialog.h"

DataBaseDialog::DataBaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataBaseDialog)
{
    ui->setupUi(this);
    _settings.loadSettings();
    loadFromSettings(&_settings);
    if (!QSqlDatabase::contains("main"))
        _db = QSqlDatabase::addDatabase("QMYSQL","main");
}

DataBaseDialog::~DataBaseDialog()
{
    _db = QSqlDatabase();
    delete ui;
}

void DataBaseDialog::loadFromSettings(const DataBaseSettings *settings)
{
    if (settings == nullptr) return;
    ui->lineEdit->setText(settings->userName());
    ui->lineEdit_2->setText(settings->password());
    ui->lineEdit_3->setText(settings->host());
    ui->spinBox->setValue(settings->port());
    ui->lineEdit_4->setText(settings->dbName());
    ui->lineEdit_5->setText(settings->linkGeneratorUrl());
    if (settings->useWebInterface())
    {
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
    }
    else
    {
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
    }
    ui->lineEdit_6->setText(settings->webInterfaceKey());
    ui->lineEdit_7->setText(settings->linkCheckerUrl());
}

void DataBaseDialog::uploadToSettings(DataBaseSettings *settings) const
{
    if (settings == nullptr) return;
    settings->setUserName(ui->lineEdit->text());
    settings->setPassword(ui->lineEdit_2->text());
    settings->setHost(ui->lineEdit_3->text());
    settings->setPort(ui->spinBox->value());
    settings->setDbName(ui->lineEdit_4->text());
    settings->setLinkGeneratorUrl(ui->lineEdit_5->text());
    settings->setUseWebInterface(ui->radioButton_2->isChecked()?true:false);
    settings->setWebInterfaceKey(ui->lineEdit_6->text());
    settings->setLinkCheckerUrl(ui->lineEdit_7->text());
}

DataBaseSettings DataBaseDialog::getSettings() const
{
    DataBaseSettings settings;
    uploadToSettings(&settings);
    return settings;
}

void DataBaseDialog::on_pushButton_clicked()
{
    if (!_db.isValid())
        qDebug() << "No valid";

    DataBaseSettings settings;
    uploadToSettings(&settings);

    _db.setUserName(settings.userName());
    _db.setPassword(settings.password());
    _db.setHostName(settings.host());
    if (settings.port() != 0)
        _db.setPort(settings.port());
    else
        _db.setPort(3306);//Default MySQL port
    _db.setDatabaseName(settings.dbName());
    if (!_db.open())
    {
        QMessageBox::critical(this, "Error", _db.lastError().text());
        ui->label->setPixmap(QPixmap(":/icons/fail.png"));
    }
    else
    {
        QMessageBox * mBox = new QMessageBox(this);
        mBox->setWindowTitle("Успех");
        mBox->setText("Соединение с базой данных произошло успешно!\nСписок доступных в базе данных таблиц можно просмотреть в деталях ниже.");
        mBox->setDetailedText(_db.tables().join('\n'));
        mBox->setIconPixmap(QPixmap(":/icons/ok.png"));
        mBox->exec();
        delete mBox;
        ui->label->setPixmap(QPixmap(":/icons/ok.png"));
    }
}

void DataBaseDialog::on_radioButton_toggled(bool checked)
{
    if (checked)
    {
        ui->groupBox_4->setEnabled(true);
        ui->groupBox_3->setEnabled(false);
    }
    else
    {
        ui->groupBox_4->setEnabled(false);
        ui->groupBox_3->setEnabled(true);
    }
}

void DataBaseDialog::on_radioButton_2_toggled(bool checked)
{
    if (!checked)
    {
        ui->groupBox_4->setEnabled(true);
        ui->groupBox_3->setEnabled(false);
    }
    else
    {
        ui->groupBox_4->setEnabled(false);
        ui->groupBox_3->setEnabled(true);
    }
}
