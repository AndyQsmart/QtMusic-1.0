#include "networkpage.h"
#include "UI/NetworkWidget/labelbutton.h"
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>

NetworkPage::NetworkPage(QWidget *parent) : QWidget(parent)
{
    QPalette palette;//调色板
    palette.setColor(QPalette::Background, QColor(244, 244, 244, 102));//设置调色板参数为背景色，RGB颜色为深蓝色
    this->setAutoFillBackground(true);//将组件的背景设为可调
    this->setPalette(palette);//将调色板应用于组件

    //test
    palette.setColor(QPalette::Background, QColor(12, 132, 132));//设置调色板参数为背景色，RGB颜色为深蓝色
    logo = new LabelButton(this);
    logo->setFixedSize(40, 40);
    logo->setIcon(":/images/network/netease_icon.jpg");
    QLineEdit *textLabel = new QLineEdit();
    textLabel->setFixedSize(300, 40);
    textLabel->setStyleSheet("font: bold large \"宋体\";"
                             "font-size:25px;"
                             "selection-color:rgba(244,244,244,100%);"
                             "background-color:rgba(244,244,244,0%);"
                             "border:2px solid rgb(128, 150, 244);"
                             "border-radius:8px;");
    search = new LabelButton(this);
    search->setFixedSize(40, 40);
    search->setIcon(":/images/network/searchbutton_icon.jpg");

    QHBoxLayout *midLayout = new QHBoxLayout();
    midLayout->addStretch();
    midLayout->addWidget(logo);
    midLayout->addWidget(textLabel);
    midLayout->addWidget(search);
    midLayout->addStretch();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(midLayout);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
}

void NetworkPage::enterEvent(QEvent *e)
{
    emit mouseEnter();
}
