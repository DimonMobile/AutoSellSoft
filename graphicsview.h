#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QCursor>
#include <QMessageBox>
#include <QIcon>
#include <QStyle>
#include <QApplication>

#include "graphicsblock.h"

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
private:
    Scene * _scene;
public:
    GraphicsView(QWidget * parent = nullptr);
    void repaintAll();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private slots:
    void onAddBlockActionTriggered(bool check);
    void onAddTransactionCheckBlockActionTriggered(bool check);
    void onAddLinkCheckBlockActionTriggered(bool check);
    void onAddVkMessageSendBlockActionTriggered(bool check);
    void onAddEmailMessageSendBlockActionTriggered(bool check);
    void onAddTimerBlockActionTriggered(bool check);
    void onAddBeginBlockActionTriggered(bool check);
    void onAddEndBlockActionTriggered(bool check);
    void onAddAskBlockActionTriggered(bool check);
    void onActionConnectYesTriggered(bool check);
    void onActionConnectNoTriggered(bool check);
    void onSceneFocusChanged(QGraphicsItem *newItem,QGraphicsItem* oldItem,Qt::FocusReason reason);
signals:
    void newInformationMessageCreated(QString);
};

#endif // GRAPHICSVIEW_H
