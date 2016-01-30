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

    protected:
        int m_currentItem;
        int m_itemOffset; // 0-itemHeight()
        qreal m_lastY;
};

class Lyric;

class LyricLabel : public AbstractWheelWidget
{
        Q_OBJECT
    public:
        LyricLabel(bool touch, QWidget *parent = 0);

        //QStringList items() const;
        //void setItems( const QStringList &items );

        //QSize sizeHint() const;
        //QSize minimumSizeHint() const;

        void paintItem(QPainter* painter, int index, const QRect &rect);

        int itemHeight() const;
        int itemCount() const;

    signals:
        void changeTo(qint64 pos);

    public slots:
        void postionChanged(qint64 pos);
        void setPostion(qint64 pos);

    protected:
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);//鼠标释放

    protected:
        Lyric *lyric;
        bool isPressed;
};

#endif // LYRICLABEL_H
