#ifndef VKSETTINGSDIALOG_H
#define VKSETTINGSDIALOG_H

#include <QDialog>
#include <QFocusEvent>
#include <QDebug>
#include <QClipboard>
#include <QMessageBox>

#include "vksettings.h"
#include "vkprocessor.h"

namespace Ui {
class VkSettingsDialog;
}

class VkSettingsDialog : public QDialog
{
    Q_OBJECT
private:
    void _loadFromSettings(const VkSettings &set);
    void _saveToSettings(VkSettings * set) const;
public:
    explicit VkSettingsDialog(QWidget *parent = 0);
    ~VkSettingsDialog();

private:
    Ui::VkSettingsDialog *ui;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_buttonBox_accepted();
    void on_pushButton_clicked();
};

#endif // VKSETTINGSDIALOG_H
