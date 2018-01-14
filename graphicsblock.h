#ifndef GRAPHICSBLOCK_H
#define GRAPHICSBLOCK_H

#include <QGraphicsItem>
#include <QFontMetricsF>
#include <QFont>
#include <QPainter>
#include <QString>

#include "scene.h"

//#define GRAPHICS_BLOCK_DEBUG

#define BORDER_WIDTH        3
#define TOP_TEXT_MARGIN     10.0f
#define BOTTOM_TEXT_MARGIN  10.0f
#define LEFT_TEXT_MARGIN    10.0f
#define RIGHT_TEXT_MARGIN   10.0f

enum BlockType{
    TypeActionSendEmail,
    TypeActionSendVK,
    TypeActionTransactionCheck,
    TypeActionWait,
    TypeActionBegin,
    TypeActionEnd,
    TypeActionLinkCheck,
    TypeActionAsk
};

class GraphicsBlock : public QGraphicsItem
{
private:
    QFont _font;
    qreal _width, _height;
    QString _informationString;
    QString _content;
    void _fitToText();
    void _initConstructor();
    BlockType _blockType;
public:
    QSet<GraphicsBlock*> yesBlock;
    QSet<GraphicsBlock*> noBlock;
    QSet<GraphicsBlock*> fromBlock;
    qreal width();
    qreal height();
    QString text();
    void setText(const QString &str);
    void updateText();
    BlockType blockType() const;
    void setBlockType(const BlockType &blockType);
    QString content() const;
    void setContent(const QString &content);
    void adjustSize();
    GraphicsBlock(QGraphicsItem * parent = nullptr);
    GraphicsBlock(const QString &str, QGraphicsItem * parent = nullptr);
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GRAPHICSBLOCK_H
