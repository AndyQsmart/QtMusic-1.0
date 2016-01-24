#ifndef FUCTIONLABEL_H
#define FUCTIONLABEL_H

#include <QLabel>

class QPalette;

class FuctionLabel : public QLabel
{
        Q_OBJECT

    public:
        FuctionLabel(QWidget *parent = 0);
        void setName(QString name);
        void setPressed();
        void setNotPressed();
        bool judgePressed();

    signals:
        void mouseEnter();
        void clicked(QString name);

    private:
        void enterEvent(QEvent *e);//鼠标进入事件
        void leaveEvent(QEvent *e);//鼠标移出
        void mousePressEvent(QMouseEvent *e);//鼠标按下

    private:
        bool isPressed;
        QPalette *whitePalette;
        QPalette *yellowPalette;
        QString name;
};

#endif // FUCTIONLABEL_H
