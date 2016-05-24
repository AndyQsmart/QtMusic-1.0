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

/*
 listlist(name, id, count);
 musiclist(listname, id, dir, name, artist);
*/

Data::Data()
{

}

bool Data::connectData()
{
    QDir dir("./data");
    if (!dir.exists())
    {
        if (dir.mkpath("./"))
        {
            qDebug() << "Make the data path sucessfully!" << endl;
        }
        else
        {
            qDebug() << "Fail to make the data path!" << endl;
            return false;
        }
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./data/musicData.db");
    if (!db.open())
    {
        qDebug() << "Open \"./data/musicData.db\" sucessfully" << endl;
        return false;
    }
    else
    {
        qDebug() << "Fail to open \"./data/musicData.db\"" << endl;
        return true;
    }
}

void Data::tryConnectListList()
{
    QSqlQuery query;
    if (!query.exec("select * from listlist;"))
    {
        qDebug() << "Table listlist no exist!" << endl;
        if (!query.exec("CREATE TABLE listlist("
                        "name VARCHAR,"
                        "id int,"
                        "count int)"))
        {
            qDebug() << "Fail to create table listlist!" << endl;
        }
        else
        {
            qDebug() << "Create table listlist sucessfully!" << endl;
        }
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
        qDebug() << "Get list" << query.value(1).toInt() << ":" << query.value(0).toString()<< endl;
    }
    if (q.size() == 0)
    {
        query.exec("insert into listlist"
                   "(name, id, count) values('默认列表', 0, 0);");
        Data::changeListCount(1);
        q.push_back(QString("默认列表"));
        qDebug() << "Create list:默认列表" << endl;
    }
    return q;
}

bool Data::addList(QString name)
{
    tryConnectListList();
    name = name.replace("'", "''");
    QSqlQuery query;
    QString str =
            QString("select * from listlist where name = '%1'").arg(name);
    query.exec(str);
    if (query.next())
    {
        qDebug() << "The list is existed, fail to create" << endl;
        return false;
    }
    int cnt = Data::getListCount();
    str = QString("insert into listlist(name, id, count) values('%1', %2, 0);").arg(name).arg(cnt);
    query.exec(str);
    Data::changeListCount(cnt+1);
    qDebug() << "Create list" << cnt << ":" << name << endl;
    return true;
}

void Data::deleteList(QString name)
{
    tryConnectListList();
    name = name.replace("'", "''");
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

    str = QString("delete from musiclist where listname = '%1'").arg(name);
    query.exec(str);

    int cnt = Data::getListCount();
    Data::changeListCount(cnt-1);
    qDebug() << "Delete list"+name << endl;
}

int Data::getListId(QString name)
{
    tryConnectListList();
    name = name.replace("'", "''");
    QSqlQuery query;
    QString str =
            QString("select * from listlist where name = '%1'").arg(name);
    query.exec(str);
    query.next();
    return query.value(1).toInt();
}

void Data::tryConnectMusicList()
{
    QSqlQuery query;
    if (!query.exec("select * from musiclist;"))
    {
        qDebug() << "Table musiclist no exist!" << endl;
        if (!query.exec("CREATE TABLE musiclist("
                        "listname VARCHAR,"
                        "id int,"
                        "dir VARCHAR,"
                        "name VARCHAR,"
                        "artist VARCHAR)"))
        {
            qDebug() << "Fail to create table musiclist!" << endl;
        }
        else
        {
            qDebug() << "Create table musiclist sucessfully!" << endl;
        }
    }
}

QString Data::getMusicName(QString listName, int id)
{
    tryConnectMusicList();
    listName = listName.replace("'", "''");
    QSqlQuery query;
    QString str =
            QString("select * from musiclist where listname = '%1' and id = %2").arg(listName).arg(id);
    query.exec(str);
    query.next();
    return query.value(3).toString();
}

QString Data::getMusicDir(QString listName, int id)
{
    tryConnectMusicList();
    listName = listName.replace("'", "''");
    QSqlQuery query;
    QString str =
            QString("select * from musiclist where listname = '%1' and id = %2").arg(listName).arg(id);
    query.exec(str);
    query.next();
    return query.value(2).toString();
}

QQueue<MusicInfo> Data::getMusicList(QString listName)
{
    tryConnectMusicList();
    listName = listName.replace("'", "''");
    QQueue <MusicInfo> q;
    MusicInfo musicInfo;
    QSqlQuery query;
    query.exec("select * from musiclist where listname = '"+listName+"' order by id;");
    while (query.next())
    {
        musicInfo.setDir(query.value(2).toString());
        musicInfo.setName(query.value(3).toString());
        musicInfo.setArtist(query.value(4).toString());
        q.push_back(musicInfo);
        qDebug() << "Get music:" << query.value(3).toString() << endl;
    }
    int all = q.size();
    query.exec("select * from listlist;");
    QString str =
            QString("update listlist set count = %1 "
                    "where name = '%2';").arg(all).arg(listName);
    query.exec(str);
    return q;
}

void Data::setArtist(QString listName, int id, QString artist)
{
    tryConnectMusicList();
    listName = listName.replace("'", "''");
    QSqlQuery query;
    QString str =
            QString("update musiclist set artist = '%1' where listname = '%2' and id = %3;").arg(artist).arg(listName).arg(id);
    query.exec(str);
}

void Data::addMusicsToEnd(QString listName, QQueue<MusicInfo> musics)
{
    QSqlDatabase::database().transaction();
    listName = listName.replace("'", "''");
    tryConnectMusicList();
    int cnt;
    QSqlQuery query;
    QString str =
            QString("select * from listlist where name='%1';").arg(listName);
    query.exec(str);
    if (query.next())
        cnt = query.value(2).toInt();
    else
        cnt = 0;
    MusicInfo musicInfo;
    while (!musics.empty())
    {
        musicInfo = musics.front();
        QString str
                = QString("insert into "
                          "musiclist(listname, id, dir, name, artist) "
                          "values('%1', %2, '%3', '%4', '%5');").arg(
                      listName).arg(
                      cnt).arg(
                      musicInfo.getDir().replace("'", "''")).arg(
                      musicInfo.getName().replace("'", "''")).arg(
                      musicInfo.getArtist());
        qDebug() << "Add music:" << musicInfo.getName() << endl;
        query.exec(str);
        cnt++;
        musics.pop_front();
    }
    query.exec("select * from listlist;");
    str = QString("update listlist set count = %1 "
                    "where name = '%2';").arg(cnt).arg(listName);
    query.exec(str);
    QSqlDatabase::database().commit();
}

void Data::deleteMusic(QString listName, int row)
{
    tryConnectMusicList();
    listName = listName.replace("'", "''");
    QSqlQuery query;
    QString str =
            QString("delete from musiclist where listname = '%1' and id = %2").arg(listName).arg(row);
    query.exec(str);
    str = QString("update musiclist set id = id-1 "
                    "where listname = '%1' and id > '%2';").arg(listName).arg(row);
    query.exec(str);
}

void Data::deleteAllMusic(QString listName)
{
    tryConnectMusicList();
    listName = listName.replace("'", "''");
    QSqlQuery query;
    QString str =
            QString("delete from musiclist where listname = '%1'").arg(listName);
    query.exec(str);
}

void Data::moveMusic(QString listName, int from, int to)
{
    QSqlDatabase::database().transaction();
    listName = listName.replace("'", "''");
    tryConnectMusicList();
    QSqlQuery query;
    QString str =
            QString("update musiclist set id = -1 where listname = '%1' and id = %2").arg(listName).arg(from);
    query.exec(str);
    str = QString("update musiclist set id = id-1 where listname = '%1' and id > %2").arg(listName).arg(from);
    query.exec(str);
    str = QString("update musiclist set id = id+1 where listname = '%1' and id >= %2").arg(listName).arg(to);
    query.exec(str);
    str = QString("update musiclist set id = %1 where listname = '%2' and id = -1").arg(to).arg(listName);
    query.exec(str);
    QSqlDatabase::database().commit();
    qDebug() << "Move music from" << from << "to" << to << endl;
}

void Data::tryConnectBaseInfo()
{
    QSqlQuery query;
    QString cmd = QString("select * from baseinfo;");
    if (!query.exec(cmd))
    {
        qDebug() << "Table baseinfo no exist!" << endl;
        cmd = QString("create table baseinfo("
                      "name VARCHAR,"
                      "val VARCHAR)");
        if (!query.exec(cmd))
        {
            qDebug() << "Fail to create table baseinfo!" << endl;
        }
        else
        {
            qDebug() << "Create table baseinfo sucessfully!" << endl;
        }
    }
}

int Data::getListCount()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("select * from baseinfo where name='listcount';");
    if (query.next()) return query.value(1).toInt();
    else return 0;
}

