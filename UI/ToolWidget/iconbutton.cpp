#include "iconbutton.h"
#include <QPainter>
#include <QMouseEvent>

IconButton::IconButton(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    isPressed = false;
    isEnter = false;
}

IconButton::IconButton(const QString &iconDir, QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    pixmap = QPixmap(iconDir);
    setFixedSize(pixmap.size());
    isPressed = false;
    isEnter = false;
}

void IconButton::setTheIcon(const QString &iconDir)
{
    pixmap = QPixmap(iconDir);
    update();
}

void IconButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (isEnter)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(Qt::lightGray)));
        painter.drawEllipse(rect());
        painter.drawPixmap(rect(), pixmap);
    }
    else
        painter.drawPixmap(rect(), pixmap);
}

void IconButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        e->ignore();
    else
        isPressed = true;
}

void IconButton::mouseReleaseEvent(QMouseEvent *)
{
    if (isPressed)
    {
        emit clicked();
        emit clicked(whichI, whichJ);
    }
    isPressed = false;
}

void IconButton::enterEvent(QEvent *)
{
    isEnter = true;
    preCursor = cursor();
    setCursor(Qt::PointingHandCursor);
    update();
}

void IconButton::leaveEvent(QEvent *)
{
    isEnter = false;
    setCursor(preCursor);
    update();
}

void IconButton::setIJ(int i, int j)
{
    this->whichI = i;
    this->whichJ = j;
}
