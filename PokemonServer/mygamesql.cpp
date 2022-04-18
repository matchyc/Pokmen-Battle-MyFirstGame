#include "mygamesql.h"
#include <QDebug>
#include <QVariant>
#include <QVariantList>
MyGameSql::MyGameSql(QObject *parent) : QObject(parent)
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));

    db->setHostName(host);
    db->setUserName("root");
    db->setPassword("990925");
    db->setDatabaseName("pokemon");

    if(db->open() == false)
    {
        qDebug() << "Failed to connect mysql";
        exit(1);
    }
    else {
        qDebug() << "Connect to mysql successfully";
    }
}

bool MyGameSql::discard_pokemon(QString account, int no)
{
    QSqlQuery query;
    QString sql;

    sql = QString("delete from `"+account+"` where `no` = %1").arg(no);
    if(query.exec(sql))
        return true;
    else
        return false;
}

bool MyGameSql::medal_update(QString account, QList<QByteArray> list)
{
    QSqlQuery query;
    QString sql(QString("update tb_player set `num_gold` = %1, `num_silver` = %2, `num_copper` = %3,"
                "`master_gold` = %4, `master_silver` = %5, `master_copper` = %6 where `id` = '"+account+"'").arg(list[0].toInt()).arg(list[1].toInt())
            .arg(list[2].toInt()).arg(list[3].toInt()).arg(list[4].toInt()).arg(list[5].toInt()));
    if(query.exec(sql))
        return true;
    else
        return false;
}

bool MyGameSql::get_opponent(QString account, QList<QByteArray> list)
{
    QSqlQuery query;
    QString sql;

    int id = list[0].toInt();
    int kind = list[1].toInt();
    QString name = list[2];
    int level = list[3].toInt();
    int exp = list[4].toInt();
    int attack_power = list[5].toInt();
    int defense = list[6].toInt();
    int interval = list[7].toInt();
    int type = list[8].toInt();
    QString skill = list[9];
    int hp = list[10].toInt();
    int mp = list[11].toInt();

    sql = QString("insert into `"+account+"`(`id`,`kind`,`name`,`level`,`exp`,`attack_power`,`defense`,`interval`,"
                                          "`type`,`skill`,`hp`,`mp`) values(%1,%2,'%3',%4,%5,%6,%7,%8,%9,'%10',%11,%12);")
                                        .arg(id).arg(kind).arg(name).arg(level).arg(exp).arg(attack_power).arg
                                        (defense).arg(interval).arg(type).arg(skill).arg(hp).arg(mp);
    qDebug() << "获得对方精灵语句" << sql;
    if(query.exec(sql))
        return true;
    else
        return false;
}

bool MyGameSql::pokemon_update(QString account,int no, QList<QByteArray> list)
{
    QSqlQuery query;
    QString sql;
    QString name = list[0];
    int level = list[1].toInt();
    int exp = list[2].toInt();
    int attack_power = list[3].toInt();
    int defense = list[4].toInt();
    int interval = list[5].toInt();
    int type = list[6].toInt();
    QString skill = list[7];
    int hp = list[8].toInt();
    int mp = list[9].toInt();
    sql = QString("update `"+account+"` set `name`='%1', `level` = %2, `exp`=%3,"
                                     "`attack_power` = %4, `defense` = %5, `interval` = %6,"
                                     "`type` = %7, `skill` = '%8', `hp` = %9, `mp` = %10"
                                     " where `no` = %11").arg(name).arg(level).arg(exp).arg(attack_power).arg
            (defense).arg(interval).arg(type).arg(skill).arg(hp).arg(mp).arg(no);
    qDebug() << "更新精灵的语句" << sql;
    if(query.exec(sql))
        return true;
    else
        return false;
}

