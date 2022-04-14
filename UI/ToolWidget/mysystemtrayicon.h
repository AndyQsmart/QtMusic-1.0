#ifndef MYSYSTEMTRAYICON_H
#define MYSYSTEMTRAYICON_H

#include <QSystemTrayIcon>

class QMenu;

class MySystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    MySystemTrayIcon(QWidget *parent = 0);

public slots:
    void setPlayMode(int mode);

signals:
    void showWindow();
    void setMode(int mode);
    void quit();

private slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void setModeonemusic();
    void setModeonerep();
    void setModeturnmusic();
    void setModeallrep();
    void setModerandmusic();

private:
    QMenu *playMode;
};

#endif // MYSYSTEMTRAYICON_H
