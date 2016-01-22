#include "musiclist.h"
#include "UI/ToolWidget/mymenu.h"
#include <QScrollBar>
#include <QContextMenuEvent>
#include <QColor>
#include <QHeaderView>
#include <QException>

#include <QtDebug>

MusicList::MusicList(QWidget *parent)
    : QTableWidget(parent)
{
    this->setFrameStyle(QFrame::NoFrame);//去边框
    this->setStyleSheet("background:rgba(244,244,244,40%);"
                        "selection-background-color:rgba(128,128,128,40%);"
                        "selection-color:rgba(255,255,255,100%)");
    this->verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical"
            "{"
                "width:12px;"
                "background:rgba(0,0,0,0%);"
                "margin:0px,0px,0px,0px;"
                "padding-top:9px;"
                "padding-bottom:9px;"
            "}"
            "QScrollBar::handle:vertical"
            "{"
                "width:8px;"
                "background:rgba(0,0,0,25%);"
                " border-radius:4px;"
                "min-height:20;"
            "}"
            "QScrollBar::handle:vertical:hover"
            "{"
                "width:8px;"
                "background:rgba(0,0,0,50%);"
                " border-radius:4px;"
                "min-height:20;"
            "}"
            "QScrollBar::add-line:vertical"
            "{"
                "height:9px;width:8px;"
                "subcontrol-position:bottom;"
                "border-image:url(:/images/3.png);"
            "}"
            "QScrollBar::sub-line:vertical"
            "{"
                "height:9px;width:8px;"
                "subcontrol-position:top;"
                "border-image:url(:/images/1.png);"
            "}"
            "QScrollBar::add-line:vertical:hover"
            "{"
                "height:9px;width:8px;"
                "subcontrol-position:bottom;"
                "border-image:url(:/images/4.png);"
            "}"
            "QScrollBar::sub-line:vertical:hover"
            "{"
                "height:9px;width:8px;"
                "subcontrol-position:top;"
                "border-image:url(:/images/2.png);"
            "}"
            "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
            "{"
                "background:rgba(0,0,0,10%);"
                "border-radius:4px;"
            "}"
            );
    this->horizontalScrollBar()->hide();
    setColumnCount(2);
    setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中一行
    setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可修改
    setShowGrid(false);//格子线不显示
    verticalHeader()->setVisible(false);//垂直表头不可见
    horizontalHeader()->setVisible(false);//垂直表头不可见
    setFocusPolicy(Qt::NoFocus);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->resizeSection(0, 180);
    horizontalHeader()->resizeSection(1, 80);
    horizontalHeader()->setStretchLastSection(true);//占满表头

    MyMenu *menu = new MyMenu(this);

    QAction *play = new QAction("播放", menu);
    QAction *deleteMusic = new QAction("删除", menu);
    QAction *deleteAll = new QAction("清空列表", menu);
    connect(play, SIGNAL(triggered()), this, SLOT(playTheMusic()));
    connect(deleteMusic, SIGNAL(triggered()), this, SLOT(removeTheMusic()));
    connect(deleteAll, SIGNAL(triggered()), this, SLOT(removeAllMusic()));

    menu->addAction(play);
    menu->addAction(deleteMusic);
    menu->addAction(deleteAll);
    connect(this, SIGNAL(rightClicked()), menu, SLOT(menuVisiable()));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SIGNAL(playTheMusic(int)));
}

MusicList::~MusicList()
{
    qDebug() << "MusicList delete" << endl;
}

void MusicList::playTheMusic()
{
    int index = this->currentRow();
    emit playTheMusic(index);
}

void MusicList::removeTheMusic()
{
    int row = this->currentRow();
    if (row < highLightRow) highLightRow--;
    if (row == highLightRow) highLightRow = -1;

    for (int i = 0; i < 2; i++)
    {
        QTableWidgetItem *item = this->takeItem(row, i);
        delete item;
        item = Q_NULLPTR;
    }
    //虽然removeRow(int row)中的Item会被Qt自动删除,
    //但总觉得自己删除会好一些
    this->removeRow(row);
    emit removeTheMusic(row);
}

void MusicList::removeAllMusic()
{
    int row = this->rowCount()-1;
    //qDebug() << row << endl;
    while (row >= 0)
    {
        for (int i = 0; i < 2; i++)
        {
            QTableWidgetItem *item = this->takeItem(row, i);
            delete item;
            item = Q_NULLPTR;
        }
        //虽然removeRow(int row)中的Item会被Qt自动删除,
        //但总觉得自己删除会好一些
        this->removeRow(row);
        row--;
    }
    emit removeAllMusics();
}

void MusicList::setHighLight(int row)
{
    highLightRow = row;
    int columnCount = this->columnCount();
    QTableWidgetItem *item;
    for (int i = 0; i < columnCount; ++i)
    {
        item = this->item(row, i);
        item->setTextColor(QColor(255, 255, 255));
        item->setBackground(QBrush(QColor(128, 150, 244)));
    }
    qDebug() << "set HighLightRow" << highLightRow << endl;
}

void MusicList::removeHighLight()
{
    qDebug() << "HighLightRow" << highLightRow << endl;
    if (highLightRow == -1) return;
    if (highLightRow >= this->rowCount())
    {
        highLightRow = -1;
        return;
    }
    int columnCount = this->columnCount();
    for (int i = 0; i < columnCount; ++i)
    {
        QTableWidgetItem *item = this->item(highLightRow, i);
        item->setTextColor(QColor(0, 0, 0));
        item->setBackground(QBrush(QColor(0, 0, 0, 0)));
    }
    highLightRow = -1;
}

void MusicList::enterEvent(QEvent *e)//鼠标进入事件
{
    emit mouseEnter();
}

void MusicList::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();//得到窗口坐标
    QTableWidgetItem *item = this->itemAt(point);
    if(item != NULL)
        emit rightClicked();
    QWidget::contextMenuEvent(event);
}
