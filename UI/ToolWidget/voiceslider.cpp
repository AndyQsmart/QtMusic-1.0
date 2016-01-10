#include "voiceslider.h"
#include <QPainter>
#include <QMouseEvent>

#include <QtDebug>

VoiceSlider::VoiceSlider(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(8);
    setAttribute(Qt::WA_TranslucentBackground);
    isPressed = false;
    currentPos = 0.0;

    //setFixedSize(400, 10);
}

qreal VoiceSlider::getCurrentPostion()
{
    return this->currentPos;
}

void VoiceSlider::moveTo(qreal pos)
{
    currentPos = pos;
    update();
}

void VoiceSlider::paintEvent(QPaintEvent *)
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

void VoiceSlider::enterEvent(QEvent *)
{
    preCursor = cursor();
    setCursor(Qt::PointingHandCursor);
}

void VoiceSlider::leaveEvent(QEvent *)
{
    setCursor(preCursor);
}

void VoiceSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;
    isPressed = true;
    currentPos = event->pos().x() / (this->width()+0.0);
    emit changeTo(currentPos);
    update();
}

void VoiceSlider::mouseReleaseEvent(QMouseEvent *event)
{
    isPressed = false;
    //emit changeTo(currentPos);
}

void VoiceSlider::mouseMoveEvent(QMouseEvent *e)
{
    if (isPressed)
    {
        currentPos = e->pos().x() / (this->width()+0.0);
        emit changeTo(currentPos);
        update();
    }
}
