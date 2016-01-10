#include "sliderwidget.h"
#include <QPainter>
#include <QMouseEvent>

#include <QtDebug>

SliderWidget::SliderWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(8);
    setAttribute(Qt::WA_TranslucentBackground);
    isAvailable = false;
    isPressed = false;
    currentPos = 0.0;

    //setFixedSize(400, 10);
}

void SliderWidget::setAvailable(bool flag)
{
    isAvailable = flag;
}

qreal SliderWidget::getCurrentPostion()
{
    return this->currentPos;
}

void SliderWidget::moveTo(qreal pos)
{
    if (isPressed) return;
    currentPos = pos;
    update();
}

void SliderWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(122, 139, 152));
    pen.setWidth(0.5);
    painter.setPen(pen);
    painter.setBrush(QColor(122, 139, 152));
    painter.drawRect(0, 0, this->width()-1, this->height()-1);

    pen.setColor(QColor(255, 220, 26));
    painter.setPen(pen);
    painter.setBrush(QColor(255, 220, 26));
    painter.drawRect(1, 1, this->width() * currentPos, this->height()-1);
}

void SliderWidget::enterEvent(QEvent *)
{
    preCursor = cursor();
    setCursor(Qt::PointingHandCursor);
}

void SliderWidget::leaveEvent(QEvent *)
{
    setCursor(preCursor);
}

void SliderWidget::mousePressEvent(QMouseEvent *event)
{
    if (!isAvailable) return;
    if (event->button() != Qt::LeftButton)
        return;
    isPressed = true;
    currentPos = event->pos().x() / (this->width()+0.0);
    update();
}

void SliderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!isAvailable) return;
    isPressed = false;
    emit changeTo(currentPos);
}

void SliderWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!isAvailable) return;
    if (isPressed)
    {
        currentPos = e->pos().x() / (this->width()+0.0);
        update();
    }
}
