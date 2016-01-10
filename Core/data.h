#ifndef DATA_H
#define DATA_H

#include "Core/musicinfo.h"
#include <QString>
#include <QThread>
#include <QQueue>

class Data
{
    public:
        Data();

        static bool connectData();
        static void tryConnectListList();
        static QQueue <QString> getListList();
        static bool addList(QString name);
        static void deleteList(QString name);
        static int getListId(QString name);

        static void tryConnectList(QString listName);
        static QString getMusicName(QString listName, int id);
        static QQueue <MusicInfo> getMusicList(QString listName);
        static void addMusicsToEnd(QString listName, QQueue<MusicInfo> musics);
        static void deleteMusic(QString listName, int row);
        static void deleteAllMusic(QString listName);

        static void tryConnectBaseInfo();
        static int getListCount();
        static void changeListCount(int cnt);
        static QString getCurrentBackground();
        static void changeCurrentBackground(QString dir);
        static int getSoundLevel();
        static void changeSoundLevel(int voice);
        static int getPlayMode();
        static void changePlayMode(int mode);
};

class MusicThread : public QThread
{
        Q_OBJECT

    public:
        MusicThread(QString listName, QQueue<MusicInfo> musics);

    protected:
        void run();

    private:
        QString listName;
        QQueue<MusicInfo> musics;
};

#endif // DATA_H