bool MyGameSql::new_user_pokemon(QString account)
{
    QSqlQuery query;
    QString sql;
//    sql = "insert into tb_pkm(id,owner,kind,`name`,`level`,exp,attack_power,defense,`interval`,`type`,skill,hp,mp)"
//                "select id, '"+account+"' as `owner`,kind,`name`,`level`,exp,attack_power,defense,`interval`,`type`,skill,hp,mp "
//                "from tb_pkm where id = :id and owner='newuser';";
    sql = "insert into `"+account+"`(id,kind,`name`,`level`,exp,attack_power,defense,`interval`,`type`,skill,hp,mp)"
          "select id,kind,`name`,`level`,exp,attack_power,defense,`interval`,`type`,skill,hp,mp "
          "from tb_pkm where id = :id and owner='newuser';";
    query.prepare(sql);
    QVariantList id_list;
    id_list << 1 << 2 << 3;
    query.bindValue(":id",id_list);
    if(query.execBatch() == false)
    {
        return false;
    }
    else {
//        return true;
    }
    sql = "update tb_player set `pkm_num` = 3 where `id` = "+account+"";
    if( query.exec(sql) == false)
    {
        qDebug() << "更新宠物数量失败";
        return false;
    }
    return true;
}

bool MyGameSql::win_lose_count(QString account, bool win)
{
    QSqlQuery query;
    QString sql;
    if(win)
        sql = "update tb_player set `win` = `win` + 1 where `id` = '"+account+"'";
    else
        sql = "update tb_player set `lose` = `lose` + 1 where `id` = '"+account+"'";
    if(query.exec(sql))
        return true;
    else
        return  false;
}

bool MyGameSql::sign_up(QString account, QString password)
{
    QSqlQuery query;
    QString sql = QString("insert into tb_user(id, psword) values('%1', '%2')").arg(account).arg(password);
    qDebug() << "插入注册用户语句" << sql;
    if(query.exec(sql))
    {
        qDebug() << "注册一个用户，插入数据库成功，账号" << account << "密码" << password;
//        return true;
    }
    else
    {
        qDebug() << "注册一个用户，插入数据库失败，账号" << account;
        return false;
    }
    sql = QString("insert into tb_player(`id`) values('"+account+"');");
    if(query.exec(sql))
    {
        qDebug() << "注册一个用户，插入player表成功，账号" << account;
    }
    else {
         qDebug() << "注册一个用户，插入player表失败，账号" << account;
         return false;
    }

    sql = "create table `"+account+"`(`no` int primary key auto_increment, id int NOT NULL,"
                                  "kind int,`name` varchar(20),`level` int default 0,`exp` int default 0,"
                                  "attack_power int,defense int,`interval` int,`type` int,`skill` varchar(20),hp int,"
                                  "mp int);";

    if(query.exec(sql))
    {
        qDebug() << "创建用户表成功，账号" << account;
    }
    else {
         qDebug() << "创建用户表失败，账号" << account;
         return false;
    }

    return true;
}

bool MyGameSql::sign_in(QString account, QString password)
{
    QSqlQuery query;
    QString sql = "select psword from tb_user where id = '"+account+"'";
    query.exec(sql);
    if(query.next())
    {
        qDebug() << "账号" << account << "对应的密码是" << query.value("psword");
        if(query.value("psword") == password)
        {
            sql = "update tb_player set `online` = 1 where id = '"+account+"';";
            if(query.exec(sql))
            return true;
        }

    }
    return false;
}

bool MyGameSql::off_line(QString account)
{
    QSqlQuery query;
    QString sql("update tb_player set `online` = 0 where id = '"+account+"'");
    if(query.exec(sql))
    {
        qDebug() << account << "成功下线";
        return true;
    }
    return false;
}

bool MyGameSql::is_exist(QString account)
{
    QSqlQuery query;
    account.push_front('\'');
    account.push_back('\'');
    QString sql(QString("select * from tb_user where id=%1").arg(account));
    query.exec(sql);
    if(query.next())
        return false;
    else
        return true;
}
