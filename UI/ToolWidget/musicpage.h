#ifndef MUSICPAGE_H
#define MUSICPAGE_H

#include <QWidget>
#include <QQueue>
#include "Core/musicinfo.h"

class ListList;
class QStackedWidget;
class QPushButton;

class MusicPage : public QWidget
{
        Q_OBJECT
    public:
        explicit MusicPage(QWidget *parent = 0);
        void setCurrentList(int index);
        QString getCurrentList();
        void addMusics(QStringList fileDir);
        void addMusics(int listId, QQueue<MusicInfo> musics);
        void clearMusic();//
        void removeHighLight();
        void setHighLight(int list, int index);

    protected:
        void enterEvent(QEvent *e);//鼠标进入事件
        void addMusic(QString name, QString artist);

    signals:
        void mouseEnter();
        void addMusics();
        void tryToCreateList(QString name);
        void createList(QString name);
        void deleteList(QString name);
        void changeMusicList(QString listName);
        void removeTheMusic(QString listName, int index);
        void removeAllMusics(QString listName);
        void play(QString name, int index);
        void hightLightChanged();

    private slots:
        void changeMusicList(int index);
        void createList();
        void deleteList(int index);
        void removeTheMusic(int index);
        void removeAllMusics();
        void play(int index);

    private:
        QPushButton *addButton;
        ListList *listList;
        QStackedWidget *musicLists;
};

#endif // MUSICPAGE_H
