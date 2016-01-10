#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QQueue>
#include <QUrl>
#include <QMediaPlayer>
#include "Core/musicinfo.h"

class MyPlayer : public QObject
{
        Q_OBJECT

    public:
        enum PlyaerState
        {
            NOMUSIC,
            PAUSE,
            PLAYING
        };

    public:
        MyPlayer();
        void setPlay();
        void setPause();
        void setCurrentIndex(int index);
        int getCurrentIndex();
        int getPlayerState();
        int mediaCount();
        void addMedia(QString listName, QUrl url);
        void clear(QString listName);

    public slots:
        void setVoice(int voice);
        void setPlayMode(int mode);
        void addMusics(QString name, QQueue<MusicInfo> q);
        void removeTheMusic(QString listName, int index);
        void removeAllMusics(QString listName);
        void setPosition(qint64 pos);
        void playNext(QMediaPlayer::MediaStatus state);
        void playTheMusic(QString listName, int index);
        bool playPre();
        bool playNext();
        void play();
        void pause();

    private slots:
        void musicChanged();
        void stateChanged(int state);

    signals:
        void durationChanged(qint64);
        void musicChanged(QString listName, int index);
        void positionChanged(qint64);
        void musicNotAvailable();
        void playerStateChanged(int state);

    private:
        QMediaPlayer player;
        QString currentList;
        QMap<QString, QVector <QUrl> > list;
        int playMode;
        int currentindex;
        int playerState;
};

#endif // MYPLAYER_H
