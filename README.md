# QtMusic
This is a music player written by C++ GUI Qt
QtMusic是基于Qt C++实现的，编译后支持跨平台运行。预计将包含歌曲播放、歌单管理、歌词显示以及网络歌曲等功能，界面友好。

#目前实现以下模块
#歌曲播放
使用Qt自带的媒体库实现了歌曲的播放，支持mp3、wma等音乐格式。

#歌单管理
使用Qt自带的图形库实现了歌单管理的界面，重写一些事件函数，实现了新建歌单、托砖歌曲、歌曲高亮等功能。使用Qt封装的SQLite数据库，实现了歌曲、歌单的存档。

#歌词显示
通过手写字符串匹配解析了lrc格式的歌词文件，然后通过二分查找实现了显示某时刻的歌词。

#为什么使用Qt GUI
*Qt GUI库支持跨平台，而且图形库比较强大。
*Qt支持C++编写，运行效率相对较高。
*C++编写下支持指针，相对灵活一些。
*Qt封装了C++原有的STL函数库，可以方便的实现一些数据结构上的功能。

#为什么使用SQLite
*Qt自带的库封装了SQLite，跨平台方面支持较好。
*采用传统的文件存档效率较低，数据库效率相对较高。这样可以实时存档，即使在程序崩溃的情况下，也能保存较新的存档。
*在载入大量歌曲的情况下，SQLite支持事务处理，即使在不使用多线程的情况下，界面响应也比较及时。

#界面效果图
![image](https://github.com/AndyQsmart/QtMusic-1.0/blob/master/readme_picture/pic1.png)
![image](https://github.com/AndyQsmart/QtMusic-1.0/blob/master/readme_picture/pic2.png)
![image](https://github.com/AndyQsmart/QtMusic-1.0/blob/master/readme_picture/pic3.png)


