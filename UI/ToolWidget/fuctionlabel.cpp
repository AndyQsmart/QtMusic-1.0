#include "fuctionlabel.h"
#include <QPalette>

FuctionLabel::FuctionLabel(QWidget *parent)
    : QLabel(parent)
{
    whitePalette = new QPalette();
    whitePalette->setColor(QPalette::WindowText, QColor(255, 255, 255));
    yellowPalette = new QPalette();
    yellowPalette->setColor(QPalette::WindowText, QColor(255, 220, 26));
    this->setPalette(*whitePalette);
    this->setFont(QFont("FuctionLabel", 15, QFont::Thin));
    this->isPressed = false;
}

void FuctionLabel::setName(QString name)
{
    this->name = name;
}

void FuctionLabel::setPressed()
{
    this->isPressed = true;
    this->setPalette(*yellowPalette);
}

void FuctionLabel::setNotPressed()
{
    this->isPressed = false;
    this->setPalette(*whitePalette);
}

bool FuctionLabel::judgePressed()
{
    return this->isPressed;
}

void FuctionLabel::enterEvent(QEvent *e)//鼠标进入事件
{
    if (!this->isPressed) this->setPalette(*yellowPalette);
    emit mouseEnter();
}

void FuctionLabel::leaveEvent(QEvent *e)
{
    if (!this->isPressed) this->setPalette(*whitePalette);
}

void FuctionLabel::mousePressEvent(QMouseEvent *e)
{
    emit clicked(name);
}
