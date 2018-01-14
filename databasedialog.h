#ifndef DATABASEDIALOG_H
#define DATABASEDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

#include "databasesettings.h"

namespace Ui {
class DataBaseDialog;
}

class DataBaseDialog : public QDialog
{
    Q_OBJECT
private:
    DataBaseSettings _settings;
    QSqlDatabase _db;
public:
    explicit DataBaseDialog(QWidget *parent = 0);
    ~DataBaseDialog();
    void loadFromSettings(const DataBaseSettings * settings);
    void uploadToSettings(DataBaseSettings * settings) const;
    DataBaseSettings getSettings() const;
private slots:
    void on_pushButton_clicked();    
    void on_radioButton_toggled(bool checked);
    void on_radioButton_2_toggled(bool checked);

private:
    Ui::DataBaseDialog *ui;
};

#endif // DATABASEDIALOG_H
