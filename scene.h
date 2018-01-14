#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QInputDialog>

#include "graphicsblock.h"
#include "datetimepickerdialog.h"

enum ItemSelectMode
{
    ModeDefault,
    ModeSelectTargetYes,
    ModeSelectTargetNo
};

class Scene : public QGraphicsScene
{
    Q_OBJECT
private:
    ItemSelectMode _selMode;
    QList<QGraphicsItem*> _arrows;
public:
    Scene(QObject * parent = nullptr);
    ItemSelectMode selMode() const;
    void setSelMode(const ItemSelectMode &selMode);
    void deleteArrows();
private slots:
    void drawArrows(QList<QRectF> rect);
    // QGraphicsScene interface
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
