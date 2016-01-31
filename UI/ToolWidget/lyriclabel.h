#ifndef LYRICLABEL_H
#define LYRICLABEL_H

#include <QWidget>

class AbstractWheelWidget : public QWidget
{
    Q_OBJECT

    public:
        AbstractWheelWidget(bool touch, QWidget *parent = 0);
        virtual ~AbstractWheelWidget();

        int currentIndex() const;
        void setCurrentIndex(int index);

        bool event(QEvent*);
        void paintEvent(QPaintEvent *e);
        virtual void paintItem(QPainter* painter, int index, const QRect &rect) = 0;

        virtual int itemHeight() const = 0;
        virtual int itemCount() const = 0;

    public slots:
        void scrollTo(int index);

    signals:
        void stopped(int index);
        void changeTo(int index);

    protected:
        int m_currentItem;
        int m_itemOffset; // 0-itemHeight()
        qreal m_lastY;
        bool isScrolled;
        bool dosignal;//滚动是否触发信号,用于区别是否是人为滚动
};

class Lyric;
class QFont;

class LyricLabel : public AbstractWheelWidget
{
        Q_OBJECT
    public:
        LyricLabel(bool touch, QWidget *parent = 0);

        //QStringList items() const;
        //void setItems( const QStringList &items );

        //QSize sizeHint() const;
        //QSize minimumSizeHint() const;
        void getFromFile(QString dir);
        void paintItem(QPainter* painter, int index, const QRect &rect);

        int itemHeight() const;
        int itemCount() const;

    signals:
        void changeTo(qint64 pos);
        void rightClicked();

    public slots:
        void postionChanged(qint64 pos);
        void setPostion(qint64 pos);

    protected slots:
        void changeToEvent(int index);
        void changeFont();
        void changeNormalColor();
        void changeHightLightColor();

    protected:
        void contextMenuEvent(QContextMenuEvent *event);//右击事件

    protected:
        Lyric *lyric;
        QFont *lyricFont;
        QColor *lyricNormal;
        QColor *lyricHighlight;
};

#endif // LYRICLABEL_H
