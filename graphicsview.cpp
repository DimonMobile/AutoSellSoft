#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    _scene = new Scene(this);
    setScene(_scene);
    setDragMode(QGraphicsView::RubberBandDrag);
    //ScrollHandDrag
    connect(_scene, SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)), SLOT(onSceneFocusChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)));
}

void GraphicsView::repaintAll()
{
    _scene->update();
}

void GraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    if (_scene->selMode() != ModeDefault) return;
    QMenu *menu = new QMenu(this);
    QAction * act = new QAction();
    act->setText("Создать блок");
    act->setShortcut(QKeySequence("Ctrl+N"));
    menu->addAction(act);
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddBlockActionTriggered(bool)));

    menu->addSeparator();

    int itemsSelectedSize = scene()->selectedItems().size();
    Q_UNUSED(itemsSelectedSize);

    QGraphicsItem * focusedItem = _scene->focusItem();

    //Block connect menu actions
    act = new QAction();
    act->setText("Соединить положительно");
    if (focusedItem == nullptr || _scene->selMode() != ModeDefault)
        act->setEnabled(false);
    menu->addAction(act);
    connect(act, SIGNAL(triggered(bool)), SLOT(onActionConnectYesTriggered(bool)));

    act = new QAction(act);
    act->setText("Соединить отрицательно");
    if (focusedItem == nullptr || _scene->selMode() != ModeDefault)
        act->setEnabled(false);
    menu->addAction(act);
    connect(act, SIGNAL(triggered(bool)), SLOT(onActionConnectNoTriggered(bool)));

    menu->exec(QCursor::pos());
    delete menu;
}

void GraphicsView::onAddBlockActionTriggered(bool check)
{
    Q_UNUSED(check);

    QMenu * menu = new QMenu(this);

    QAction * act = new QAction();
    act->setText("Начальный блок");
    act->setIcon(QIcon(":/icons/begin.png"));
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddBeginBlockActionTriggered(bool)));
    menu->addAction(act);

    act = new QAction();
    act->setText("Конечный блок");
    act->setIcon(QIcon(":/icons/end.png"));
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddEndBlockActionTriggered(bool)));
    menu->addAction(act);

    menu->addSeparator();

    act = new QAction();
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddTransactionCheckBlockActionTriggered(bool)));
    act->setText("[В разработке]Проверка покупки");
    act->setEnabled(false);
    act->setIcon(QIcon(":/icons/cart.png"));
    menu->addAction(act);

    act = new QAction();
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddLinkCheckBlockActionTriggered(bool)));
    act->setText("Проверка перехода по ссылке");
    act->setIcon(QIcon(":/icons/link.png"));
    menu->addAction(act);

    menu->addSeparator();

    act = new QAction(this);
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddEmailMessageSendBlockActionTriggered(bool)));
    act->setText("Отправить Email");
    act->setIcon(QIcon(":/icons/mail.png"));
    menu->addAction(act);

    act = new QAction(this);
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddVkMessageSendBlockActionTriggered(bool)));
    act->setText("Отправить сообщение VK");
    act->setIcon(QIcon(":/icons/vk.png"));
    menu->addAction(act);

    menu->addSeparator();

    act = new QAction(this);
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddAskBlockActionTriggered(bool)));
    act->setText("Вопрос оператору");
    act->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
    menu->addAction(act);

    menu->addSeparator();

    act = new QAction(this);
    connect(act, SIGNAL(triggered(bool)), SLOT(onAddTimerBlockActionTriggered(bool)));
    act->setText("Таймер");
    act->setIcon(QIcon(":/icons/timer.png"));
    menu->addAction(act);    

    menu->exec(QCursor::pos());
    delete menu;
    //On create block action triggered
}

