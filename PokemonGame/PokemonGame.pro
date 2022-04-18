#-------------------------------------------------
#
# Project created by QtCreator 2020-08-10T14:15:38
#
#-------------------------------------------------

QT       += core gui sql network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokemonGame
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    pokemon.cpp \
    loginwidget.cpp \
    signupdialog.cpp \
    client.cpp \
    playerlistdialog.cpp \
    gymforbattle.cpp \
    preparebattle.cpp \
    selectdiscard.cpp \
    watchpokemon.cpp \
    challengedialog.cpp

HEADERS += \
        mainwindow.h \#游戏主体窗口
    pokemon.h \ #宝可梦类，无实际窗口
    loginwidget.h \ #登录窗口
    signupdialog.h \#注册窗口
    client.h \#客户端操作类，无实际窗口，封装发送各种报文的函数
    playerlistdialog.h \#用户列表窗口
    gymforbattle.h \#战斗窗口
    preparebattle.h \#准备战斗窗口
    selectdiscard.h \ #决斗赛战败丢弃精灵窗口
    watchpokemon.h \ #用户精灵查看
    challengedialog.h #挑战大厅窗口

FORMS += \
        mainwindow.ui \
    loginwidget.ui \
    signupdialog.ui \
    playerlistdialog.ui \
    gymforbattle.ui \
    preparebattle.ui \
    selectdiscard.ui \
    watchpokemon.ui \
    challengedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc \
    music.qrc

