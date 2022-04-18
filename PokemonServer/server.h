#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QMap>
#include "mygamesql.h"

// ***************************************
// 操作数 | 数据 | 数据 | 数据 | 数据 |...|\n
//  int  | QByteArray | QByteArray | QByteArray | ... | \n


enum{SIGN_UP, SIGN_IN, FIGHT_RESULT, UPDATE_PKM, GET_OPPONENT, DISCARD_PKM, MEDAL_UPDATE};

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void on_read_data();//接受数据时的函数
    void deal_with_connection();//有连接请求时
    void deal_with_disconnection();//连接断开时
    void sign_up(QString, QString, QTcpSocket*);//注册请求
    void new_user_pokemon(QString);//新用户发精灵
    void sign_in(QString, QString, QTcpSocket*);//登录请求
    void win_lose_count(QString,bool);//胜负场消息
    void pokemon_update(QString account, int no, QList<QByteArray> list);//宝可梦信息更新，list里面是各个属性
    void get_opponent(QString account, QList<QByteArray> list);//得到对方精灵
    void discard_pokemon(QString account, int no);//丢弃一个精灵
    void medal_update(QString account, QList<QByteArray>);//更新奖牌信息

public:
    QTcpServer *tcpServer;//监听套接字
    QTcpSocket *tcpSocket;//通信套接字
    quint16 port = 14444;//端口
    MyGameSql *db;//数据库
    QMap<QTcpSocket*,QString> online_map;//套接字到账号的映射


signals:

public slots:
};

#endif // SERVER_H
