#include "musicinfo.h"
#include <QMediaPlayer>
#include <QMediaContent>
#include <QMediaMetaData>

MusicInfo::MusicInfo()
{

}

void MusicInfo::setDir(QString dirStr)
{
    this->dir = dirStr;
    QMediaPlayer *player = new QMediaPlayer();
    //connect(player, SIGNAL(metaDataChanged())
    //player->setMedia(QMediaContent(QUrl(dirStr)));
    //this->artist = player->metaData(QMediaMetaData::Title).toString();
    //this->artist = player->metaData(QMediaMetaData::ContributingArtist).toString();
}
