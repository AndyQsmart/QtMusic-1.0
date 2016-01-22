#include "mainwindow.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    resize(970, 700);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    //第一个参数是设置无边框。第二个参数是允许任务栏按钮右键菜单，第三个参数是允许最小化与还原。

    isLeftPressDown = false;
    isMaxSize = false;
    changeDir = NONE;
    setMaximumWidth(QApplication::desktop()->width());
    setMaximumHeight(QApplication::desktop()->height());
    setMinimumHeight(570);
    setMinimumWidth(985);
    setMouseTracking(true);//追踪鼠标
    //this->setStyleSheet("QDialog{background:url(:/bg_main.png)}");//设置样式背景色，可有可无

    this->setFocusPolicy(Qt::ClickFocus);//主窗口设置鼠标点击焦点，新建歌单时有用
}

void MainWindow::setMaxSize()
{
    if (this->isMaxSize)
    {
        this->isMaxSize = false;
        this->showNormal();
    }
    else
    {
        this->isMaxSize = true;
        this->showMaximized();
    }
}

void MainWindow::region(const QPoint &cursorGlobalPoint)
{
    if (isMaxSize)
        return;
    //获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
    QRect rect = this->rect();
    QPoint topleft = mapToGlobal(rect.topLeft());
    QPoint rightbottom = mapToGlobal(rect.bottomRight());

    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(topleft.x()+PADDING >= x &&
        topleft.x() <= x &&
        topleft.y()+PADDING >= y &&
        topleft.y() <= y)
    {
        // 左上角
        changeDir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));//设置鼠标形状
    }
    else if(x >= rightbottom.x()-PADDING &&
        x <= rightbottom.x() &&
        y >= rightbottom.y()-PADDING &&
        y <= rightbottom.y())
    {
        // 右下角
        changeDir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x <= topleft.x()+PADDING &&
        x >= topleft.x() &&
        y >= rightbottom.y()-PADDING &&
        y <= rightbottom.y())
    {
        //左下角
        changeDir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= rightbottom.x() &&
        x >= rightbottom.x()-PADDING &&
        y >= topleft.y() &&
        y <= topleft.y()+PADDING)
    {
        // 右上角
        changeDir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x <= topleft.x()+PADDING &&
        x >= topleft.x())
    {
        // 左边
        changeDir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(x <= rightbottom.x() &&
        x >= rightbottom.x()-PADDING)
    {
        // 右边
        changeDir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(y >= topleft.y() &&
        y <= topleft.y()+PADDING)
    {
        // 上边
        changeDir = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if(y <= rightbottom.y() &&
        y >= rightbottom.y()-PADDING)
    {
        // 下边
        changeDir = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else
    {
        // 默认
        changeDir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void MainWindow::loseMouse()
{
    changeDir = NONE;
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (isMaxSize)
        return;
    if(event->button() == Qt::LeftButton)
    {
        isLeftPressDown = false;
        if(changeDir != NONE)
        {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (isMaxSize)
        return;
    if (event->button() == Qt::LeftButton)
    {
        isLeftPressDown = true;
        if(changeDir != NONE)
            this->mouseGrabber();
        else
            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
    }
    else
        QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isMaxSize)
        return;
    QPoint globalPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint topleft = mapToGlobal(rect.topLeft());
    QPoint bottomright = mapToGlobal(rect.bottomRight());

    if(!isLeftPressDown)
    {
        this->region(globalPoint);
    }
    else if(changeDir != NONE)
    {
        QRect rMove(topleft, bottomright);
        switch(changeDir)
        {
            case LEFT:
                if(bottomright.x() - globalPoint.x() <= this->minimumWidth())
                    rMove.setX(topleft.x());
                else
                    rMove.setX(globalPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(globalPoint.x() - topleft.x());
                break;
            case UP:
                if(bottomright.y() - globalPoint.y() <= this->minimumHeight())
                    rMove.setY(topleft.y());
                else
                    rMove.setY(globalPoint.y());
                break;
            case DOWN:
                rMove.setHeight(globalPoint.y() - topleft.y());
                break;
            case LEFTTOP:
                if(bottomright.x() - globalPoint.x() <= this->minimumWidth())
                    rMove.setX(topleft.x());
                else
                    rMove.setX(globalPoint.x());
                if(bottomright.y() - globalPoint.y() <= this->minimumHeight())
                    rMove.setY(topleft.y());
                else
                    rMove.setY(globalPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(globalPoint.x() - topleft.x());
                rMove.setY(globalPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(globalPoint.x());
                rMove.setHeight(globalPoint.y() - topleft.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(globalPoint.x() - topleft.x());
                rMove.setHeight(globalPoint.y() - topleft.y());
                break;
            default:
                break;
        }
        this->setGeometry(rMove);
    }
    else
    {
        move(event->globalPos() - dragPosition);
        //event->accept();
    }
    QWidget::mouseMoveEvent(event);
}
