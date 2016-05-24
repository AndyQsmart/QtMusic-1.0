#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <QLabel>

class LabelButton : public QLabel
{
        Q_OBJECT
    public:
        LabelButton(QWidget *parent = 0);
        void setIcon(QString dir);

    signals:
        void clicked();

    protected:
        void enterEvent(QEvent *e);
        void leaveEvent(QEvent *e);
        void mousePressEvent(QMouseEvent *e);//鼠标按下
};

#endif // LABELBUTTON_H
