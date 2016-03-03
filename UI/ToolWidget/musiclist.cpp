#include "musiclist.h"
#include "UI/ToolWidget/mymenu.h"
#include <QScrollBar>
#include <QContextMenuEvent>
#include <QColor>
#include <QHeaderView>
#include <QException>

#include <QMimeData>
#include <QPainter>
#include <QtDebug>

MusicList::MusicList(QWidget *parent)
    : QListWidget(parent)
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
    //setColumnCount(2);
    setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中一行
    setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可修改
    //setShowGrid(false);//格子线不显示
    //verticalHeader()->setVisible(false);//垂直表头不可见
    //horizontalHeader()->setVisible(false);//垂直表头不可见
    //setFocusPolicy(Qt::NoFocus);
    //horizontalHeader()->setHighlightSections(false);
    //horizontalHeader()->resizeSection(0, 180);
    //horizontalHeader()->resizeSection(1, 80);
    //horizontalHeader()->setStretchLastSection(true);//占满表头
    this->setDragEnabled(true);
    this->setDropIndicatorShown(true);
    this->setAcceptDrops(true);

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
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickedEvent(QModelIndex)));
    //connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SIGNAL(playTheMusic(int)));
}

MusicList::~MusicList()
{
    qDebug() << "MusicList delete" << endl;
}

void MusicList::setArtist(int index, QString artist)
{
    QListWidgetItem *item = this->item(index);
    item->setSizeHint(QSize(item->sizeHint().width(), 60));
    MusicListItem *itemwidget = (MusicListItem *)this->itemWidget(item);
    itemwidget->setArtist(artist);
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
    QListWidgetItem *item = this->takeItem(row);
    //delete item;
    //item = Q_NULLPTR;
    //虽然removeRow(int row)中的Item会被Qt自动删除,
    //但总觉得自己删除会好一些
    this->removeItemWidget(item);
    delete item;
    //this->removeRow(row);
    emit removeTheMusic(row);
}

void MusicList::removeAllMusic()
{
    int row = this->count()-1;
    //qDebug() << row << endl;
    while (row >= 0)
    {
        QListWidgetItem *item = this->takeItem(row);
        this->removeItemWidget(item);
        delete item;
        //虽然removeRow(int row)中的Item会被Qt自动删除,
        //但总觉得自己删除会好一些
        //this->removeRow(row);
        row--;
    }
    emit removeAllMusics();
}

void MusicList::setHighLight(int row)
{
    highLightRow = row;
    QListWidgetItem *item = this->item(row);
    item->setSizeHint(QSize(item->sizeHint().width(), 60));
    MusicListItem *itemwidget = (MusicListItem *)this->itemWidget(item);
    itemwidget->setHighLight();
    /*
    highLightRow = row;
    int columnCount = this->columnCount();
    QTableWidgetItem *item;
    for (int i = 0; i < columnCount; ++i)
    {
        item = this->item(row, i);
        item->setTextColor(QColor(255, 255, 255));
        item->setBackground(QBrush(QColor(128, 150, 244)));
    }
    */
    qDebug() << "set HighLightRow" << highLightRow << endl;
}

void MusicList::removeHighLight()
{
    qDebug() << "HighLightRow" << highLightRow << endl;
    if (highLightRow == -1) return;
    if (highLightRow >= this->count())
    {
        highLightRow = -1;
        return;
    }
    QListWidgetItem *item = this->item(highLightRow);
    item->setSizeHint(QSize(item->sizeHint().width(), 30));
    MusicListItem *itemwidget = (MusicListItem *)this->itemWidget(item);
    itemwidget->removeHighLight();
    /*
    int columnCount = this->columnCount();
    for (int i = 0; i < columnCount; ++i)
    {
        QTableWidgetItem *item = this->item(highLightRow, i);
        item->setTextColor(QColor(0, 0, 0));
        item->setBackground(QBrush(QColor(0, 0, 0, 0)));
    }
    */
    highLightRow = -1;
}

void MusicList::enterEvent(QEvent *e)//鼠标进入事件
{
    emit mouseEnter();
}

void MusicList::doubleClickedEvent(QModelIndex index)
{
    playTheMusic(index.row());
}

void MusicList::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();//得到窗口坐标
    QListWidgetItem *item = this->itemAt(point);
    if(item != NULL)
        emit rightClicked();
    QWidget::contextMenuEvent(event);
}

void MusicList::dropEvent(QDropEvent *event)
{
    MusicList *source = (MusicList *)((void*)(event->source()));
    if (source && source == this)
    {
        QListWidgetItem *item = itemAt(event->pos());
        int from, to = this->row(item);
        if (to == -1) to = this->count()-1;
        from = this->currentRow();
        item = this->item(from);
        MusicListItem *itemwidget = (MusicListItem *)this->itemWidget(item);
        MusicListItem *newitemwidget = new MusicListItem(this);
        newitemwidget->setName(itemwidget->getName());
        newitemwidget->setArtist(itemwidget->getArtist());
        item = this->takeItem(from);
        this->removeItemWidget(item);
        delete item;
        item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 30));
        if (highLightRow == from)
        {
            newitemwidget->setHighLight();
            item->setSizeHint(QSize(item->sizeHint().width(), 60));
            highLightRow = to;
        }
        else if (highLightRow > from && highLightRow <= to) highLightRow--;
        else if (highLightRow < from && highLightRow >= to) highLightRow++;
        this->insertItem(to, item);
        this->setItemWidget(item, newitemwidget);
        qDebug() << from << " " << to << " " << highLightRow << endl;
        emit moveMusic(from, to);
    }
}

MusicListItem::MusicListItem(QWidget *parent) : QWidget(parent)
{
    //this->setFixedHeight(30);
    this->isHighLight = false;
}

void MusicListItem::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    int w = this->width();
    int h = this->height();
    QPainter painter(this);
    if (isHighLight)
    {
        QBrush brush(QColor(0, 0, 0, 50));
        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, w, h);
        QPen pen(QColor(255, 255, 255));
        painter.setPen(pen);
        painter.drawText(QRectF(10, 0, w, h/2.0), Qt::AlignLeft | Qt::AlignVCenter, name);
        painter.drawText(QRectF(10, h/2.0, w, h/2.0), Qt::AlignLeft | Qt::AlignVCenter, artist);
    }
    else
    {
        painter.drawText(QRectF(10, 0, w*3.0/5, h), Qt::AlignLeft | Qt::AlignVCenter, name);
        painter.drawText(QRectF(w*3.0/5, 0, w*2.0/5-5, h), Qt::AlignRight | Qt::AlignVCenter, artist);
    }
}

void MusicListItem::setName(QString name)
{
    this->name = name;
}

void MusicListItem::setArtist(QString artist)
{
    this->artist = artist;
}

QString MusicListItem::getName()
{
    return this->name;
}

QString MusicListItem::getArtist()
{
    return this->artist;
}

void MusicListItem::setHighLight()
{
    isHighLight = true;
}

void MusicListItem::removeHighLight()
{
    isHighLight = false;
}
