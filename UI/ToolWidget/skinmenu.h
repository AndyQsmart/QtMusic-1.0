#ifndef SKINMENU_H
#define SKINMENU_H

#include <QWidget>

class IconButton;

class SkinMenu : public QWidget
{
    Q_OBJECT
    public:
        explicit SkinMenu(QWidget *parent = 0);

    signals:
        void changeSkin(QString dir);
    public slots:
        void clickSkin(int i, int j);
    private:
        IconButton **skinButton;
};

#endif // SKINMENU_H
