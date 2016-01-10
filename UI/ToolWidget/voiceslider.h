#ifndef VOICESLIDER_H
#define VOICESLIDER_H

#include <QWidget>

class VoiceSlider : public QWidget
{
        Q_OBJECT

    public:
        explicit VoiceSlider(QWidget *parent = 0);
        qreal getCurrentPostion();

    public slots:
        void moveTo(qreal pos);

    signals:
        void changeTo(qreal pos);

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void paintEvent(QPaintEvent *e);
        void enterEvent(QEvent *e);
        void leaveEvent(QEvent *e);

    private:
        bool isPressed;
        qreal currentPos;
        QCursor preCursor;
};

#endif // VOICESLIDER_H
