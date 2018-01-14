#include "treeedit.h"
#include "ui_treeedit.h"

QString TreeEdit::getErrorString() const
{
    return _errorString;
}

TreeEdit::TreeEdit(QWidget *parent) :
    QDialog(parent),
    _currentBlock(nullptr),
    _graphIsBuilded(false),
    ui(new Ui::TreeEdit)
{
    ui->setupUi(this);
    _layout = new QGridLayout();
    setLayout(_layout);
    _buttonsLayout = new QHBoxLayout();
    _graphicsView = new GraphicsView(this);
    connect(_graphicsView, SIGNAL(newInformationMessageCreated(QString)), SLOT(onNewInformationFromGraphicsView(QString)));
    _layout->addWidget(_graphicsView, 1, 0);
    _statusLabel = new QLabel();
    _statusLabel->setWordWrap(true);
    _statusLabel->setText("Готово. Начните работу со щелчка правой кнопкой мыши.");
    _fsBtn = new QToolButton(this);
    _fsBtn->setIconSize(QSize(16, 16));
    _fsBtn->setIcon(QIcon(":/icons/fs.png"));
    connect(_fsBtn, SIGNAL(clicked(bool)), SLOT(onFsButtonClicked()));
    _saveBtn = new QToolButton(this);
    _saveBtn->setIconSize(QSize(16,16));
    _saveBtn->setIcon(QIcon(":/icons/floppy.png"));
    connect(_saveBtn, SIGNAL(clicked(bool)), SLOT(onSaveBtnClicked()));
    _openBtn = new QToolButton(this);
    _openBtn->setIconSize(QSize(16,16));
    _openBtn->setIcon(QIcon(":/icons/open.png"));
    connect(_openBtn, SIGNAL(clicked(bool)), SLOT(onOpenBtnClicked()));
    _layout->addWidget(_statusLabel, 2, 0);
    _buttonsLayout->addWidget(_fsBtn);
    _buttonsLayout->addWidget(_openBtn);
    _buttonsLayout->addWidget(_saveBtn);
    _layout->addLayout(_buttonsLayout, 0, 0);
    _fsShortcut = new QShortcut(this);
    _fsShortcut->setKey(Qt::Key_F11);
    connect(_fsShortcut, SIGNAL(activated()), SLOT(onFsButtonClicked()));
}

GraphicsBlock *TreeEdit::getCurrentBlock(const qint32 &blockId)
{
    //Если блок не задан изначально или нет такого блока, то возвращаемся к первому
    if (blockId == -1 || !_ptrHash2.contains(blockId))
    {

        QList<QGraphicsItem*> items = _graphicsView->scene()->items();
        for(QList<QGraphicsItem*>::const_iterator i = items.begin(); i != items.end(); ++i)
        {
            if ( ((*i)->flags() & QGraphicsItem::ItemIsSelectable) != QGraphicsItem::ItemIsSelectable ) continue;
            GraphicsBlock * block = (GraphicsBlock*)(*i);
            if (block->blockType() == TypeActionBegin)
            {
                _currentBlock = block;
                break;
            }
        }
    }
    else
        _currentBlock = _ptrHash2.value(blockId);
    return _currentBlock;
}

qint32 TreeEdit::getCurrentBlockId(GraphicsBlock *block)
{
    if (block == nullptr)
        return -1;
    return _ptrHash2.key(block, -1);
}

GraphicsBlock * TreeEdit::nextPositiveBlock(const GraphicsBlock *block)
{
    if (block == nullptr)
    {
        _errorString = tr("При поиске следующего положительного блока, мы не обнаружили блок-источник");
        return nullptr;
    }
    else if (block->yesBlock.isEmpty())
    {
        _errorString = tr("При поиске слудуюего положительного блока, мы его не нашли вовсе");
        return nullptr;
    }
    GraphicsBlock * remoteBlock = block->yesBlock.values()[0];
    if (remoteBlock == nullptr)
    {
        _errorString = tr("При поиске следующего положительно блока, указатель на него оказался убит");
        return nullptr;
    }
    return remoteBlock;
}

