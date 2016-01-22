#ifndef MYSYSTEMTRAYICON_H
#define MYSYSTEMTRAYICON_H

#include <QSystemTrayIcon>

class QMenu;

class MySystemTrayIcon : public QSystemTrayIcon
{
        Q_OBJECT
    public:
        MySystemTrayIcon(QWidget *parent = 0);

    signals:
        showWindow();
        quit();

    private slots:
        void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

    private:
        QMenu *playMode;
};

#endif // MYSYSTEMTRAYICON_H
