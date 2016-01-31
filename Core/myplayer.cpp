#include "myplayer.h"
#include "Core/playmode.h"
#include <QMediaContent>
#include <QTime>

MyPlayer::MyPlayer()
{
    playerState = NOMUSIC;
    connect(&player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(playNext(QMediaPlayer::MediaStatus)));
    connect(&player, SIGNAL(durationChanged(qint64)), this, SIGNAL(durationChanged(qint64)));
    connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(musicChanged()));
    connect(&player, SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChanged(qint64)));
    connect(this, SIGNAL(playerStateChanged(int)), this, SLOT(stateChanged(int)));
}

void MyPlayer::setPlay()
{
    player.play();
    emit playerStateChanged(PLAYING);
}

void MyPlayer::setPause()
{
    player.pause();
    emit playerStateChanged(PAUSE);
}

void MyPlayer::musicChanged()
{
    emit musicChanged(currentList, currentindex);
}

int MyPlayer::getPlayerState()
{
    return this->playerState;
}

void MyPlayer::stateChanged(int state)
{
    this->playerState = state;
}

void MyPlayer::setVoice(int voice)
{
    this->player.setVolume(voice);
}

void MyPlayer::setPlayMode(int mode)
{
    this->playMode = mode;
}

void MyPlayer::setPosition(qint64 pos)
{
    player.setPosition(pos);
}

void MyPlayer::addMusics(QString name, QQueue<MusicInfo> q)
{
    MusicInfo musicInfo;
    while (!q.empty())
    {
        musicInfo = q.front();
        q.pop_front();
        list[name].push_back(QUrl::fromLocalFile(musicInfo.getDir()));
    }
}

void MyPlayer::removeTheMusic(QString listName, int index)
{
    list[listName].removeAt(index);
    if (listName == currentList && index == currentindex)
    {
        player.stop();
        emit musicNotAvailable();
        emit playerStateChanged(NOMUSIC);
    }
}

void MyPlayer::removeAllMusics(QString listName)
{
    list[listName].clear();
    if (list[currentList].size() == 0)
    {
        player.stop();
        emit musicNotAvailable();
        emit playerStateChanged(NOMUSIC);
    }
}

void MyPlayer::setCurrentIndex(int index)
{
    this->currentindex = index;
    this->player.setMedia(QMediaContent(list[currentList][index]));
}

int MyPlayer::getCurrentIndex()
{
    return this->currentindex;
}

int MyPlayer::mediaCount()
{
    return this->list[currentList].size();
}

void MyPlayer::addMedia(QString listName, QUrl url)
{
    this->list[listName].push_back(url);
}

void MyPlayer::clear(QString listName)
{
    this->list[listName].clear();
    if (list[currentList].size() == 0)
        player.stop();
}

void MyPlayer::playNext(QMediaPlayer::MediaStatus state)
{
    if (!(state == QMediaPlayer::EndOfMedia))
        return;
    switch (playMode)
    {
        case CurrentItemOnce:
            player.stop();
            emit playerStateChanged(PAUSE);
            break;
        case CurrentItemInLoop:
            player.play();
            emit playerStateChanged(PLAYING);
            break;
        case Sequential:
            currentindex++;
            if (currentindex < list[currentList].size())
            {
                setCurrentIndex(currentindex);
                player.play();
                emit playerStateChanged(PLAYING);
            }
            else
            {
                currentindex--;
                player.stop();
                emit playerStateChanged(PAUSE);
            }
            break;
        case Loop:
            currentindex++;
            if (currentindex < list[currentList].size())
            {
                setCurrentIndex(currentindex);
                player.play();
                emit playerStateChanged(PLAYING);
            }
            else
            {
                currentindex = 0;
                setCurrentIndex(currentindex);
                player.play();
                emit playerStateChanged(PLAYING);
            }
            break;
        case Random:
            QTime time;
            time = QTime::currentTime();
            qsrand(time.msec()+time.second()*1000);
            currentindex = qrand()%list[currentList].size();
            setCurrentIndex(currentindex);
            player.play();
            emit playerStateChanged(PLAYING);
            break;
    }
}

void MyPlayer::playTheMusic(QString listName, int index)
{
    currentList = listName;
    this->setCurrentIndex(index);
    player.play();
    emit playerStateChanged(PLAYING);
}

bool MyPlayer::playPre()
{
    if (this->mediaCount() == 0)
        return false;
    int index = this->getCurrentIndex();
    if(--index <= 0)
        index=0;
    this->setCurrentIndex(index);
    player.play();
    emit playerStateChanged(PLAYING);
    return true;
}

bool MyPlayer::playNext()
{
    if (this->mediaCount() == 0)
        return false;
    int index = this->getCurrentIndex();
    if(++index == this->mediaCount())
        index=0;
    this->setCurrentIndex(index);
    player.play();
    emit playerStateChanged(PLAYING);
    return true;
}

void MyPlayer::play()
{
    player.play();
    emit playerStateChanged(PLAYING);
}

void MyPlayer::pause()
{
    player.pause();
    emit playerStateChanged(PAUSE);
}