GraphicsBlock * TreeEdit::nextNegativeBlock(const GraphicsBlock *block)
{
    if (block == nullptr)
    {
        _errorString = tr("При поиске следующего отрицательного блока, мы не обнаружили блок-источник");
        return nullptr;
    }
    else if (block->yesBlock.isEmpty())
    {
        _errorString = tr("При поиске слудуюего отрицательного блока, мы его не нашли вовсе");
        return nullptr;
    }
    GraphicsBlock * remoteBlock = block->noBlock.values()[0];
    if (remoteBlock == nullptr)
    {
        _errorString = tr("При поиске следующего отрицательного блока, указатель на него оказался убит");
        return nullptr;
    }
    return remoteBlock;
}

bool TreeEdit::buildGraph()
{
    //Очистка имеющегося графа и хеша с указателями
    _graph.clear();
    _ptrHash.clear();
    //Построим хеш указателей
    Scene * scene = qobject_cast<Scene*>(_graphicsView->scene());
    QList<QGraphicsItem*> items = scene->items();
    int currentIndex(-1);
    for(QList<QGraphicsItem*>::const_iterator i = items.begin(); i != items.end(); ++i)
    {
        if ( ((*i)->flags() & QGraphicsItem::ItemIsSelectable) != QGraphicsItem::ItemIsSelectable ) continue;
        GraphicsBlock * block = (GraphicsBlock*)(*i);
        ++currentIndex;
        _ptrHash.insert(block, currentIndex);
    }
    //Теперь настроим размеры сипска
    int hashSize = _ptrHash.size();
    _graph.resize(hashSize);
    for(int i = 0 ; i < hashSize; ++i)
    {
        _graph[i].resize(hashSize);
        //Обнулим заодно на всякий случай
        for(int j = 0 ; j < hashSize; ++j)
            _graph[i][j] = 0;
    }
    //Когда сформировали, теперь можем заполнить
    for(QHash<GraphicsBlock*, int>::const_iterator i = _ptrHash.begin(); i != _ptrHash.end(); ++i)
    {
        int index = i.value();
        GraphicsBlock * blockPtr = i.key();
        for(QSet<GraphicsBlock*>::const_iterator j = blockPtr->yesBlock.begin(); j != blockPtr->yesBlock.end(); ++j)
        {
            //Если нет обратной связи, то пропускаем
            if (! (*j)->fromBlock.contains(blockPtr) ) continue;
            //Вносим изменения в графе
            int destIndex = _ptrHash.value(*j);
            _graph[index][destIndex] = 1;//1 - yes
        }
        for(QSet<GraphicsBlock*>::const_iterator j = blockPtr->noBlock.begin(); j != blockPtr->noBlock.end(); ++j)
        {
            //Если нет обратной связи, то пропускаем
            if (! (*j)->fromBlock.contains(blockPtr) ) continue;
            //Вносим изменения в графе
            int destIndex = _ptrHash.value(*j);
            _graph[index][destIndex] = 2;//2 - no
        }
    }
    _graphIsBuilded = true;
    //В результате выполнения функции у нас вроде как имеются "_graph" с массивом чисел и "_ptrHash" с деревом указателей
    return true;
}

bool TreeEdit::saveTreeToFile(const QString &fileName)
{
    if (!_graphIsBuilded) return false;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QDataStream dataStream(&file);
    dataStream.setVersion(QDataStream::Qt_5_9);
    int numberOfBlocks(_ptrHash.size());
    dataStream << numberOfBlocks;

    //Сохраняем информацию о блоках
    for(QHash<GraphicsBlock*, int>::const_iterator i = _ptrHash.begin(); i != _ptrHash.end(); ++i)
    {
        GraphicsBlock * block = i.key();
        if ( (block->flags() & QGraphicsItem::ItemIsSelectable) == QGraphicsItem::ItemIsSelectable )
        {
            dataStream << i.value();//index
            dataStream << block->x() << block->y();//cooords
            dataStream << (qint32)block->blockType();
            dataStream << block->text();
            dataStream << block->content();
        }
        if (dataStream.status() != QDataStream::Ok)
        {
            file.close();
            return false;
        }
    }
    //Запишем графчик теперь(жирафчик)
    for(int i = 0; i < numberOfBlocks; ++i)
        for(int j = 0 ; j < numberOfBlocks; ++j)
            dataStream << _graph[i][j];
    if (dataStream.status() != QDataStream::Ok)
    {
        file.close();
        return false;
    }
    file.close();
    qDebug() << "File loaded fully and successfully";
    return true;
}

