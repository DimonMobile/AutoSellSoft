#ifndef VKADDUSERDIALOG_H
#define VKADDUSERDIALOG_H

#include <QDialog>
#include <QRegExp>
#include <QDebug>

#include "vkprocessor.h"

namespace Ui {
class VkAddUserDialog;
}

class VkAddUserDialog : public QDialog
{
    Q_OBJECT
private:
    VkProcessor *_vkProcessor;
public:
    explicit VkAddUserDialog(QWidget *parent = 0);
    ~VkAddUserDialog();

private slots:
    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::VkAddUserDialog *ui;
};

#endif // VKADDUSERDIALOG_H
