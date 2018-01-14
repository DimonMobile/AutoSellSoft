#include "graphicsblock.h"

QString GraphicsBlock::content() const
{
    return _content;
}

void GraphicsBlock::setContent(const QString &content)
{
    _content = content;
}

void GraphicsBlock::adjustSize()
{
    _fitToText();
}

void GraphicsBlock::_fitToText()
{
    QFontMetrics metrics(_font);
    QSize textSize = metrics.size(0, _informationString);
    //Установим внутренние размеры в соответствии с размером текста и отступов
    _width = textSize.width() + LEFT_TEXT_MARGIN + RIGHT_TEXT_MARGIN;
    _height = textSize.height() + TOP_TEXT_MARGIN + BOTTOM_TEXT_MARGIN;
}

void GraphicsBlock::_initConstructor()
{
    _font = QFont("Arial");
    _font.setPointSize(14);
}

BlockType GraphicsBlock::blockType() const
{
    return _blockType;
}

void GraphicsBlock::setBlockType(const BlockType &blockType)
{
    _blockType = blockType;
}

qreal GraphicsBlock::width()
{
    return _width;
}

qreal GraphicsBlock::height()
{
    return _height;
}

QString GraphicsBlock::text()
{
    return _informationString;
}

void GraphicsBlock::setText(const QString &str)
{
    _informationString = str;
    _fitToText();
}

void GraphicsBlock::updateText()
{
    switch(_blockType)
    {
    case TypeActionWait:
        _informationString = QString("[%1ч]Таймер").arg(_content);
        setToolTip("Настройка длительности ожидания, имеет только положительный результат");
        break;
    case TypeActionSendEmail:
        _informationString = "Отправить Email";
        setToolTip("В качестве отправляемого email будет использоваться случайная почта из тех, которые есть у клиента");
        break;
    case TypeActionSendVK:
        _informationString = "Отправить сообщение VK";
        setToolTip("В качестве отправляемого сервиса будет выбран \"VK\", если доступ закрыт настройками приватности, или по иной причине сообщение не будет отправлено, цепочка пойдет по красной ветке");
        break;
    case TypeActionTransactionCheck:
        _informationString = "Проверка покупки";
        setToolTip("\t*НЕ ФУНКЦИОНИРУЕТ*");
        break;
    case TypeActionBegin:
        _informationString = "Начало";
        setToolTip("Задает начало цепочки");
        break;
    case TypeActionEnd:
        _informationString = "Конец";
        setToolTip("Задает конец цепочки");
        break;
    case TypeActionLinkCheck:
        _informationString = "Проверка перехода по ссылке";
        setToolTip("Проверяет переход по последней сгенерированной ссылке");
        break;
    case TypeActionAsk:
        _informationString = "Вопрос оператору";
        setToolTip("Останавливает процесс воронки, и следущее направление будет выбрано в зависииости от выбора оператора, которому будет передан этот вопрос");
        break;
    default:
        _informationString = "Что-то сломалось(((\nДаа баги они такие";
        break;
    }
    _fitToText();
}

GraphicsBlock::GraphicsBlock(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    _initConstructor();
}

GraphicsBlock::GraphicsBlock(const QString &str, QGraphicsItem *parent) : QGraphicsItem(parent)
{
    _initConstructor();
    setText(str);
}

QRectF GraphicsBlock::boundingRect() const
{
    return QRectF(0-BORDER_WIDTH/2.0, 0-BORDER_WIDTH/2.0, _width + BORDER_WIDTH/2.0, _height + BORDER_WIDTH/2.0);
}

void GraphicsBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen mainPen;

    if (!isSelected())
        mainPen.setColor(Qt::black);
    else
        mainPen.setColor(Qt::blue);
    mainPen.setWidth(3);
    painter->setPen(mainPen);
    //fill color
    switch(_blockType)
    {
    case TypeActionTransactionCheck:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(30, 30, 30));
        break;
    case TypeActionSendEmail:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(55,55,0));
        break;
    case TypeActionSendVK:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(70, 70 , 150));
        break;
    case TypeActionWait:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(200, 50, 50));
        break;
    case TypeActionBegin:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(0, 0, 255));
        break;
    case TypeActionEnd:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(0, 0, 255));
        break;
    case TypeActionLinkCheck:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(114, 21, 114));
        break;
    case TypeActionAsk:
        painter->fillRect(0.0, 0.0, _width, _height, QColor::fromRgb(65, 75, 112));
        break;
    default:
        painter->fillRect(0.0, 0.0, _width, _height, Qt::red);
        break;
    }
    painter->drawRect(0.0, 0.0, _width, _height);
#ifdef GRAPHICS_BLOCK_DEBUG
    //texjtdr
    mainPen.setWidth(0);
    mainPen.setColor(Qt::red);
    painter->setPen(mainPen);
    painter->drawRect(LEFT_TEXT_MARGIN, TOP_TEXT_MARGIN, _width - LEFT_TEXT_MARGIN - RIGHT_TEXT_MARGIN, _height - TOP_TEXT_MARGIN - BOTTOM_TEXT_MARGIN);
#endif
    //text
    mainPen.setColor(Qt::white);
    painter->setPen(mainPen);
    painter->setFont(_font);
    painter->drawText(LEFT_TEXT_MARGIN, TOP_TEXT_MARGIN, _width - LEFT_TEXT_MARGIN - RIGHT_TEXT_MARGIN, _height - TOP_TEXT_MARGIN - BOTTOM_TEXT_MARGIN, Qt::AlignLeft, _informationString);
    //sel
    if (isSelected())
    {
        painter->setOpacity(0.2);
        painter->fillRect(0.0, 0.0, _width, _height, Qt::blue);
    }
    //
    Scene * scene = (Scene*)this->scene();
    painter->setOpacity(0.3);
    if (scene->selMode() == ModeSelectTargetNo)
        painter->fillRect(0.0, 0.0, _width, _height, Qt::red);
    else if (scene->selMode() == ModeSelectTargetYes)
        painter->fillRect(0.0, 0.0, _width, _height, Qt::green);
}
