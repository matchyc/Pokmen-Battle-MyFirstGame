#include "preparebattle.h"
#include "ui_preparebattle.h"
#include "mainwindow.h"
#include "gymforbattle.h"
#include "selectdiscard.h"
#include <QModelIndex>
#include <QRandomGenerator>
#include <QDebug>
#include <QList>
#include <QMessageBox>


//PrepareBattle::PrepareBattle(QWidget* parent, QString account):QWidget (parent),account_(account),
//    ui(new Ui::PrepareBattle)
//{

//}
PrepareBattle::PrepareBattle(bool is_duel, QString opponent, QWidget *parent) :
    QWidget(parent),is_duel_(is_duel),oppoent_(opponent),
    ui(new Ui::PrepareBattle)
{
    ui->setupUi(this);
    setUpPreBattleUI();

    //连一下数据库
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    db->setHostName("129.211.84.160");
    db->setUserName("wechat");
    db->setPassword("bupt666");
    db->setDatabaseName("pokemon");

    if(db->open() == false)
    {
        qDebug() << "Failed to connect mysql";
//        exit(1);
    }
    else
    {
        qDebug() << "Connect to mysql successfully";
    }

    //Gym就是道馆，比赛场——实例化
    GymForBattle *my_gym = new GymForBattle(is_duel_, this);
    my_gym->hide();//先隐藏Gym
    my_gym->is_duel_ = this->is_duel_;//是否是决斗赛
    ui->beginBattleButton->setEnabled(false);//都选择以后，开始按钮才能被使用

    //点击开始按钮，显示战斗画面
    connect(ui->beginBattleButton, &QPushButton::clicked, this, [=](){
        my_gym->show();
        my_gym->set_battle_field();
        my_gym->battle_of_two(my_gym->p1, my_gym->p2);
    });

    //避免用户选中多行
    ui->myTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->opponentTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    //点击我方某一精灵，实例化出该精灵
    connect(ui->myTableView, &QTableView::clicked, my_gym, [=](QModelIndex index){
        Pokemon* temp_p1;
        int row = index.row();

        QModelIndex idx =model->index(row,1);
        switch(idx.data().toInt())
       {
        case 0:
        {
            temp_p1 = new Pikachu;
            temp_p1->kind_ = DEXTERITY_KIND;
            temp_p1->type_ = ELECTRIC_TYPE;
            break;
        }
        case 1:
        {
            temp_p1 = new Charmander;
            temp_p1->kind_ = STRENGTH_KIND;
            temp_p1->type_ = FIRE_TYPE;
            break;
        }
        case 2:
        {
            temp_p1 = new Bulbasaur;
            temp_p1->kind_ = TANK_KIND;
            temp_p1->type_ = GRASS_TYPE;
            break;
        }
        case 3:
        {
            temp_p1 = new Squirtle;
            temp_p1->kind_ = DEFENSE_KIND;
            temp_p1->type_ = WATER_TYPE;
            break;
        }
        default:
            temp_p1 = new Pikachu;
            break;
       }
        idx = model->index(row,0);
        temp_p1->no_ = idx.data().toInt();
        for(int i = 2; i < model->columnCount(); ++i)
        {

            QVariant var = model->index(row, i).data();
            switch (i) {
            case 2:
                temp_p1->name_ = var.toString();
                break;
            case 3:
                temp_p1->level_ = var.toInt();
                break;
            case 4:
                temp_p1->exp_ = var.toInt();
                break;
            case 5:
                temp_p1->attack_power_ = var.toInt();
                break;
            case 6:
                temp_p1->defense_ = var.toInt();
                break;
            case 7:
                temp_p1->attack_interval_ = var.toInt();
                break;
            case 8:
                temp_p1->skill_name_ = var.toString();
                break;
            case 9:
                temp_p1->hp_ = var.toInt();
                temp_p1->hp_in_battle_ = var.toInt();
                break;
            case 10:
                temp_p1->mp_ = var.toInt();
                temp_p1->mp_in_battle_ = var.toInt();
                break;
            default:
                break;
            }
        }
        qDebug() << "选择的精灵名字" << temp_p1->name_;
/*
//        QString pkm_name = index.data().toString();
//        qDebug() << "选择的精灵名字" << pkm_name;
//        if(index.column() == 1)
//        {
//            QSqlQuery query;
//            QString sql("select * from tb_pkm where `owner` = '"+account_+"' and `name` = '"+pkm_name+"'");
//            if(query.exec(sql))
//            {
//                qDebug() << "选择我方精灵时，数据库查询成功";
//            }
//            query.next();
//            //选择派生类

//            switch (query.value("id").toInt())
//           {
//            case 0:
//            {
//                temp_p1 = new Pikachu;
//                break;
//            }
//            case 1:
//            {
//                temp_p1 = new Charmander;
//                break;
//            }
//            case 2:
//            {
//                temp_p1 = new Bulbasaur;
//                break;
//            }
//            case 3:
//            {
//                temp_p1 = new Squirtle;
//                break;
//            }
//            default:
//                temp_p1 = new Pikachu;
//                break;
//           }
//            if(1)
//            {
//                qDebug() << "记录有效";
//                temp_p1->kind_ = query.value("kind").toInt();
//                temp_p1->name_ = query.value("name").toString();
//                temp_p1->exp_ = query.value("exp").toInt();
//                temp_p1->level_ = query.value("level").toInt();
//                temp_p1->attack_power_ = query.value("attack_power").toInt();
//                temp_p1->defense_ = query.value("defense").toInt();
//                temp_p1->hp_ = query.value("hp").toInt();
//                temp_p1->attack_interval_ = query.value("interval").toInt();
//                temp_p1->hp_in_battle_ = query.value("hp").toInt();
//                temp_p1->mp_in_battle_ = query.value("mp").toInt();
//                temp_p1->type_ = query.value("type").toInt();
//                temp_p1->skill_name_ = query.value("skill").toString();
//            }

//            qDebug() << "你选择的精灵" << temp_p1->name_;
//        }
*/
            my_gym->p1 = temp_p1;
            click_flag = true;

    });
    //点击对方某一精灵，实例化出该精灵
    connect(ui->opponentTableView, &QTableView::clicked, my_gym, [=](QModelIndex index){
//        qDebug() << index.data().toInt();
        Pokemon* temp_p1;
        int row = index.row();
        QModelIndex idx = opponent_model->index(row,1);
        switch(idx.data().toInt())
       {
        case 0:
        {
            temp_p1 = new Pikachu;
            temp_p1->kind_ = DEXTERITY_KIND;
            temp_p1->type_ = ELECTRIC_TYPE;
            break;
        }
        case 1:
        {
            temp_p1 = new Charmander;
            temp_p1->kind_ = STRENGTH_KIND;
            temp_p1->type_ = FIRE_TYPE;
            break;
        }
        case 2:
        {
            temp_p1 = new Bulbasaur;
            temp_p1->kind_ = TANK_KIND;
            temp_p1->type_ = GRASS_TYPE;
            break;
        }
        case 3:
        {
            temp_p1 = new Squirtle;
            temp_p1->kind_ = DEFENSE_KIND;
            temp_p1->type_ = WATER_TYPE;
            break;
        }
        default:
            temp_p1 = new Pikachu;
            break;
       }
        idx = opponent_model->index(row,0);
        temp_p1->no_ = idx.data().toInt();
        for(int i = 2; i < opponent_model->columnCount(); ++i)
        {

            QVariant var = opponent_model->index(row, i).data();
            switch (i) {
            case 2:
                temp_p1->name_ = var.toString();
                break;
            case 3:
                temp_p1->level_ = var.toInt();
                break;
            case 4:
                temp_p1->exp_ = var.toInt();
                break;
            case 5:
                temp_p1->attack_power_ = var.toInt();
                break;
            case 6:
                temp_p1->defense_ = var.toInt();
                break;
            case 7:
                temp_p1->attack_interval_ = var.toInt();
                break;
            case 8:
                temp_p1->skill_name_ = var.toString();
                break;
            case 9:
                temp_p1->hp_ = var.toInt();
                temp_p1->hp_in_battle_ = var.toInt();
                break;
            case 10:
                temp_p1->mp_ = var.toInt();
                temp_p1->mp_in_battle_ = var.toInt();
                break;
            default:
                break;
            }
        }
        qDebug() << "选择的精灵名字" << temp_p1->name_;
/*
//        if(index.column() == 1)
//        {
//            QSqlQuery query;
//            QString sql("select * from tb_pkm where `owner` = 'newuser' and `name` = '"+pkm_name+"'");
//            query.exec(sql);
//            query.next();
//            //选择派生类
//            switch (query.value("id").toInt())
//           {
//            case 0:
//            {
//                temp_p1 = new Pikachu;
//                break;
//            }
//            case 1:
//            {
//                temp_p1 = new Charmander;
//                break;
//            }
//            case 2:
//            {
//                temp_p1 = new Bulbasaur;
//                break;
//            }
//            case 3:
//            {
//                temp_p1 = new Squirtle;
//                break;
//            }
//            default:
//                temp_p1 = new Pikachu;
//                break;
//           }
//            if(1)
//            {
//                temp_p1->kind_ = query.value("kind").toInt();
//                temp_p1->name_ = query.value("name").toString();
//                temp_p1->exp_ = query.value("exp").toInt();
//                temp_p1->level_ = query.value("level").toInt();
//                temp_p1->attack_power_ = query.value("attack_power").toInt();
//                temp_p1->defense_ = query.value("defense").toInt();
//                temp_p1->hp_ = query.value("hp").toInt();
//                temp_p1->attack_interval_ = query.value("interval").toInt();
//                temp_p1->hp_in_battle_ = query.value("hp").toInt();
//                temp_p1->mp_in_battle_ = query.value("mp").toInt();
//                temp_p1->type_ = query.value("type").toInt();
//                temp_p1->skill_name_ = query.value("skill").toString();
//            }
//        }
*/
        my_gym->p2 = temp_p1;
        if(click_flag)
            ui->beginBattleButton->setEnabled(true);
    });
    //关闭后析构
    setAttribute(Qt::WA_DeleteOnClose);

    //播放战斗音乐
    player = new QMediaPlayer(this);
    QMediaPlaylist *play_list = new QMediaPlaylist(player);
    play_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    play_list->addMedia(QUrl("qrc:/music/fight_music.mp3"));
    play_list->setCurrentIndex(1);
    play_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player->setPlaylist(play_list);
    player->play();
}

