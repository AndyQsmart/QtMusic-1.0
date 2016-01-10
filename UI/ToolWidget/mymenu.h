#ifndef MYMENU_H
#define MYMENU_H

#include <QMenu>

class MyMenu : public QMenu
{
        Q_OBJECT
    public:
        explicit MyMenu(QWidget *parent = 0);

    public slots:
        void menuVisiable();
};

#endif // MYMENU_H
