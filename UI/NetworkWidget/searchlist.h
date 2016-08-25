#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QTableWidget>
#include <QVector>

class SearchList : public QTableWidget
{
    Q_OBJECT
public:
    SearchList(QWidget *parent = 0);
    void addSong(const QString &name, const QString &artist, const QString &length);
    void clearSongs();
    void setSongs(QVector<QString> &name, QVector<QString> &artist, QVector<int> &length);
};

#endif // SEARCHLIST_H