void PrepareBattle::battle_failed()
{
    QMessageBox::information(this, "提示", "决斗赛中失败！选择要丢弃的精灵!");
    SelectDiscard *myDialog= new SelectDiscard(this);
    QSqlQuery query;
    QString sql;

    myDialog->model = new QSqlQueryModel(myDialog);

    sql = "SELECT * FROM "+account_+" ORDER BY RAND() LIMIT 3;";

    myDialog->model->setQuery(sql);
    qDebug() << "查询要丢弃的精灵记录完成";
    myDialog->set_plot();

    sql = "select * from "+account_+"";
    query.exec(sql);
    bool flag = false;
    if(query.size() == 1)
    {
        flag = true;
    }

    connect(myDialog, &SelectDiscard::discard_pkm_no, this, [=](int no){
        emit discard_pokemon(no);
    });

    //没有精灵了，获取一只精灵
    if(flag == true)
    {
        sql = "insert into `"+account_+"`(id,kind,`name`,`level`,exp,attack_power,defense,`interval`,`type`,skill,hp,mp)"
                                      "select id,kind,`name`,`level`,exp,attack_power,defense,`interval`,`type`,skill,hp,mp "
                                      "from tb_pkm where owner='newuser' order by rand() limit 1;";
        query.exec(sql);
    }

    myDialog->setModal(true);
    myDialog->exec();
}


