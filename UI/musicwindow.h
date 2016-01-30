#ifndef MUSICWINDOW_H
#define MUSICWINDOW_H

#include "UI/ToolWidget/mainwindow.h"

class MySystemTrayIcon;
class TopBar;
class QStackedWidget;
class LyricLabel;
class MusicPage;
class BottomBar;
class MyPlayer;

class MusicWindow : public MainWindow
{
        Q_OBJECT

    public:
        MusicWindow(QWidget *parent = 0);
        ~MusicWindow();

    public slots:
        void setBackgroud(QString dir);
        void aboutQtMusic();

    private slots:
        void clickPlay();
        void playPre();
        void playNext();
        void playerStateChanged(int state);
        void musicChanged(QString listName, int index);
        void tryToCreateList(QString name);
        void addMusics();
        void removeTheMusic(QString listName, int index);
        void removeAllMusics(QString listName);
        void deleteList(QString name);

    private:
        void readData();
        void paintEvent(QPaintEvent *);
        void dealMouse();

    private:
        QPixmap currentBackground;
        MySystemTrayIcon *systemTrayIcon;
        TopBar *topBar;
        QStackedWidget *fuctionPage;
        LyricLabel *lyricLabel;
        MusicPage *musicPage;
        BottomBar *bottomBar;
        MyPlayer *player;
};

#endif // MUSICWINDOW_H
