#include "mymenu.h"
#include <QDesktopWidget>
#include <QApplication>

MyMenu::MyMenu(QWidget *parent) : QMenu(parent)
{
    this->setStyleSheet("QMenu{background:rgba(255,255,255,100%);}"
                        "QMenu::item{padding:6px 42px;}"
                        "QMenu::item:selected{background:rgba(0,128,180,80%);}");

}

void MyMenu::menuVisiable()
{
    QPoint pos = QCursor::pos();
    int width = 0, height = 0;
    int maxWidth = QApplication::desktop()->width();
    int maxHeight = QApplication::desktop()->height();
    setVisible(true);
    if (pos.x()+this->width() > maxWidth)
        width = -this->width();
    if (pos.y()+this->height() > maxHeight)
        height = -this->height();
    move(pos.x()+width, pos.y()+height);
}
