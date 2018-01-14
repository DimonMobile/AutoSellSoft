#ifndef DATETIMEPICKERDIALOG_H
#define DATETIMEPICKERDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class DateTimePickerDialog;
}

class DateTimePickerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DateTimePickerDialog(QWidget *parent = 0);
    int getDateTime();
    void setDateTime(const int &dateTime);
    ~DateTimePickerDialog();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
private:
    Ui::DateTimePickerDialog *ui;
};

#endif // DATETIMEPICKERDIALOG_H
