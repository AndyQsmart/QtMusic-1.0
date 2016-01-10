#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QTableWidget>
#include <QBrush>

class MusicList : public QTableWidget
{
        Q_OBJECT

    public:
        MusicList(QWidget *parent = 0);
        ~MusicList();
        void setHighLight(int row);

    public slots:
        void removeHighLight();

    private slots:
        void playTheMusic();
        void removeTheMusic();
        void removeAllMusic();

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

#endif // MUSICLIST_H
