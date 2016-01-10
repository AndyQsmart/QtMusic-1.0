#include "mysystemtrayicon.h"
#include <QMenu>
#include <QAction>

MySystemTrayIcon::MySystemTrayIcon(QWidget *parent) : QSystemTrayIcon(parent)
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
        return;
    QMenu *menu = new QMenu();
    menu->setStyleSheet("background:rgba(255,255,255,100%);"
                            "selection-background-color:rgba(0,128,180,80%);");

    QAction *quit = new QAction("退出", menu);
    QAction *showNor = new QAction("显示主窗口", menu);
    connect(showNor, SIGNAL(triggered()), this, SIGNAL(showWindow()));
    connect(quit, SIGNAL(triggered()), this, SIGNAL(quit()));

    menu->addAction(showNor);
    menu->addSeparator();
    menu->addAction(quit);
    this->setIcon(QIcon(":/icons/res/icons/app_icon.gif"));
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
