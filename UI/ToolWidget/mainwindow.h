#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

//PADDING用于设置改变窗口时鼠标侦测的边距
#define PADDING 4
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LEFTTOP,
    LEFTBOTTOM,
    RIGHTBOTTOM,
    RIGHTTOP,
    NONE
};

class MainWindow : public QWidget
{
        Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = 0);

    public slots:
        void loseMouse();//当鼠标焦点被子组件获取时，需要还原鼠标形状和changeDir。
        void setMaxSize();

    private:
        void region(const QPoint &cursorGlobalPoint);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);



    protected:
        bool isMaxSize;
        bool isLeftPressDown;//判断左键是否按下
        QPoint dragPosition;//窗口移动拖动时需要记住的点
        Direction changeDir;//窗口大小改变时，记录改变方向
};

#endif // MAINWINDOW_H
