#include "listlist.h"
#include "UI/ToolWidget/mymenu.h"
#include <QScrollBar>
#include <QContextMenuEvent>
#include <QColor>
#include <QHeaderView>
#include <QLineEdit>

ListList::ListList(QWidget *parent)
    : QTableWidget(parent)
{
    this->setFrameStyle(QFrame::NoFrame);//去边框
    this->setSelectionMode(QAbstractItemView::NoSelection);
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

    setColumnCount(1);
    setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中一行
    setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可修改
    setShowGrid(false);//格子线不显示
    verticalHeader()->setVisible(false);//垂直表头不可见
    horizontalHeader()->setVisible(false);//垂直表头不可见
    setFocusPolicy(Qt::NoFocus);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->resizeSection(0, 80);
    horizontalHeader()->setStretchLastSection(true);//占满表头
    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(clickEvent()));

    MyMenu *listMenu = new MyMenu(this);
    QAction *addList = new QAction("创建列表", listMenu);
    QAction *rename = new QAction("重命名列表", listMenu);
    QAction *deleteList = new QAction("删除列表", listMenu);
    connect(addList, SIGNAL(triggered(bool)), this, SLOT(wantToCreateList()));
    connect(deleteList, SIGNAL(triggered(bool)), this, SLOT(deleteList()));
    listMenu->addAction(addList);
    listMenu->addAction(rename);
    listMenu->addAction(deleteList);
    connect(this, SIGNAL(rightClicked()), listMenu, SLOT(menuVisiable()));
}

void ListList::wantToCreateList()
{
    QLineEdit *newList = new QLineEdit();
    int cnt = rowCount();
    insertRow(cnt);
    setCellWidget(cnt, 0, newList);
    newList->setFocus();
    connect(newList, SIGNAL(editingFinished()), this, SLOT(tryToCreateList()));
}

void ListList::tryToCreateList()
{
    QLineEdit *item;
    int cnt = this->rowCount();
    item = (QLineEdit *)this->cellWidget(cnt-1, 0);
    this->removeRow(cnt-1);
    emit tryToCreateList(item->text());
}

void ListList::createList(QString name)
{
    int cnt = this->rowCount();
    QTableWidgetItem *newList = new QTableWidgetItem(name);
    newList->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->insertRow(cnt);
    this->setItem(cnt, 0, newList);
    this->setCurrentCell(cnt, 0);
}

QString ListList::getHighLightRowName()
{
    QTableWidgetItem *item = this->item(getHighLightRow(), 0);
    return item->text();
}

void ListList::deleteList()
{
    int row = currentRow();
    int now = getHighLightRow();
    if (row == 0) return;
    if (row < now) highLightRow--;
    if (now == row)
    {
        this->setHighLight(0);
        emit changeList(0);
    }
    QTableWidgetItem *item = this->takeItem(row, 0);
    QString name = item->text();
    //delete item;
    //item = Q_NULLPTR;
    //qDebug() << now << " " << row << endl;
    removeRow(row);
    emit deleteList(name);
    emit deleteList(row);
}

void ListList::setHighLight(int row)
{
    if (highLightRow != -1)
        removeHighLight(highLightRow);
    highLightRow = row;
    int columnCount = this->columnCount();
    QTableWidgetItem *item;
    for (int i = 0; i < columnCount; ++i)
    {
        item = this->item(row, i);
        item->setTextColor(QColor(255, 255, 255));
        item->setBackground(QBrush(QColor(128, 150, 244)));
    }
}

void ListList::removeHighLight(int row)
{
    int columnCount = this->columnCount();
    for (int i = 0; i < columnCount; ++i)
    {
        QTableWidgetItem *item = this->item(row, i);
        item->setTextColor(QColor(0, 0, 0));
        item->setBackground(QBrush(QColor(0, 0, 0, 0)));
    }
}

int ListList::getHighLightRow()
{
    return highLightRow;
}

void ListList::enterEvent(QEvent *e)//鼠标进入事件
{
    emit mouseEnter();
}

void ListList::clickEvent()
{
    int row = currentRow();
    setHighLight(row);
    emit changeList(row);
}

void ListList::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->pos();//得到窗口坐标
    QTableWidgetItem *item = this->itemAt(point);
    if(item != NULL)
    {
        clickEvent();
        emit rightClicked();
    }
    QWidget::contextMenuEvent(event);
}
