#ifndef TREEEDIT_H
#define TREEEDIT_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QShortcut>
#include <QHash>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QFileDialog>

#include "graphicsview.h"

namespace Ui {
class TreeEdit;
}

class TreeEdit : public QDialog
{
    Q_OBJECT
private:
    QShortcut * _fsShortcut;
    QLabel * _statusLabel;
    QToolButton * _fsBtn, * _saveBtn, * _openBtn;
    GraphicsView * _graphicsView;
    QGridLayout * _layout;
    QHBoxLayout * _buttonsLayout;
    GraphicsBlock * _currentBlock;
    //Граф
    bool _graphIsBuilded;
    QVector<QVector<int> > _graph;
    QHash<GraphicsBlock*, int> _ptrHash;
    QHash<int, GraphicsBlock*> _ptrHash2;
    //Для обработочек последних
    QString _errorString;
public:
    explicit TreeEdit(QWidget *parent = 0);
    GraphicsBlock *getCurrentBlock(const qint32 &blockId);
    qint32 getCurrentBlockId(GraphicsBlock * block);
    GraphicsBlock *nextPositiveBlock(const GraphicsBlock * block);
    GraphicsBlock *nextNegativeBlock(const GraphicsBlock * block);
    bool buildGraph();
    bool saveTreeToFile(const QString &fileName);
    bool loadFromFile(const QString &fileName);
    ~TreeEdit();
    QString getErrorString() const;
private:
    Ui::TreeEdit *ui;
public slots:
    void onNewInformationFromGraphicsView(QString str);
    void onFsButtonClicked();
    void onSaveBtnClicked();
    void onOpenBtnClicked();
};

#endif // TREEEDIT_H