void PrepareBattle::medal_check()
{
    QSqlQuery query;
    QString sql;
    sql = "select * from `"+account_+"`";
    if(query.exec(sql))
    {
        qDebug() << "该用户的全部精灵数据查询成功";
    }
    else {
        qDebug() << "该用户的全部精灵数据查询失败";
    }

    int pkm_num = query.size();

    sql = "select * from `"+account_+"` where `level` = 15";
    query.clear();
    if(query.exec(sql))
    {
        qDebug() << "该用户的高级精灵数据查询成功";
    }
    else {
        qDebug() << "该用户的高级精灵数据查询失败";
    }

    int high_level_num = query.size();

    QList<int> medal_list;

    int copper = 0;
    while (pkm_num >= 3) {
        copper += 1;
        pkm_num -= 3;
    }
    int silver = 0;
    while (copper >= 3)
    {
        silver += 1;
        copper -= 3;
    }
    int gold = 0;
    while (silver >= 3)
    {
        gold += 1;
        silver -= 3;
    }
    medal_list << gold << silver << copper;

    copper = 0;
    while (high_level_num >= 3) {
        copper += 1;
        pkm_num -= 3;
    }
    silver = 0;
    while (copper >= 3)
    {
        silver += 1;
        copper -= 3;
    }
    gold = 0;
    while (silver >= 3)
    {
        gold += 1;
        silver -= 3;
    }
    medal_list << gold << silver << copper;

    sql = "select num_gold, num_silver, num_copper, master_gold, master_silver, master_copper from tb_player where `id` = '"+account_+"'";
    query.clear();
    query.exec(sql);
    if(query.next())
    {

        if(query.value(0) != medal_list[0] || query.value(1) != medal_list[1]
                || query.value(2) != medal_list[2] || query.value(3) != medal_list[3]
                || query.value(4) != medal_list[4] || query.value(5) != medal_list[5])
        {
                QMessageBox::information(this, "提示", "获得了新的奖章!请到用户信息列表查看！");
        }

    }
    emit medal_update(medal_list);
}

void PrepareBattle::send_win_count(bool win)
{
    emit win_or_lose(win);
}