void GraphicsView::onAddTransactionCheckBlockActionTriggered(bool check)
{
    Q_UNUSED(check);
    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionTransactionCheck);
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onAddLinkCheckBlockActionTriggered(bool check)
{
    Q_UNUSED(check);
    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionLinkCheck);
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onAddVkMessageSendBlockActionTriggered(bool check)
{
    Q_UNUSED(check);
    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionSendVK);
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onAddEmailMessageSendBlockActionTriggered(bool check)
{
    Q_UNUSED(check);
    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionSendEmail);
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onAddTimerBlockActionTriggered(bool check)
{
    Q_UNUSED(check);
    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionWait);
    block->setContent("1");
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onAddBeginBlockActionTriggered(bool check)
{
    Q_UNUSED(check);

    QList<QGraphicsItem*> itmLst = _scene->items();
    for(QList<QGraphicsItem*>::const_iterator i = itmLst.begin(); i != itmLst.end(); ++i)
    {
        if ( ((*i)->flags() & QGraphicsItem::ItemIsSelectable) == QGraphicsItem::ItemIsSelectable )
        {
            GraphicsBlock * block = (GraphicsBlock*)(*i);
            if (block->blockType() == TypeActionBegin)
            {
                QMessageBox::critical(this, "Error", "Данный тип блоков уже присутствует!");
                return;
            }
        }
    }

    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionBegin);
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onAddEndBlockActionTriggered(bool check)
{
    Q_UNUSED(check);

    QList<QGraphicsItem*> itmLst = _scene->items();
    for(QList<QGraphicsItem*>::const_iterator i = itmLst.begin(); i != itmLst.end(); ++i)
    {
        if ( ((*i)->flags() & QGraphicsItem::ItemIsSelectable) == QGraphicsItem::ItemIsSelectable )
        {
            GraphicsBlock * block = (GraphicsBlock*)(*i);
            if (block->blockType() == TypeActionEnd)
            {
                QMessageBox::critical(this, "Error", "Данный тип блоков уже присутствует!");
                return;
            }
        }
    }

    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionEnd);
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onAddAskBlockActionTriggered(bool check)
{
    Q_UNUSED(check);
    QPointF pos = mapFromGlobal(QCursor::pos());
    pos = mapToScene(pos.toPoint());
    GraphicsBlock * block = new GraphicsBlock();
    block->setBlockType(TypeActionAsk);
    block->setContent("");
    block->updateText();
    block->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    block->setPos(pos);
    scene()->addItem(block);
}

void GraphicsView::onActionConnectYesTriggered(bool check)
{
    Q_UNUSED(check);
    _scene->setSelMode(ModeSelectTargetYes);
    emit newInformationMessageCreated(tr("Выберите блок, с которым будет соединен текущий для положительного результата выполнения"));
}

void GraphicsView::onActionConnectNoTriggered(bool check)
{
    Q_UNUSED(check);
    _scene->setSelMode(ModeSelectTargetNo);
    emit newInformationMessageCreated(tr("Выберите блок, с которым будет соединен текущий для отрицательного результата выполнения"));
}

void GraphicsView::onSceneFocusChanged(QGraphicsItem *newItem, QGraphicsItem *oldItem, Qt::FocusReason reason)
{
    if (reason != Qt::MouseFocusReason) return;
    if (_scene->selMode() == ModeDefault) return;
    if (oldItem == nullptr)
    {
        _scene->setSelMode(ModeDefault);
        emit newInformationMessageCreated(tr("Ошибка: вы не выбрали блок!"));
        return;
    }
    if (newItem == oldItem)
    {
        _scene->setSelMode(ModeDefault);
        emit newInformationMessageCreated(tr("Ошибка: невозможно соединить блок с самим собой!"));
        return;
    }
    if (newItem == nullptr)
    {
        _scene->setSelMode(ModeDefault);
        emit newInformationMessageCreated(tr("Соединение отменено!"));
        return;
    }
    GraphicsBlock * nBlock = (GraphicsBlock*)newItem;
    GraphicsBlock * oBlock = (GraphicsBlock*)oldItem;
    try
    {
        if (oBlock->fromBlock.contains(nBlock))
            throw tr("Взаимное соединение запрещено!");
        else if ( (nBlock->blockType() == TypeActionBegin) || (oBlock->blockType() == TypeActionEnd) )
            throw tr("Конечный блок должен являться конечным, а начальный начальным!");
        if (_scene->selMode() == ModeSelectTargetYes)
        {
            if (oBlock->yesBlock.size() > 0)
                throw tr("Не может быть больше одной исходящей ветки!");
            else if (oBlock->noBlock.contains(nBlock))
                throw tr("Невозможно выводить 2 результата в 1 блок!");

            oBlock->yesBlock.insert(nBlock);
        }
        else if (_scene->selMode() == ModeSelectTargetNo)
        {
            if (oBlock->noBlock.size() > 0)
                throw tr("Не может быть больше одной исходящей ветки!");
            else if (oBlock->yesBlock.contains(nBlock))
                throw tr("Невозможно выводить 2 результата в 1 блок!");

            //Проверка типов
            else if (oBlock->blockType() == TypeActionWait)
                throw tr("Таймер не может иметь отрицательного результата");
            else if (oBlock->blockType() == TypeActionBegin)
                throw tr("Начальный блок выполнится всегда!(Не может иметь отрицательного результата!)");
            else if (oBlock->blockType() == TypeActionSendEmail)
                throw tr("Попытка отправить сообщение будет выполнятся всегда(при ошибке процесс будет просто остановлен)");
            else if (oBlock->blockType() == TypeActionSendVK)
                throw tr("Попытка отправить сообщение будет выполнятся всегда(при ошибке процесс будет просто остановлен)");
            oBlock->noBlock.insert(nBlock);
        }
        emit newInformationMessageCreated(tr("Блоки соединены!"));
        nBlock->fromBlock.insert(oBlock);
    }
    catch (QString &e)
    {
        emit newInformationMessageCreated(QString("Error: %1").arg(e));
    }

    _scene->setSelMode(ModeDefault);
}
