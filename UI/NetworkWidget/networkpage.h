#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QWidget>

class LabelButton;

class NetworkPage : public QWidget
{
        Q_OBJECT
    public:
        explicit NetworkPage(QWidget *parent = 0);

    signals:
        void mouseEnter();

    protected:
        void enterEvent(QEvent *e);//鼠标进入事件

    private:
        LabelButton *logo;
        LabelButton *search;
};

#endif // NETWORKPAGE_H
