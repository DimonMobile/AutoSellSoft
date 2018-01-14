#include "scene.h"

ItemSelectMode Scene::selMode() const
{
    return _selMode;
}

void Scene::setSelMode(const ItemSelectMode &selMode)
{
    _selMode = selMode;
    update();
}

void Scene::drawArrows(QList<QRectF> rect)
{
    Q_UNUSED(rect);
    deleteArrows();
    QList<QGraphicsItem*> itms = items();
    for(int i = 0 ; i < itms.size(); ++i)
    {
        if( (itms[i]->flags() & QGraphicsItem::ItemIsSelectable) == QGraphicsItem::ItemIsSelectable)
        {
            GraphicsBlock * gBlock = (GraphicsBlock*)itms[i];

            float posX = gBlock->pos().x() + gBlock->width()/2.0;
            float posY = gBlock->pos().y() + gBlock->height()/2.0;

            for(auto &j : gBlock->yesBlock)
            {
                if (!j->fromBlock.contains(gBlock)) continue;
                float destX = j->pos().x() + j->width() / 2.0;
                float destY = j->pos().y() + j->height() / 2.0;
                float angle = atan2(destY-posY, destX-posX);
                float dX = fabs(posX - destX);
                float dY = fabs(posY - destY);
                float l = (j->width() * dY)/(2*dX);
                if (l > j->height()/2.0) l = j->height()/2.0;
                float l1 = (dX*j->height())/(2*dY);
                if (l1 > j->width()/2.0) l1 = j->width()/2.0;
                float gyppo = sqrtf(l1*l1+l*l);
                float distance = sqrtf(dX*dX+dY*dY);
                float endX = posX + (distance-gyppo)*cos(angle);
                float endY = posY + (distance-gyppo)*sin(angle);
                QGraphicsItem * someItem = addLine(posX, posY, endX, endY, QPen(Qt::green));
                someItem->setZValue(-1);
                _arrows.push_back(someItem);
                someItem = addPolygon(QPolygonF() << QPointF(destX + gyppo*cos(angle+3.1415), destY + gyppo*sin(angle+3.1415)) << QPointF(destX + (gyppo+15.0)*cos(angle+3.1415+0.1), destY + (gyppo+15.0) * sin(angle+3.1415+0.1)) << QPointF(destX + (gyppo+15.0)*cos(angle+3.1415-0.1), destY + (gyppo+15.0) * sin(angle+3.1415-0.1)), QPen(Qt::black), QBrush(Qt::blue));
                _arrows.push_back(someItem);
            }
            for(auto &j : gBlock->noBlock)
            {
                if (!j->fromBlock.contains(gBlock)) continue;
                float destX = j->pos().x() + j->width() / 2.0;
                float destY = j->pos().y() + j->height() / 2.0;
                float angle = atan2(destY-posY, destX-posX);
                float dX = fabs(posX - destX);
                float dY = fabs(posY - destY);
                float l = (j->width() * dY)/(2*dX);
                if (l > j->height()/2.0) l = j->height()/2.0;
                float l1 = (dX*j->height())/(2*dY);
                if (l1 > j->width()/2.0) l1 = j->width()/2.0;
                float gyppo = sqrtf(l1*l1+l*l);
                float distance = sqrtf(dX*dX+dY*dY);
                float endX = posX + (distance-gyppo)*cos(angle);
                float endY = posY + (distance-gyppo)*sin(angle);
                QGraphicsItem * someItem = addLine(posX, posY, endX, endY, QPen(Qt::red));
                someItem->setZValue(-1);
                _arrows.push_back(someItem);
                someItem = addPolygon(QPolygonF() << QPointF(destX + gyppo*cos(angle+3.1415), destY + gyppo*sin(angle+3.1415)) << QPointF(destX + (gyppo+15.0)*cos(angle+3.1415+0.1), destY + (gyppo+15.0) * sin(angle+3.1415+0.1)) << QPointF(destX + (gyppo+15.0)*cos(angle+3.1415-0.1), destY + (gyppo+15.0) * sin(angle+3.1415-0.1)), QPen(Qt::black), QBrush(Qt::blue));
                _arrows.push_back(someItem);
            }
        }
    }
}

void Scene::deleteArrows()
{
    for(int i = 0 ; i < _arrows.size(); ++i)
        delete _arrows[i];
    _arrows.clear();
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *itm = itemAt(event->scenePos(), QTransform());
    if (itm == nullptr) return;
    GraphicsBlock * block(nullptr);
    if ( (itm->flags() & QGraphicsItem::ItemIsSelectable) == QGraphicsItem::ItemIsSelectable)
        block = (GraphicsBlock*)itm;
    if (block == nullptr) return;

    switch (block->blockType())
    {
    case TypeActionSendEmail:
    {
        bool isOk;
        QString newText(QInputDialog::getMultiLineText(qobject_cast<QWidget*>(this->parent()), "Email", "Введите в поле ниже полное содержание сообщения", block->content(), &isOk));
        if (isOk)
            block->setContent(newText);
        break;
    }
    case TypeActionSendVK:
    {
        bool isOk;
        QString newText(QInputDialog::getMultiLineText(qobject_cast<QWidget*>(this->parent()), "VK", "Введите в поле ниже сообщение в ВК", block->content(), &isOk));
        if (isOk)
            block->setContent(newText);
        break;
    }
    case TypeActionAsk:
    {
        bool isOk;
        QString newText(QInputDialog::getMultiLineText(qobject_cast<QWidget*>(this->parent()), "Вопрос", "Введите вопрос для оператора", block->content(), &isOk));
        if (isOk)
            block->setContent(newText);
        break;
    }
    case TypeActionWait:
    {
        DateTimePickerDialog * pickerDialog = new DateTimePickerDialog(qobject_cast<QWidget*>(this->parent()));
        if (!block->content().isEmpty())
            pickerDialog->setDateTime( block->content().toInt());

        if (pickerDialog->exec() == QDialog::Accepted)
        {
            int time = pickerDialog->getDateTime();
            block->setContent(QString::number(time));
            block->updateText();
        }
        delete pickerDialog;
    }
    default:
        break;
    }
}

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent),
    _selMode(ModeDefault)
{
    setBackgroundBrush(Qt::gray);
    connect(this, SIGNAL(changed(QList<QRectF>)), SLOT(drawArrows(QList<QRectF>)));
    qDebug() << "Backgroup succesufly set";
}
