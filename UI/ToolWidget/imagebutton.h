#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QWidget>

enum ButtonStatus
{
    BUTTON_FIRST,
    BUTTON_SCALED,
    BUTTON_ENTER,
    BUTTON_LEAVE,
    BUTTON_PRESSED,
    BUTTON_DISABLE
};

class ImageButton : public QWidget
{
        Q_OBJECT
    public:
        explicit ImageButton(QWidget *parent = 0);
        explicit ImageButton(const QString &icon, int num = 4, QWidget *parent = 0);
        explicit ImageButton(const QString &icon, bool isDivision, int num = 4, QWidget *parent = 0);
        void setOneButtonInfo(const QString &icon, int num = 4);
        void setDivisionButtonInfo(const QString &icon, int num = 3);
        void setCursorEnable(bool cursorEnable);
        void setButtonStatus(ButtonStatus status);

    signals:
        void clicked();
        void mouseEnter();

    protected:
        void enterEvent(QEvent *e);//鼠标进入事件
        void leaveEvent(QEvent *e);//鼠标移出
        void mousePressEvent(QMouseEvent *e);//鼠标按下
        void mouseReleaseEvent(QMouseEvent *e);//鼠标释放
        void paintEvent(QPaintEvent *e);//窗口重汇
        void changeEvent(QEvent *e);

    private:
        QPixmap currentPixmap;
        QList<QPixmap> pixmapList;
        int pixmapNum;
        QCursor currentCursor;
        bool cursorChangeAble;//控制鼠标进入的形状,默认关闭
};

#endif // IMAGEBUTTON_H
