#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QInputDialog>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>
#include <QDataStream>
#include <QGridLayout>
#include <QFile>
#include <QTimer>

#include "clientcard.h"
#include "mailaccountsdialog.h"
#include "treeedit.h"
#include "databasedialog.h"
#include "linkgenerator.h"
#include "vksettingsdialog.h"
#include "vkprocessor.h"
#include "vkadduserdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    //inline processors
    VkProcessor * _vkProcessor;
    //Automatically funnel
    QTimer * _intervalTimer;
    qint32  _intervalTimerAmount;
    //
    QString _currentFileName;
    QList<ClientCard> _clientCards;
    void _updateAmounts();
    void _displayClientInfo(const int index);
    void _fromEditsToCard(const int index);
    void _saveData();
    void _loadData();
    void _addLog(const QString &color, const QString &text);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_toolButton_clicked();
    void onAddServiceFieldsBtnClicked();
    void onAddPhoneFieldsBtnClicked();
    void onAddEmailFieldsBtnClicked();
    void onApplyBtnClicked();
    void onFunnelFileNameBrowseBtnClicked(bool checked);
    void onFunnelResetBtnClicked(bool checked);
    void onIntervalTimerTimeout();
    void on_spinBox_valueChanged(int arg1);
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_SMTP_triggered();
    void on_action_3_triggered();
    void on_action_MySQL_triggered();
    void on_action_4_triggered();
    void on_pushButton_clicked();    
    void on_pushButton_2_toggled(bool checked);
    void on_action_VK_triggered();

    void on_action_VK_2_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
