#include "data.h"
#include "Core/playmode.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QQueue>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QtDebug>

Data::Data()
{

}

bool Data::connectData()
{
    QDir dir("./data");
    if (!dir.exists())
    {
        if (dir.mkpath("./data")) {}
        else return false;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./data/musicData.db");
    if (!db.open())
        return false;
    return true;
}

void Data::tryConnectListList()
{
    QSqlQuery query;
    if (!query.exec("select * from listlist;"))
    {
        if (!query.exec("CREATE TABLE listlist("
                        "name VARCHAR,"
                        "id int,"
                        "count int)")) {}
    }
}

QQueue<QString> Data::getListList()
{
    tryConnectListList();
    QQueue <QString> q;
    QSqlQuery query;
    query.exec("select * from listlist order by id;");
    while (query.next())
    {
        q.push_back(query.value(0).toString());
        qDebug() << query.value(1).toInt() << endl;
    }
    if (q.size() == 0)
    {
        query.exec("insert into listlist"
                   "(name, id, count) values('默认列表', 0, 0);");
        Data::changeListCount(1);
        q.push_back(QString("默认列表"));
    }
    return q;
}

bool Data::addList(QString name)
{
    tryConnectListList();
    QSqlQuery query;
    QString str =
            QString("select * from listlist where name = '%1'").arg(name);
    query.exec(str);
    if (query.next()) return false;
    int cnt = Data::getListCount();
    str = QString("insert into listlist(name, id, count) values('%1', %2, 0);").arg(name).arg(cnt);
    query.exec(str);
    Data::changeListCount(cnt+1);
    return true;
}

void Data::deleteList(QString name)
{
    tryConnectListList();
    QSqlQuery query;
    QString str =
            QString("select * from listlist where name = '%1'").arg(name);
    query.exec(str);
    int id;
    query.next();
    id = query.value(1).toInt();

    str = QString("delete from listlist where name = '%1'").arg(name);
    query.exec(str);

    str = QString("update listlist set id = id-1 "
                    "where id > '%1';").arg(id);
    query.exec(str);

    str = QString("drop table %1").arg(name);
    query.exec(str);

    int cnt = Data::getListCount();
    Data::changeListCount(cnt-1);
}

int Data::getListId(QString name)
{
    tryConnectListList();
    QSqlQuery query;
    QString str =
            QString("select * from listlist where name = '%1'").arg(name);
    query.exec(str);
    query.next();
    return query.value(1).toInt();
}

void Data::tryConnectList(QString listName)
{
    QSqlQuery query;
    if (!query.exec("select * from "+listName+";"))
    {
        if (!query.exec("CREATE TABLE "+listName+"("
                        "dir VARCHAR,"
                        "name VARCHAR,"
                        "artist VARCHAR,"
                        "id int)")) {}
    }
}

QString Data::getMusicName(QString listName, int id)
{
    tryConnectList(listName);
    QSqlQuery query;
    QString str =
            QString("select * from %1 where id = %2").arg(listName).arg(id);
    qDebug() << str << endl;
    query.exec(str);
    query.next();
    return query.value(1).toString();
}

QQueue<MusicInfo> Data::getMusicList(QString listName)
{
    tryConnectList(listName);
    QQueue <MusicInfo> q;
    MusicInfo musicInfo;
    QSqlQuery query;
    query.exec("select * from "+listName+" order by id;");
    while (query.next())
    {
        musicInfo.setDir(query.value(0).toString());
        musicInfo.setName(query.value(1).toString());
        musicInfo.setArtist(query.value(2).toString());
        q.push_back(musicInfo);
        //qDebug() << query.value(3).toInt() << " ";
    }
    int all = q.size();
    query.exec("select * from listlist;");
    QString str =
            QString("update listlist set count = %1 "
                    "where name = '%2';").arg(all).arg(listName);
    query.exec(str);
    return q;
}

void Data::addMusicsToEnd(QString listName, QQueue<MusicInfo> musics)
{
    QSqlDatabase::database().transaction();
    tryConnectList(listName);
    int cnt;
    QSqlQuery query;
    QString str =
            QString("select * from listlist where name='%1';").arg(listName);
    query.exec(str);
    if (query.next())
        cnt = query.value(2).toInt();
    else
        cnt = 0;
    qDebug() << cnt << endl;
    MusicInfo musicInfo;
    while (!musics.empty())
    {
        musicInfo = musics.front();
        QString str
                = QString("insert into "
                          "%1(dir, name, artist, id) "
                          "values('%2', '%3', '%4', %5);").arg(listName).arg(musicInfo.getDir()).arg(musicInfo.getName()).arg(musicInfo.getArtist()).arg(cnt);
        query.exec(str);
        //qDebug() << str << endl;
        cnt++;
        musics.pop_front();
    }
    query.exec("select * from listlist;");
    str = QString("update listlist set count = %1 "
                    "where name = '%2';").arg(cnt).arg(listName);
    qDebug() << str << endl;
    query.exec(str);
    QSqlDatabase::database().commit();
}

void Data::deleteMusic(QString listName, int row)
{
    tryConnectList(listName);
    QSqlQuery query;
    QString str =
            QString("delete from %1 where id = %2").arg(listName).arg(row);
    qDebug() << str << endl;
    query.exec(str);
    str = QString("update %1 set id = id-1 "
                    "where id > '%2';").arg(listName).arg(row);
    qDebug() << str << endl;
    query.exec(str);
}

void Data::deleteAllMusic(QString listName)
{
    tryConnectList(listName);
    QSqlQuery query;
    QString str =
            QString("delete from %1 where id >= 0").arg(listName);
    query.exec(str);
}

void Data::tryConnectBaseInfo()
{
    QSqlQuery query;
    QString cmd = QString("select * from baseinfo;");
    if (!query.exec(cmd))
    {
        cmd = QString("create table baseinfo("
                      "name VARCHAR,"
                      "val VARCHAR)");
        if (!query.exec(cmd)) {}
        qDebug() << cmd << endl;
    }
}

int Data::getListCount()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("select * from baseinfo where name='listcount';");
    if (query.next())
    {
        qDebug() << "read listcount successful:" << query.value(1).toInt() << endl;
        return query.value(1).toInt();
    }
    else
        return 0;
}

