#ifndef MYGAMESQL_H
#define MYGAMESQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class MyGameSql : public QObject
{
    Q_OBJECT
public:
    explicit MyGameSql(QObject *parent = nullptr);

    bool is_exist(QString);//判断是否已存在用户名
    bool sign_up(QString account, QString password);//注册处理
    bool new_user_pokemon(QString);//新用户分发精灵
    bool sign_in(QString account, QString password);//登陆处理
    bool off_line(QString account);//下线处理
    bool win_lose_count(QString account, bool win);
    bool pokemon_update(QString account,int no, QList<QByteArray> list);
    bool get_opponent(QString account, QList<QByteArray> list);
    bool discard_pokemon(QString account, int no);
    bool medal_update(QString account, QList<QByteArray> list);

public:
    QSqlDatabase* db;
    QString host = "127.0.0.1"; //数据库ip

signals:

public slots:
};

#endif // MYGAMESQL_H