bool TreeEdit::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) return false;
    QDataStream dataStream(&file);
    Scene *scene = (Scene*)_graphicsView->scene();
    //Очистка сцены
    scene->deleteArrows();
    scene->clear();
    //Очистим граф и хеш
    _graph.clear();
    _ptrHash.clear();
    //Загрузим...
    int numberOfBlocks;
    dataStream >> numberOfBlocks;
    //Настроим размеры графа
    _graph.resize(numberOfBlocks);
    for(int i = 0 ; i < numberOfBlocks; ++i)
        _graph[i].resize(numberOfBlocks);
    //Загрузим инфу о блоках
    QHash<int, GraphicsBlock*> itemHash;
    for(int i = 0 ; i < numberOfBlocks; ++i)
    {
        GraphicsBlock * block = new GraphicsBlock();
        int index;//Индекс нужен для восстановления цепочек в графе
        dataStream >> index;
        //Инсертем в хеш
        itemHash.insert(index, block);
        qreal x, y;
        dataStream >> x >> y;
        qint32 btype;
        dataStream >> btype;
        QString informationString;
        dataStream >> informationString;
        QString content;
        dataStream >> content;
        //Наполним блок, получеными данными
        block->setPos(x, y);
        block->setBlockType((BlockType)btype);
        block->setText(informationString);
        block->setContent(content);
        block->adjustSize();
        block->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        block->updateText();
        scene->addItem(block);
    }
    _ptrHash2 = itemHash;
    //Загрузим графчик
    for(int i = 0 ; i < numberOfBlocks; ++i)
        for(int j = 0 ; j < numberOfBlocks; ++j)
            dataStream >> _graph[i][j];
    //Восстановим связи из графчика-жирафчика
    for(int i = 0 ; i < numberOfBlocks; ++i)
    {
        GraphicsBlock * currentBlock = itemHash.value(i);
        for(int j = 0 ; j < numberOfBlocks; ++j)
        {
            GraphicsBlock * iteratedBlock = itemHash.value(j);
            if(_graph[i][j] == 1)//yes
            {
                currentBlock->yesBlock.insert(iteratedBlock);
                iteratedBlock->fromBlock.insert(currentBlock);
            }
            if(_graph[i][j] == 2)//no
            {
                currentBlock->noBlock.insert(iteratedBlock);
                iteratedBlock->fromBlock.insert(currentBlock);
            }
        }
    }
    file.close();
    return true;
}

TreeEdit::~TreeEdit()
{
    delete ui;
}

void TreeEdit::onNewInformationFromGraphicsView(QString str)
{
    _statusLabel->setText(str);
}

void TreeEdit::onSaveBtnClicked()
{
    QString fName = QFileDialog::getSaveFileName(this, "Сохранить таблицу в", "", "Funnel table resource (*.ftr)");
    if (fName.isEmpty()) return;
    if (!buildGraph())
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка построения графа");
        return;
    }
    //Тут нужно описать теперь процесс сохранения
    if (!saveTreeToFile(fName))
        QMessageBox::critical(this, "Error", "Ошибка сохранения");
}

void TreeEdit::onOpenBtnClicked()
{
    QString fName = QFileDialog::getOpenFileName(this, "Открыть таблицу","", "Funnel table resource (*.ftr)");
    if (fName.isEmpty()) return;
    if (!loadFromFile(fName))
        QMessageBox::critical(this, "Error", "Ошибка открытия");
}

void TreeEdit::onFsButtonClicked()
{
    Qt::WindowStates states = windowState();
    if ( (states & Qt::WindowFullScreen) == Qt::WindowFullScreen )
        setWindowState(Qt::WindowNoState);
    else
        setWindowState(Qt::WindowFullScreen);
}

