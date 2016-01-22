#include "skinmenu.h"
#include "UI/ToolWidget/iconbutton.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QtDebug>

SkinMenu::SkinMenu(QWidget *parent) : QWidget(parent)
{
    skinButton = new IconButton*[4];
    QVBoxLayout *vLayout = new QVBoxLayout();
    for (int i = 0; i < 4; ++i)
    {
        QHBoxLayout *hLayout = new QHBoxLayout();
        skinButton[i] = new IconButton[5];
        for (int j = 0; j < 5; ++j)
        {
            QString str = ":/images/icons/skins/skin";
            char ch = '0'+i;
            str = str+ch;
            ch = '0'+j;
            str = str+ch;
            skinButton[i][j].setTheIcon(str);
            skinButton[i][j].setFixedSize(120, 80);
            skinButton[i][j].setIJ(i, j);
            hLayout->addWidget(&skinButton[i][j]);
            connect(&skinButton[i][j], SIGNAL(clicked(int,int)), this, SLOT(clickSkin(int ,int)));
        }
        vLayout->addLayout(hLayout);
    }
    vLayout->setSpacing(2);
    vLayout->setMargin(1);
    this->setLayout(vLayout);
}

void SkinMenu::clickSkin(int i, int j)
{
    if (i != 3 || j != 4)
    {
        QString str = ":/images/skins/skin";
        char ch = '0'+i;
        str = str+ch;
        ch = '0'+j;
        str = str+ch;
        emit changeSkin(str);
    }
    else
    {
        QString fileDir = QFileDialog::getOpenFileName(
                    this,
                    tr("添加"),
                    ".",
                    tr("所有图片文件(*.jpg *.jpeg *.jpe *.jfif *.png *.jif);; JPEG(*.jpg *.jpeg *.jpe *.jfif);; PNG(*.png);; GIF(*.jif)"));
        emit changeSkin(fileDir);
    }
}
