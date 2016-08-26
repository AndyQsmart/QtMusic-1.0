#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QMutex>
#include <QWidget>

class LabelButton;
class QNetworkAccessManager;
class QNetworkReply;
class QLineEdit;
class SearchList;

class QTextEdit;

class NetworkPage : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkPage(QWidget *parent = 0);

signals:
    void mouseEnter();

protected:
    void enterEvent(QEvent *e);//鼠标进入事件

private slots:
    void searchSongs();
    void searchFinished(QNetworkReply *reply);

private:
    LabelButton *logoButton;
    LabelButton *searchButton;
    QLineEdit *keyText;
    QNetworkAccessManager *accessManager;
    SearchList *searchList;
    int nowPage;
    int pageCount;
};

#endif // NETWORKPAGE_H