void Data::changeListCount(int cnt)
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("delete from baseinfo where name = 'listcount';");
    QString cmd = QString("insert into baseinfo(name, val) values('listcount', '%1');").arg(cnt);
    query.exec(cmd);
}

QString Data::getCurrentBackground()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    QString dir = QString(":/images/skins/skin00");
    query.exec("select * from baseinfo where name='background';");
    if (query.next())
        dir = query.value(1).toString();
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
}

int Data::getSoundLevel()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("select * from baseinfo where name='soundlevel';");
    if (query.next()) return query.value(1).toInt();
    else return 25;
}

void Data::changeSoundLevel(int voice)
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("delete from baseinfo where name = 'soundlevel';");
    QString cmd = QString("insert into baseinfo(name, val) values('soundlevel', '%1');").arg(voice);
    query.exec(cmd);
}

int Data::getPlayMode()
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("select * from baseinfo where name='playmode';");
    if (query.next()) return query.value(1).toInt();
    else return Sequential;
}

void Data::changePlayMode(int mode)
{
    tryConnectBaseInfo();
    QSqlQuery query;
    query.exec("delete from baseinfo where name = 'playmode';");
    QString cmd = QString("insert into baseinfo(name, val) values('playmode', '%1');").arg(mode);
    query.exec(cmd);
}
