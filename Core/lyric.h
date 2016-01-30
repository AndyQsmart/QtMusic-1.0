#ifndef LYRIC_H
#define LYRIC_H

#include <QVector>

class Lyric
{
    public:
        Lyric();
        void getFromFile(QString dir);
        QString getLineAt(int index);
        int getCount();
        int getIndex(qint64 pos);
        qint64 getPostion(int index);

    private:
        //QString singer;艺人名
        //QString title;曲名
        //QString album;专辑名
        //QString writer;编者
        QString filedir;
        double offset;
        QVector<qint64> postion;
        QVector<QString> line;
};

#endif // LYRIC_H
