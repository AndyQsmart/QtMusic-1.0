#include "lyriclabel.h"
#include "Core/lyric.h"
#include <QScroller>
#include <QEvent>
#include <QScrollPrepareEvent>
#include <QPainter>
#include <QApplication>
#define WHEEL_SCROLL_OFFSET 50000.0

#include <QDebug>

LyricLabel::LyricLabel(bool touch, QWidget *parent)
    :AbstractWheelWidget(touch, parent)
{
    lyric = new Lyric();
    isPressed = false;
}

void LyricLabel::paintItem(QPainter* painter, int index, const QRect &rect)
{
    painter->drawText(rect, Qt::AlignCenter, lyric->getLineAt(index));
}

int LyricLabel::itemHeight() const
{
    //QFontMetrics fm(QFont("LyricBig", 30, QFont::Bold));
    //return fm.height();
    return 45;
}

int LyricLabel::itemCount() const
{
    return lyric->getCount();
}

void LyricLabel::postionChanged(qint64 pos)
{
    if (this->isPressed) return;
    int index = lyric->getIndex(pos);
    if (index != m_currentItem)
        this->scrollTo(index);
}

void LyricLabel::setPostion(qint64 pos)
{
    int index = lyric->getIndex(pos);
    this->setCurrentIndex(index);
}

void LyricLabel::mousePressEvent(QMouseEvent *e)
{
    this->isPressed = true;
}

void LyricLabel::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << m_currentItem << endl;
    emit changeTo(this->m_currentItem);
    this->isPressed = false;
}

AbstractWheelWidget::AbstractWheelWidget(bool touch, QWidget *parent)
    : QWidget(parent), m_currentItem(0), m_itemOffset(0)
{
// ![0]
    QScroller::grabGesture(this, touch ? QScroller::TouchGesture : QScroller::LeftMouseButtonGesture);
// ![0]
}

AbstractWheelWidget::~AbstractWheelWidget()
{ }

int AbstractWheelWidget::currentIndex() const
{
    return m_currentItem;
}

void AbstractWheelWidget::setCurrentIndex(int index)
{
    if (index >= 0 && index < itemCount()) {
        m_currentItem = index;
        m_itemOffset = 0;
        update();
    }
}

bool AbstractWheelWidget::event(QEvent *e)
{
    switch (e->type())
    {
// ![1]
        case QEvent::ScrollPrepare:
        {
            // We set the snap positions as late as possible so that we are sure
            // we get the correct itemHeight
            QScroller *scroller = QScroller::scroller(this);
            scroller->setSnapPositionsY( WHEEL_SCROLL_OFFSET, itemHeight() );

            QScrollPrepareEvent *se = static_cast<QScrollPrepareEvent *>(e);
            se->setViewportSize(QSizeF(size()));
            // we claim a huge scrolling area and a huge content position and
            // hope that the user doesn't notice that the scroll area is restricted
            se->setContentPosRange(QRectF(0.0, 0.0, 0.0, WHEEL_SCROLL_OFFSET * 2));
            se->setContentPos(QPointF(0.0, WHEEL_SCROLL_OFFSET + m_currentItem * itemHeight() + m_itemOffset));
            se->accept();
            return true;
        }
// ![1]
// ![2]
        case QEvent::Scroll:
        {
            QScrollEvent *se = static_cast<QScrollEvent *>(e);

            qreal y = se->contentPos().y();
            int iy = y - WHEEL_SCROLL_OFFSET;
            int ih = itemHeight();

// ![2]
// ![3]
            // -- calculate the current item position and offset and redraw the widget
            int ic = itemCount();
            if (ic>0)
            {
                m_currentItem = iy / ih;
                //m_currentItem = iy / ih % ic;
                m_itemOffset = iy % ih;

                // take care when scrolling backwards. Modulo returns negative numbers
                /*
                if (m_itemOffset < 0) {
                    m_itemOffset += ih;
                    m_currentItem--;
                }
                */
                //if (m_currentItem < 0)
                    //m_currentItem += ic;
                    //m_currentItem = 0;
                if (m_currentItem >= ic) m_currentItem = ic-1;
            }
            // -- repaint
            update();

            se->accept();
            return true;
// ![3]
        }
        case QEvent::MouseButtonPress:
            return true;
        default:
            return QWidget::event(e);
    }
    return true;
}

void AbstractWheelWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED( event );

    // -- first calculate size and position.
    int w = width();
    int h = height();

    QPainter painter(this);
    QPalette palette = QApplication::palette();
    QPalette::ColorGroup colorGroup = isEnabled() ? QPalette::Active : QPalette::Disabled;

    // linear gradient brush
    QLinearGradient grad(0.5, 0, 0.5, 1.0);
    grad.setColorAt(0, palette.color(colorGroup, QPalette::ButtonText));
    grad.setColorAt(0.2, palette.color(colorGroup, QPalette::Button));
    grad.setColorAt(0.8, palette.color(colorGroup, QPalette::Button));
    grad.setColorAt(1.0, palette.color(colorGroup, QPalette::ButtonText));
    grad.setCoordinateMode( QGradient::ObjectBoundingMode );
    QBrush gBrush( grad );
/*
    // paint a border and background
    painter.setPen(palette.color(colorGroup, QPalette::ButtonText));
    painter.setBrush(gBrush);
    // painter.setBrushOrigin( QPointF( 0.0, 0.0 ) );
    //painter.drawRect( 0, 0, w-1, h-1 );

    // paint inner border
    painter.setPen(palette.color(colorGroup, QPalette::Button));
    painter.setBrush(Qt::NoBrush);
    //painter.drawRect( 1, 1, w-3, h-3 );
*/
    // paint the items
    painter.setClipRect( QRect( 3, 3, w-6, h-6 ) );
    painter.setPen(palette.color(colorGroup, QPalette::ButtonText));

    int iH = itemHeight();
    int iC = itemCount();
    if (iC > 0)
    {

        m_itemOffset = m_itemOffset % iH;

        for (int i = -h/2/iH; i <= h/2/iH+1; i++)
        {

            int itemNum = m_currentItem + i;
            /*
            while (itemNum < 0)
                itemNum += iC;
            while (itemNum >= iC)
                itemNum -= iC;
            */
            if (itemNum >= 0 && itemNum < iC)
            {
                if (itemNum == this->m_currentItem)
                {
                    painter.setPen(QColor(255, 220, 26));
                    painter.setFont(QFont("宋体", 17, QFont::Bold));
                    paintItem(&painter, itemNum, QRect(6, h/2 +i*iH - m_itemOffset - iH/2, w-6, iH ));
                }
                else
                {
                    int len = h/2/iH;
                    int t = len-abs(i);
                    t = (t+8)*255/(len+8);
                    //qDebug() << "a值:" << t << endl;
                    painter.setPen(QColor(255, 255, 255, t));
                    painter.setFont(QFont("宋体", 12, QFont::Bold));
                    paintItem(&painter, itemNum, QRect(6, h/2 +i*iH - m_itemOffset - iH/2, w-6, iH ));
                }
            }
        }
    }
/*
    // draw a transparent bar over the center
    QColor highlight = palette.color(colorGroup, QPalette::Highlight);
    highlight.setAlpha(150);

    QLinearGradient grad2(0.5, 0, 0.5, 1.0);
    grad2.setColorAt(0, highlight);
    grad2.setColorAt(1.0, highlight.lighter());
    grad2.setCoordinateMode( QGradient::ObjectBoundingMode );
    QBrush gBrush2( grad2 );

    QLinearGradient grad3(0.5, 0, 0.5, 1.0);
    grad3.setColorAt(0, highlight);
    grad3.setColorAt(1.0, highlight.darker());
    grad3.setCoordinateMode( QGradient::ObjectBoundingMode );
    QBrush gBrush3( grad3 );

    painter.fillRect( QRect( 0, h/2 - iH/2, w, iH/2 ), gBrush2 );
    painter.fillRect( QRect( 0, h/2,        w, iH/2 ), gBrush3 );
*/
}

/*!
    Rotates the wheel widget to a given index.
    You can also give an index greater than itemCount or less than zero in which
    case the wheel widget will scroll in the given direction and end up with
    (index % itemCount)
*/
void AbstractWheelWidget::scrollTo(int index)
{
    QScroller *scroller = QScroller::scroller(this);
    scroller->scrollTo(QPointF(0, WHEEL_SCROLL_OFFSET + index * itemHeight()), 500);
    //500ms切换到下一句歌词
}