void Data::changeListCount(int cnt)
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("delete from baseinfo where name = 'listcount';");
    QString cmd = QString("insert into baseinfo(name, val) values('listcount', '%1');").arg(cnt);
    query.exec(cmd);
    qDebug() << cmd << endl;
}

QString Data::getCurrentBackground()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    QString dir = QString(":/skins/res/skins/skin00");
    query.exec("select * from baseinfo where name='background';");
    if (query.next())
        dir = query.value(1).toString();
    qDebug() << "Background: " << dir << endl;
    return dir;
}

void Data::changeCurrentBackground(QString dir)
{
    tryConnectBaseInfo();
    QSqlQuery query;
    QString cmd = QString("delete from baseinfo where name = 'background';");
    query.exec(cmd);
    cmd = QString("insert into baseinfo(name, val) values('background', '%1');").arg(dir);
    query.exec(cmd);
    qDebug() << cmd << endl;
}

int Data::getSoundLevel()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("select * from baseinfo where name='soundlevel';");
    if (query.next())
    {
        qDebug() << "read soundlevel successful:" << query.value(1).toInt() << endl;
        return query.value(1).toInt();
    }
    else
        return 25;
}

void Data::changeSoundLevel(int voice)
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("delete from baseinfo where name = 'soundlevel';");
    QString cmd = QString("insert into baseinfo(name, val) values('soundlevel', '%1');").arg(voice);
    query.exec(cmd);
    qDebug() << cmd << endl;
}

int Data::getPlayMode()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("select * from baseinfo where name='playmode';");
    if (query.next())
    {
        qDebug() << "read playmode successful:" << query.value(1).toInt() << endl;
        return query.value(1).toInt();
    }
    else
    {
        return Sequential;
    }
}

void Data::changePlayMode(int mode)
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("delete from baseinfo where name = 'playmode';");
    QString cmd = QString("insert into baseinfo(name, val) values('playmode', '%1');").arg(mode);
    if (query.exec(cmd))
        qDebug() << cmd << endl;
    else
        qDebug() << "fail " << cmd << endl;
}
