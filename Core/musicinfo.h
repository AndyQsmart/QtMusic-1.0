#ifndef MUSICINFO_H
#define MUSICINFO_H

#include <QString>

class MusicInfo
{
    public:
        MusicInfo();
        void setDir(QString dirStr) {dir = dirStr;}
        void setName(QString nameStr) {name = nameStr;}
        void setArtist(QString artistStr) {artist = artistStr;}
        QString getDir() {return dir;}
        QString getName() {return name;}
        QString getArtist() {return artist;}

    private:
        QString dir;
        QString name;
        QString artist;
};

#endif // MUSICINFO_H
