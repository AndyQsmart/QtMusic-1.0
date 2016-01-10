#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QtWidgets/QLabel>

class QPushButton;
class ImageButton;
class LogoLabel;

class TopBar : public QWidget
{
        Q_OBJECT
    public:
        explicit TopBar(QWidget *parent = 0);

    protected:
        void mouseDoubleClickEvent(QMouseEvent *);
        void dealMouse();

    signals:
        void mouseEnter();
        void setMaxSize();
        void setMinSize();
        void closeWindow();
        void setBackgroud(QString);
        void aboutQtMusic();
        void quit();

    private:
        LogoLabel *logoLabel;
        ImageButton *skinButton;//皮肤按钮
        ImageButton *menuButton;//菜单按钮
        ImageButton *hideButton;//最小化按钮
        ImageButton *maxButton;//最大化按钮
        ImageButton *closeButton;//关闭按钮
};


class LogoLabel : public QLabel
{
        Q_OBJECT
    public:
        LogoLabel(QWidget *parent = 0);

    signals:
        void mouseEnter();

    private:
        void enterEvent(QEvent *e);//鼠标进入事件
};

#endif // TOPBAR_H