void PrepareBattle::send_pokemon_update(Pokemon *my_pkm)
{
    emit update_pokemon(my_pkm);
}


void PrepareBattle::get_account(QString account)
{
    account_ = account;
}

void PrepareBattle::send_get_opponent(Pokemon *new_pkm)
{
    QMessageBox::information(this, "提示", "决斗赛中获胜！获得对方精灵!");
    emit get_opponent(new_pkm);
}

void PrepareBattle::get_pokemon_table()
{
    model = new QSqlQueryModel(this);
    QString sql;
    opponent_model = new QSqlQueryModel(this);
    QString sql_2;
    //未使用用户单独表的sql语句
//    sql = "select `id`,`name`,`level`,`exp`,`attack_power`,`defense`,`interval`,`skill`,`hp`,`mp` from tb_pkm where owner = '"+account_+"'";


    sql_2 = "select `no`,`id`,`name`,`level`,`exp`,`attack_power`,`defense`,`interval`,`skill`,`hp`,`mp` from `"+oppoent_+"`";
    //使用用户单独表
    sql = "select `no`,`id`,`name`,`level`,`exp`,`attack_power`,`defense`,`interval`,`skill`,`hp`,`mp` from `"+account_+"`";
//    sql_2 = "select `id`,`name`,`level`,`exp`,`attack_power`,`defense`,`interval`,`skill`,`hp`,`mp` from `"+account_+"`";

//    sql_2 =  "select `owner`,`name`,`level` from tb_pkm where `owner` = 'newuser'";
    model->setQuery(sql);
    opponent_model->setQuery(sql_2);
    model->setHeaderData(0, Qt::Horizontal, tr("所属用户"));
    model->setHeaderData(1, Qt::Horizontal, tr("名字"));
    model->setHeaderData(2, Qt::Horizontal, tr("名字"));
    model->setHeaderData(3, Qt::Horizontal, tr("等级"));
    model->setHeaderData(4, Qt::Horizontal, tr("经验值"));
    model->setHeaderData(5, Qt::Horizontal, tr("攻击力"));
    model->setHeaderData(6, Qt::Horizontal, tr("防御力"));
    model->setHeaderData(7, Qt::Horizontal, tr("攻击间隔"));
    model->setHeaderData(8, Qt::Horizontal, tr("技能名称"));
    model->setHeaderData(9, Qt::Horizontal, tr("血量"));
    model->setHeaderData(10, Qt::Horizontal, tr("蓝量"));
    opponent_model->setHeaderData(0, Qt::Horizontal, tr("所属用户"));
    opponent_model->setHeaderData(1, Qt::Horizontal, tr("名字"));
    opponent_model->setHeaderData(2, Qt::Horizontal, tr("名字"));
    opponent_model->setHeaderData(3, Qt::Horizontal, tr("等级"));
    opponent_model->setHeaderData(4, Qt::Horizontal, tr("经验值"));
    opponent_model->setHeaderData(5, Qt::Horizontal, tr("攻击力"));
    opponent_model->setHeaderData(6, Qt::Horizontal, tr("防御力"));
    opponent_model->setHeaderData(7, Qt::Horizontal, tr("攻击间隔"));
    opponent_model->setHeaderData(8, Qt::Horizontal, tr("技能名称"));
    opponent_model->setHeaderData(9, Qt::Horizontal, tr("血量"));
    opponent_model->setHeaderData(10, Qt::Horizontal, tr("蓝量"));
//    opponent_model->setHeaderData(0, Qt::Horizontal, tr("所属用户"));
//    opponent_model->setHeaderData(1, Qt::Horizontal, tr("名字"));
//    opponent_model->setHeaderData(2, Qt::Horizontal, tr("等级"));

    ui->opponentTableView->setStyleSheet("background-color:transparent");
    ui->myTableView->setStyleSheet("background-color:transparent");
    ui->myTableView->setModel(model);
    ui->myTableView->setColumnHidden(0, true);
    ui->myTableView->setColumnHidden(1, true);
    ui->opponentTableView->setModel(opponent_model);
    ui->opponentTableView->setColumnHidden(0, true);
    ui->opponentTableView->setColumnHidden(1, true);
    ui->myTableView->show();
    ui->opponentTableView->show();
}



void PrepareBattle::setUpPreBattleUI()
{
    this->setWindowTitle("Battle!!!");
    QSize picSize(1200,800);
    setAutoFillBackground(true);
    QPixmap back_ground(":/image/prepare_battle.jpg");
    back_ground = back_ground.scaled(picSize);
    QPalette p(this->palette());
    p.setBrush(QPalette::Window,QBrush(back_ground));
    this->setPalette(p);
}

PrepareBattle::~PrepareBattle()
{
    player->stop();
    emit play_main_music();

    delete ui;
}
