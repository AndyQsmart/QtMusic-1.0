#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>
#include <QLabel>

class SliderWidget;
class VoiceSlider;
class IconButton;
class MyLabel;

class BottomBar : public QWidget
{
        Q_OBJECT
    public:
        explicit BottomBar(QWidget *parent = 0);
        int getPlayMode();
        int getSoundLevel();
        void setSoundLevel(int sound);
        void setMusicTitle(QString name);

    public slots:
        void setPlayMode(int mode);
        void setPostionAvailable();
        void setPostionNotAvailable();
        void setMaxDuration(qint64 max);
        void changePostionTo(qint64 pos);
        void setPlay();
        void setPause();

    protected:
        void dealMouse();

    signals:
        void mouseEnter();
        void showLyric(QString name);
        void setPostion(qint64 pos);//
        void tryplay();
        void pause();
        void playPre();
        void playNext();
        void setVoice(int voice);
        void setMode(int mode);

    private slots:
        void clickCover();
        void postionChanged(qreal pos);
        void voiceChanged(qreal pos);
        void clickSoundButton();
        void setModeonemusic();
        void setModeonerep();
        void setModeturnmusic();
        void setModeallrep();
        void setModerandmusic();

    private:
        SliderWidget *postionSlider;

        IconButton *coverButton;
        MyLabel *nameLabel;
        MyLabel *timeLabel;
        qint64 maxDuration;

        IconButton *preButton;
        IconButton *playButton;
        IconButton *nextButton;

        bool nosound;
        IconButton *soundButton;
        VoiceSlider *voiceSlider;

        int playmode;
        MyLabel *modeLabel;
        IconButton *modeButton;
};

class MyLabel : public QLabel
{
        Q_OBJECT
    public:
        MyLabel(QWidget *parent = 0);

    signals:
        void mouseEnter();

    private:
        void enterEvent(QEvent *e);//鼠标进入事件
};

#endif // BOTTOMBAR_H
