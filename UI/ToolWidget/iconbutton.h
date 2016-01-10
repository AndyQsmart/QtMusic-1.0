#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QWidget>

class IconButton : public QWidget
{
        Q_OBJECT
    public:
        IconButton(QWidget *parent = 0);
        explicit IconButton(const QString &iconDir, QWidget *parent = 0);
        void setTheIcon(const QString &iconDir);

        void setIJ(int i, int j);

    signals:
        void clicked();
        void clicked(int i, int j);

    protected:
        void paintEvent(QPaintEvent *e);
        void enterEvent(QEvent *e);
        void leaveEvent(QEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);

    private:
        bool isPressed;
        bool isEnter;
        QCursor preCursor;
        QPixmap pixmap;

        int whichI;
        int whichJ;
};

#endif // ICONBUTTON_H
