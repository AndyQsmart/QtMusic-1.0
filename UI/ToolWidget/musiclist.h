#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QListWidget>
#include <QBrush>

class MusicList : public QListWidget
{
        Q_OBJECT

    public:
        MusicList(QWidget *parent = 0);
        ~MusicList();
        void setHighLight(int row);
        void setArtist(int index, QString artist);

    public slots:
        void removeHighLight();

    private slots:
        void playTheMusic();
        void removeTheMusic();
        void removeAllMusic();
        void doubleClickedEvent(QModelIndex index);

    signals:
        void mouseEnter();
        void rightClicked();
        void playTheMusic(int index);
        void removeTheMusic(int index);
        void removeAllMusics();

    private:
        void enterEvent(QEvent *e);//鼠标进入事件
        void contextMenuEvent(QContextMenuEvent *event);//右击事件
        int highLightRow = -1;
};

class MusicListItem : public QWidget
{
        Q_OBJECT

    public:
        MusicListItem(QWidget *parent = 0);
        void setName(QString name);
        void setArtist(QString artist);
        void setHighLight();
        void removeHighLight();

    private:
        void paintEvent(QPaintEvent *e);

    private:
        bool isHighLight;
        QString name;
        QString artist;
};

#endif // MUSICLIST_H
