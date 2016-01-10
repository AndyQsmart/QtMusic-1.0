#ifndef MYSYSTEMTRAYICON_H
#define MYSYSTEMTRAYICON_H

#include <QSystemTrayIcon>

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
};

#endif // MYSYSTEMTRAYICON_H
