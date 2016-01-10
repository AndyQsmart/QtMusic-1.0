#include "imagebutton.h"
#include <QPainter>
#include <QMouseEvent>

ImageButton::ImageButton(QWidget *parent) : QWidget(parent), cursorChangeAble(false)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

ImageButton::ImageButton(const QString &icon, int num, QWidget *parent)
    : QWidget(parent), pixmapNum(num), cursorChangeAble(false)
{
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pix(icon);
    for (int i = 0; i != pixmapNum; i++)
        pixmapList.append(pix.copy(i * pix.width() / pixmapNum, 0, pix.width() / pixmapNum, pix.height()));
    currentPixmap = pixmapList.at(0);
    setFixedSize(currentPixmap.size());
}

ImageButton::ImageButton(const QString &icon, bool isDivision, int num, QWidget *parent)
    : QWidget(parent), pixmapNum(num), cursorChangeAble(false)
{
    setAttribute(Qt::WA_TranslucentBackground);

    if (isDivision)
        setDivisionButtonInfo(icon, num);
    else
        setOneButtonInfo(icon, num);
}

void ImageButton::setOneButtonInfo(const QString &icon, int num)
{
    if (!pixmapList.isEmpty())
        pixmapList.clear();

    pixmapNum = num;
    QPixmap pix(icon);
    for (int i = 0; i != pixmapNum; i++)
        pixmapList.append(pix.copy(i * pix.width() / pixmapNum, 0, pix.width() / pixmapNum, pix.height()));
    currentPixmap = pixmapList.at(0);
    setFixedSize(currentPixmap.size());
}

void ImageButton::setDivisionButtonInfo(const QString &icon, int num)
{
    pixmapNum = num;
    pixmapList.append(QPixmap(icon + "_normal"));
    pixmapList.append(QPixmap(icon + "_hover"));
    pixmapList.append(QPixmap(icon + "_press"));
    if (pixmapNum == 4)
        pixmapList.append(QPixmap(icon + "_disable"));
    currentPixmap = pixmapList.at(0);
    setFixedSize(currentPixmap.size());
}

void ImageButton::setButtonStatus(ButtonStatus status)
{
    if (isEnabled())
    {
        switch (status)
        {
        case BUTTON_PRESSED :
            currentPixmap = pixmapList.at(2);
            break;
        case BUTTON_LEAVE :
            currentPixmap = pixmapList.at(0);
            break;
        case BUTTON_ENTER :
            currentPixmap = pixmapList.at(1);
            break;
        default :
            break;
        }
    }
    else
        if (pixmapNum == 4)
            currentPixmap = pixmapList.at(3);
    update();
}

void ImageButton::setCursorEnable(bool cursorEnable)
{
    cursorChangeAble = cursorEnable;
}

void ImageButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), currentPixmap);
}

void ImageButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        e->ignore();
    else
        setButtonStatus(BUTTON_PRESSED);
}

void ImageButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
    {
        e->ignore();
        return;
    }
    if (rect().contains(e->pos()))
    {
        if (isEnabled())
            emit clicked();
        setButtonStatus(BUTTON_ENTER);
        e->accept();
    }
    else
    {
        setButtonStatus(BUTTON_LEAVE);
        e->ignore();
    }
}

void ImageButton::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::EnabledChange)
    {
        if (!isEnabled() && (pixmapNum == 4))
            setButtonStatus(BUTTON_DISABLE);
    }
}
void ImageButton::enterEvent(QEvent *)
{
    emit mouseEnter();
    setButtonStatus(BUTTON_ENTER);
    if (cursorChangeAble)
    {
        currentCursor = cursor();
        setCursor(Qt::PointingHandCursor);
    }
}

void ImageButton::leaveEvent(QEvent *)
{
    setButtonStatus(BUTTON_LEAVE);
    if (cursorChangeAble)
        setCursor(currentCursor);
}
