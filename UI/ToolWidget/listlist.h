#ifndef LISTLIST_H
#define LISTLIST_H

#include <QTableWidget>
#include <QBrush>

class ListList : public QTableWidget
{
        Q_OBJECT

    public:
        ListList(QWidget *parent = 0);
        void removeHighLight(int row);
        int getHighLightRow();
        QString getHighLightRowName();

    public slots:
        void setHighLight(int row);
        void wantToCreateList();
        void createList(QString name);

    signals:
        void mouseEnter();
        void rightClicked();
        void changeList(int index);
        void tryToCreateList(QString name);
        void deleteList(QString name);
        void deleteList(int index);

    protected slots:
        void clickEvent();
        void tryToCreateList();
        void deleteList();

    private:
        void enterEvent(QEvent *e);//鼠标进入事件
        void contextMenuEvent(QContextMenuEvent *event);//右击事件
        int highLightRow = -1;
};

#endif // LISTLIST_H
