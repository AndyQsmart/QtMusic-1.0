#include "musicwindow.h"
#include "UI/ToolWidget/topbar.h"
#include "UI/ToolWidget/bottombar.h"
#include "UI/ToolWidget/lyriclabel.h"
#include "UI/ToolWidget/musicpage.h"
#include "UI/ToolWidget/mysystemtrayicon.h"
#include "Core/data.h"
#include "Core/myplayer.h"
#include <QIcon>
#include <QPainter>
#include <QtWidgets/QVBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QQueue>
#include <QFileDialog>
#include <QStackedWidget>

MusicWindow::MusicWindow(QWidget *parent) : MainWindow(parent)
{
    this->setWindowIcon(QIcon(":/images/icons/app_icon"));

    systemTrayIcon = new MySystemTrayIcon(this);
    connect(systemTrayIcon, SIGNAL(showWindow()), this, SLOT(show()));
    connect(systemTrayIcon, SIGNAL(quit()), this, SLOT(close()));

    player = new MyPlayer();
    connect(player, SIGNAL(playerStateChanged(int)), this, SLOT(playerStateChanged(int)));
    connect(player, SIGNAL(musicChanged(QString,int)), this, SLOT(musicChanged(QString,int)));

    topBar = new TopBar(this);
    connect(topBar, SIGNAL(setMinSize()), this, SLOT(showMinimized()));
    connect(topBar, SIGNAL(setMaxSize()), this, SLOT(setMaxSize()));
    connect(topBar, SIGNAL(closeWindow()), this, SLOT(hide()));
    connect(topBar, SIGNAL(quit()), this, SLOT(close()));
    connect(topBar, SIGNAL(setBackgroud(QString)), this, SLOT(setBackgroud(QString)));
    connect(topBar, SIGNAL(aboutQtMusic()), this, SLOT(aboutQtMusic()));

    fuctionPage = new QStackedWidget;
    fuctionPage->setMinimumWidth(200);

    lyricLabel = new LyricLabel(false, this);
    connect(player, SIGNAL(positionChanged(qint64)), lyricLabel, SLOT(postionChanged(qint64)));
    connect(lyricLabel, SIGNAL(changeTo(qint64)), player, SLOT(setPosition(qint64)));

    QLabel *label2 = new QLabel("Network");
    label2->setAlignment(Qt::AlignCenter);
    QLabel *label3 = new QLabel("Download");
    label3->setAlignment(Qt::AlignCenter);
    fuctionPage->addWidget(lyricLabel);
    fuctionPage->addWidget(label2);
    fuctionPage->addWidget(label3);
    fuctionPage->setCurrentIndex(0);
    connect(topBar, SIGNAL(selectFuction(int)), fuctionPage, SLOT(setCurrentIndex(int)));

    musicPage = new MusicPage;
    musicPage->setMaximumWidth(500);
    musicPage->setMinimumWidth(300);
    connect(musicPage, SIGNAL(tryToCreateList(QString)), this, SLOT(tryToCreateList(QString)));
    connect(musicPage, SIGNAL(deleteList(QString)), this, SLOT(deleteList(QString)));
    connect(musicPage, SIGNAL(addMusics()), this, SLOT(addMusics()));
    connect(musicPage, SIGNAL(play(QString,int)), player, SLOT(playTheMusic(QString,int)));
    connect(musicPage, SIGNAL(removeTheMusic(QString,int)), this, SLOT(removeTheMusic(QString,int)));
    connect(musicPage, SIGNAL(removeAllMusics(QString)), this, SLOT(removeAllMusics(QString)));

    bottomBar = new BottomBar(this);
    connect(bottomBar, SIGNAL(setVoice(int)), player, SLOT(setVoice(int)));
    connect(bottomBar, SIGNAL(setMode(int)), player, SLOT(setPlayMode(int)));
    connect(bottomBar, SIGNAL(setPostion(qint64)), player, SLOT(setPosition(qint64)));
    connect(bottomBar, SIGNAL(setPostion(qint64)), lyricLabel, SLOT(setPostion(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), bottomBar, SLOT(setMaxDuration(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), bottomBar, SLOT(changePostionTo(qint64)));
    connect(musicPage, SIGNAL(play(QString,int)), bottomBar, SLOT(setPostionAvailable()));
    connect(bottomBar, SIGNAL(tryplay()), this, SLOT(clickPlay()));
    connect(bottomBar, SIGNAL(playPre()), this, SLOT(playPre()));
    connect(bottomBar, SIGNAL(playNext()), this, SLOT(playNext()));
    connect(systemTrayIcon, SIGNAL(setMode(int)), bottomBar, SLOT(setPlayMode(int)));
    connect(bottomBar, SIGNAL(setMode(int)), systemTrayIcon, SLOT(setPlayMode(int)));
    connect(bottomBar, SIGNAL(showLyric(QString)), topBar, SLOT(changeFuction(QString)));
    //connect(lyricLabel, SIGNAL(changeTo(qint64)), bottomBar, SLOT(changePostionTo(qint64)));
    //connect(lyricLabel, SIGNAL(changeTo(qint64)), bottomBar, SIGNAL(setPostion(qint64)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topBar);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    //设定分割窗口的分割条在拖动时是否为实时更新 true为实时更新 false则在拖动时只显示一条灰色的线条
    //在拖动到位并弹起鼠标后再显示分隔条
    splitter->setOpaqueResize(false);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(fuctionPage);
    splitter->addWidget(musicPage);
    splitter->setStretchFactor(0, 10);
    splitter->setStretchFactor(1, 1);
    splitter->setStyleSheet("QSplitter::handle { background-color: rgba(0,0,0,0%) }");
    splitter->setHandleWidth(4);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(bottomBar);
    mainLayout->setMargin(1);
    mainLayout->setSpacing(3);
    this->setLayout(mainLayout);

    dealMouse();
    readData();
}

MusicWindow::~MusicWindow()
{
    Data::changePlayMode(bottomBar->getPlayMode());
    Data::changeSoundLevel(bottomBar->getSoundLevel());
}

void MusicWindow::readData()
{
    if (!Data::connectData()) return;
    this->setBackgroud(Data::getCurrentBackground());
    this->bottomBar->setPlayMode(Data::getPlayMode());
    this->bottomBar->setSoundLevel(Data::getSoundLevel());

    QQueue <QString> qq = Data::getListList();
    QString listName;
    QQueue <MusicInfo> q;
    while (!qq.empty())
    {
        listName = qq.front();
        qq.pop_front();
        musicPage->createList(listName);
        q = Data::getMusicList(listName);
        musicPage->addMusics(Data::getListId(listName), q);
        player->addMusics(listName, q);
    }
    musicPage->setCurrentList(0);
}

void MusicWindow::playerStateChanged(int state)
{
    switch (state)
    {
        case MyPlayer::NOMUSIC:
            bottomBar->setPause();
            bottomBar->setMusicTitle("歌曲名字");
            bottomBar->setMaxDuration(0);
            bottomBar->setPostionNotAvailable();
            break;
        case MyPlayer::PAUSE:
            bottomBar->setPause();
            bottomBar->setPostionAvailable();
            break;
        case MyPlayer::PLAYING:
            bottomBar->setPlay();
            bottomBar->setPostionAvailable();
            break;
    }
}

void MusicWindow::musicChanged(QString listName, int index)
{
    qDebug() << "music changed" << endl;
    bottomBar->setMusicTitle(Data::getMusicName(listName, index));
    musicPage->removeHighLight();
    musicPage->setHighLight(Data::getListId(listName), index);

    //处理歌词
    //先对数据库查询歌词
    QString dir = Data::getMusicDir(listName, index);
    int len = dir.length();
    while (dir.at(len-1) != '.')
    {
        dir.remove(len-1, 1);
        len--;
    }
    dir = dir+"lrc";
    lyricLabel->getFromFile(dir);
}

void MusicWindow::clickPlay()
{
    switch (player->getPlayerState())
    {
        case MyPlayer::NOMUSIC:
            break;
        case MyPlayer::PAUSE:
            player->setPlay();
            //bottomBar->setPlay();
            break;
        case MyPlayer::PLAYING:
            player->setPause();
            //bottomBar->setPause();
            break;
    }
}

void MusicWindow::playPre()
{
    if (player->getPlayerState() == MyPlayer::NOMUSIC) return;
    if (!player->playPre()) return;
}

void MusicWindow::playNext()
{
    if (player->getPlayerState() == MyPlayer::NOMUSIC) return;
    if (!player->playNext()) return;
}

void MusicWindow::addMusics()
{
    QQueue<MusicInfo> musics;
    MusicInfo musicInfo;
    QStringList fileDir = QFileDialog::getOpenFileNames(
                this,
                tr("添加"),
                ".",
                tr("MP3 音频文件(*.mp1 *.mp2 *.mp3);; WMA 音频文件(*.wma)"));
    if (fileDir.isEmpty())
        return;
    QStringList::iterator it;
    musicPage->addMusics(fileDir);
    for (it = fileDir.begin(); it != fileDir.end(); ++it)
    {
        musicInfo.setDir(*it);
        musicInfo.setName(QFileInfo(*it).baseName());
        musicInfo.setArtist("");
        musics.push_back(musicInfo);
    }
    player->addMusics(musicPage->getCurrentList(), musics);
    Data::addMusicsToEnd(musicPage->getCurrentList(), musics);
}

void MusicWindow::removeTheMusic(QString listName, int index)
{
    player->removeTheMusic(listName, index);
    Data::deleteMusic(listName, index);
}

void MusicWindow::removeAllMusics(QString listName)
{
    player->removeAllMusics(listName);
    Data::deleteAllMusic(listName);
}

void MusicWindow::tryToCreateList(QString name)
{
    if (name.size() && Data::addList(name))
        musicPage->createList(name);
}

void MusicWindow::deleteList(QString name)
{
    Data::deleteList(name);
    //try to delete from player;
}

void MusicWindow::dealMouse()
{
    topBar->setMouseTracking(true);
    connect(topBar, SIGNAL(mouseEnter()), this, SLOT(loseMouse()));
    musicPage->setMouseTracking(true);
    connect(musicPage, SIGNAL(mouseEnter()), this, SLOT(loseMouse()));
    bottomBar->setMouseTracking(true);
    connect(bottomBar, SIGNAL(mouseEnter()), this, SLOT(loseMouse()));
}

void MusicWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), currentBackground);
}

void MusicWindow::setBackgroud(QString dir)
{
    //此处需要对文件存在进行处理？
    //QFile filedir(dir);
    try
    {
        if (dir != "")
        {
            currentBackground = QPixmap(dir);
            this->update();
            Data::changeCurrentBackground(dir);
        }
    }
    catch (QString e)
    {
    }
}

void MusicWindow::aboutQtMusic()
{
    QMessageBox::about(this, tr("关于QtMusic"), tr("QtMusic，感谢您的使用！\n此软件仅模仿酷我音乐制作，请勿用于任何非法途径。"));
}
