#include "labelbutton.h"
#include <QMouseEvent>
#include <QDebug>

LabelButton::LabelButton(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(40, 40);
}

void LabelButton::setIcon(QString dir)
{
    QPixmap pix(dir);
    setPixmap(pix.scaled(this->size(), Qt::KeepAspectRatio));
}

void LabelButton::enterEvent(QEvent *e)
{
    this->setCursor(QCursor(Qt::PointingHandCursor));
}

void LabelButton::leaveEvent(QEvent *e)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void LabelButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        e->ignore();
    else
        emit clicked();
}
