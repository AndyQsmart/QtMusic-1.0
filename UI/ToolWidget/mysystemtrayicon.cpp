#include "mysystemtrayicon.h"
#include "Core/playmode.h"
#include <QMenu>
#include <QAction>

MySystemTrayIcon::MySystemTrayIcon(QWidget *parent) : QSystemTrayIcon(parent)
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
        return;
    QMenu *menu = new QMenu();
    menu->setStyleSheet("QMenu{background:rgba(255,255,255,100%);}"
                        "QMenu::item{padding:6px 42px;}"
                        "QMenu::item:selected{background:rgba(0,128,180,80%);}");

    QAction *quit = new QAction("退出", menu);
    quit->setIcon(QIcon(":/images/systemtrayicon/close_icon"));
    QAction *showNor = new QAction("显示主窗口", menu);
    connect(showNor, SIGNAL(triggered()), this, SIGNAL(showWindow()));
    connect(quit, SIGNAL(triggered()), this, SIGNAL(quit()));

    playMode = new QMenu("播放模式");
    playMode->setStyleSheet("QMenu{background:rgba(255,255,255,100%);}"
                            "QMenu::item{padding:6px 42px;}"
                            "QMenu::item:selected{background:rgba(0,128,180,80%);}");
    QAction *onemusic = new QAction("单曲播放", playMode);
    QAction *onerep = new QAction("单曲循环", playMode);
    QAction *turnmusic = new QAction("顺序播放", playMode);
    QAction *allrep = new QAction("循环播放", playMode);
    QAction *randmusic = new QAction("随机播放", playMode);
    connect(onemusic, SIGNAL(triggered()), this, SLOT(setModeonemusic()));
    connect(onerep, SIGNAL(triggered(bool)), this, SLOT(setModeonerep()));
    connect(turnmusic, SIGNAL(triggered(bool)), this, SLOT(setModeturnmusic()));
    connect(allrep, SIGNAL(triggered(bool)), this, SLOT(setModeallrep()));
    connect(randmusic, SIGNAL(triggered(bool)), this, SLOT(setModerandmusic()));
    playMode->addAction(onemusic);
    playMode->addAction(onerep);
    playMode->addAction(turnmusic);
    playMode->addAction(allrep);
    playMode->addAction(randmusic);

    menu->addAction(showNor);
    menu->addSeparator();
    menu->addMenu(playMode);
    menu->addAction(quit);
    this->setIcon(QIcon(":/images/icons/app_icon"));
    this->setContextMenu(menu);
    this->setToolTip("QtMusic");
    this->show();
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MySystemTrayIcon::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            //break;
        case QSystemTrayIcon::DoubleClick:
            emit showWindow();
            break;
        default:
            break;
    }
}

void MySystemTrayIcon::setPlayMode(int mode)
{
    switch (mode)
    {
        case CurrentItemOnce:
            playMode->setIcon(QIcon(":/images/systemtrayicon/onemusic_icon"));
            playMode->setTitle("单曲播放");
            break;
        case CurrentItemInLoop:
            playMode->setIcon(QIcon(":/images/systemtrayicon/onerep_icon"));
            playMode->setTitle("单曲循环");
            break;
        case Sequential:
            playMode->setIcon(QIcon(":/images/systemtrayicon/turnmusic_icon"));
            playMode->setTitle("顺序播放");
            break;
        case Loop:
            playMode->setIcon(QIcon(":/images/systemtrayicon/allrep_icon"));
            playMode->setTitle("循环播放");
            break;
        case Random:
            playMode->setIcon(QIcon(":/images/systemtrayicon/randmusic_icon"));
            playMode->setTitle("随机播放");
            break;
    }
}

void MySystemTrayIcon::setModeonemusic()
{
    emit setMode(CurrentItemOnce);
}

void MySystemTrayIcon::setModeonerep()
{
    emit setMode(CurrentItemInLoop);
}

void MySystemTrayIcon::setModeturnmusic()
{
    emit setMode(Sequential);
}

void MySystemTrayIcon::setModeallrep()
{
    emit setMode(Loop);
}

void MySystemTrayIcon::setModerandmusic()
{
    emit setMode(Random);
}
