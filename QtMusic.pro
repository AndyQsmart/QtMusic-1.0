#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T21:13:33
#
#-------------------------------------------------

QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtMusic
TEMPLATE = app


SOURCES += main.cpp\
    musicapplication.cpp \
    UI/ToolWidget/iconbutton.cpp \
    UI/ToolWidget/imagebutton.cpp \
    UI/musicwindow.cpp \
    UI/ToolWidget/mainwindow.cpp \
    UI/ToolWidget/topbar.cpp \
    UI/ToolWidget/mymenu.cpp \
    UI/ToolWidget/skinmenu.cpp \
    UI/ToolWidget/mysystemtrayicon.cpp \
    UI/ToolWidget/bottombar.cpp \
    UI/ToolWidget/sliderwidget.cpp \
    Core/data.cpp \
    Core/musicinfo.cpp \
    Core/myplayer.cpp \
    UI/ToolWidget/voiceslider.cpp \
    UI/ToolWidget/musiclist.cpp \
    UI/ToolWidget/musicpage.cpp \
    UI/ToolWidget/listlist.cpp \
    UI/ToolWidget/fuctionlabel.cpp \
    UI/ToolWidget/lyriclabel.cpp \
    Core/lyric.cpp

HEADERS  += \
    musicapplication.h \
    UI/ToolWidget/iconbutton.h \
    UI/ToolWidget/imagebutton.h \
    UI/musicwindow.h \
    UI/ToolWidget/mainwindow.h \
    UI/ToolWidget/topbar.h \
    UI/ToolWidget/mymenu.h \
    UI/ToolWidget/skinmenu.h \
    UI/ToolWidget/mysystemtrayicon.h \
    UI/ToolWidget/bottombar.h \
    UI/ToolWidget/sliderwidget.h \
    Core/data.h \
    Core/musicinfo.h \
    Core/myplayer.h \
    Core/playmode.h \
    UI/ToolWidget/voiceslider.h \
    UI/ToolWidget/musiclist.h \
    UI/ToolWidget/musicpage.h \
    UI/ToolWidget/listlist.h \
    UI/ToolWidget/fuctionlabel.h \
    UI/ToolWidget/lyriclabel.h \
    Core/lyric.h

RESOURCES += \
    images/images.qrc
