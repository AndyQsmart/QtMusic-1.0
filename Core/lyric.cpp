#include "lyric.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

#include <QDebug>

Lyric::Lyric()
{
    //test
    /*
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    line.push_back("我不想不想");
    */
    getFromFile(":/Beyond-情人.lrc");
}

int Lyric::getIndex(qint64 pos)
{
    //采用二分查找
    int lt, rt, mid;
    lt = 0; rt = postion.count();
    while (lt < rt-1)
    {
        mid = (lt+rt)>>1;
        if (postion[mid] > pos) rt = mid;
        else lt = mid;
    }
    return lt;
}

qint64 Lyric::getPostion(int index)
{
    return postion[index];
}

/*
#标识标签(ID-tags)
格式："[标识名:值]"。大小写等价。
[ar:艺人名]
[ti:曲名]
[al:专辑名]
[by:编者（指编辑LRC歌词的人）]
[offset:时间补偿值]其单位是秒，正值表示整体提前，负值相反。这是用于总体调整显示快慢的。

#时间标签(Time-tag)
格式："[mm:ss]"或"[mm:ss.fff]"（分钟：秒）。数字必须为非负整数。
时间标签需位于某行歌词中的句首部分，一行歌词可以包含多个时间标签。
*/
void Lyric::getFromFile(QString dir)
{
    qDebug() << dir << endl;
    this->filedir = dir;
    //先使用暴力的字符串匹配，还不会正则表达式
    QFile file(dir);
    if (!file.exists()) return;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream read(&file);
    QChar ch;
    bool flag;
    qint64 mm;
    double ss;
    QVector<qint64> muli;
    QMap<qint64, QString> ans;
    QMap<qint64, QString>::iterator it;
    QString sign;
    QString val;
    QString str;
    while (!read.atEnd())
    {
        for (;;)
        {
            read >> ch;
            if (ch == '[' || read.atEnd())
            {
                if (muli.count())
                {
                    for (int i = 0; i < muli.count(); ++i)
                    {
                        ans.insert(muli[i], str);
                        //qDebug() << muli[i] << ":" << str;
                    }
                }
                str = "";
                muli.clear();
                break;
            }
            str = str+ch;
        }
        if (read.atEnd()) break;
        for (;;)
        {
            read >> ch;
            if (ch == ':' || read.atEnd()) break;
            sign = sign+ch;
        }
        if (read.atEnd()) break;
        for (;;)
        {
            read >> ch;
            if (ch == ']' || read.atEnd())
            {
                mm = sign.toLongLong(&flag, 10);
                //判断sign是否是整数
                if (flag)
                {
                    ss = val.toDouble(&flag);
                    if (flag)
                        muli.push_back((qint64)(ss*1000)+mm*60*1000);
                    //qDebug() << mm << ":" << ss << endl;
                }
                else
                {
                    qDebug() << sign << ":" << val;
                }
                break;
            }
            val = val+ch;
        }
        sign = "";
        val = "";
    }
    for (it = ans.begin(); it != ans.end(); ++it)
    {
        this->postion.push_back(it.key());
        this->line.push_back(it.value());
        //qDebug() << it.value() << ":" << it.value();
    }
    //qDebug() << "Lyric all :" << line.count();
}

QString Lyric::getLineAt(int index)
{
    return line[index];
}

int Lyric::getCount()
{
    return line.count();
}
