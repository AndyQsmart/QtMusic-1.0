#include "networkpage.h"
#include "UI/NetworkWidget/labelbutton.h"
#include "UI/NetworkWidget/searchlist.h"
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QTextEdit>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

NetworkPage::NetworkPage(QWidget *parent) : QWidget(parent)
{
    accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(searchFinished(QNetworkReply*)));

    QPalette palette;//调色板
    palette.setColor(QPalette::Background, QColor(244, 244, 244, 102));//设置调色板参数为背景色，RGB颜色为深蓝色
    this->setAutoFillBackground(true);//将组件的背景设为可调
    this->setPalette(palette);//将调色板应用于组件

    palette.setColor(QPalette::Background, QColor(12, 132, 132));//设置调色板参数为背景色，RGB颜色为深蓝色
    logoButton = new LabelButton(this);
    logoButton->setFixedSize(40, 40);
    logoButton->setIcon(":/images/network/netease_icon.jpg");
    keyText = new QLineEdit();
    keyText->setFixedSize(300, 40);
    keyText->setStyleSheet("font: bold large \"宋体\";"
                             "font-size:25px;"
                             "selection-color:rgba(244,244,244,100%);"
                             "background-color:rgba(244,244,244,0%);"
                             "border:2px solid rgb(128, 150, 244);"
                             "border-radius:8px;");
    connect(keyText, SIGNAL(returnPressed()), this, SLOT(searchSongs()));

    searchButton = new LabelButton(this);
    searchButton->setFixedSize(40, 40);
    searchButton->setIcon(":/images/network/searchbutton_icon.jpg");
    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchSongs()));

    QHBoxLayout *midLayout = new QHBoxLayout();
    midLayout->addStretch();
    midLayout->addWidget(logoButton);
    midLayout->addWidget(keyText);
    midLayout->addWidget(searchButton);
    midLayout->addStretch();

    searchList = new SearchList(this);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(midLayout);
    mainLayout->addWidget(searchList);
    //mainLayout->addStretch();
    this->setLayout(mainLayout);

    this->nowPage = 0;
    this->pageCount = 100;
}

void NetworkPage::searchSongs()
{
    QUrl url("http://music.163.com/api/search/get/web");
    QNetworkRequest request(url);
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
    request.setRawHeader("Host", "music.163.com");
    request.setRawHeader("Referer", "http://music.163.com/");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");

    QByteArray data = "";
    data.append("s=" + keyText->text() + "&type=1&offset=" + QString("%1").arg(nowPage*pageCount) + "&total=true&limit=100");
    this->accessManager->post(request, data);
}

void NetworkPage::searchFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError)
        return;
    QJsonObject obj = json.object().find("result").value().toObject();
    int cnt = obj.find("songCount").value().toInt();
    QJsonArray objList = obj.find("songs").value().toArray();
    if (this->nowPage == 0) this->searchList->clearSongs();
    QString name, artist, length;
    for (int i = 0; i < objList.count(); ++i)
    {
        name = objList.at(i).toObject().find("name").value().toString();
        QJsonArray tmp =
                objList.at(i).toObject().find("artists").value().toArray();
        artist = "";
        for (int j = 0; j < tmp.count(); ++j)
        {
            if (j) artist += "&";
            artist += tmp.at(j).toObject().find("name").value().toString();
        }
        int duration = objList.at(i).toObject().find("duration").value().toInt();
        duration /= 1000;
        length = QString("%1:%2").arg(duration/60).arg(duration%60);
        this->searchList->addSong(name, artist, length);
    }
    nowPage++;
    if (nowPage*pageCount >= cnt) nowPage = 0;
    else searchSongs();
}

void NetworkPage::enterEvent(QEvent *e)
{
    emit mouseEnter();
}
