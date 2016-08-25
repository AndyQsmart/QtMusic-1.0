#include "searchlist.h"
#include <QScrollBar>
#include <QHeaderView>

SearchList::SearchList(QWidget *parent)
        : QTableWidget(parent)
{
    this->setFrameStyle(QFrame::NoFrame);//去边框
    //this->setSelectionMode(QAbstractItemView::NoSelection);
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

    setColumnCount(3);
    setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中一行
    setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可修改
    setShowGrid(false);//格子线不显示
    verticalHeader()->setVisible(false);
    //horizontalHeader()->setVisible(false);
    //horizontalHeader()->setResizeContentsPrecision(QHeaderView::ResizeToContents);
    setFocusPolicy(Qt::NoFocus);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->resizeSection(0, 80);
    horizontalHeader()->setStretchLastSection(true);//占满表头
    //resizeColumnToContents(0);
}

void SearchList::addSong(const QString &name, const QString &artist, const QString &length)
{
    int now = this->rowCount();
    this->insertRow(now);
    QTableWidgetItem *itemName = new QTableWidgetItem(name);
    this->setItem(now, 0, itemName);
    QTableWidgetItem *itemArtist = new QTableWidgetItem(artist);
    this->setItem(now, 1, itemArtist);
    QTableWidgetItem *itemLength = new QTableWidgetItem(length);
    this->setItem(now, 2, itemLength);
}

void SearchList::clearSongs()
{
    int len = this->rowCount();
    for (int i = 0; i < len; ++i)
    {
        for (int j = 0; j < 3; ++ j)
        {
            QTableWidgetItem *item = this->takeItem(0, j);
            delete item;
        }
        this->removeRow(0);
    }
}

void SearchList::setSongs(QVector<QString> &name, QVector<QString> &artist, QVector<int> &length)
{
    int len = name.size();
    for (int i = 0; i < len; ++i)
    {
        this->insertRow(i);
        QTableWidgetItem *itemName = new QTableWidgetItem(name[i]);
        this->setItem(i, 0, itemName);
        QTableWidgetItem *itemArtist = new QTableWidgetItem(artist[i]);
        this->setItem(i, 1, itemArtist);
        QTableWidgetItem *itemLength = new QTableWidgetItem();
        this->setItem(i, 2, itemLength);
    }
}
