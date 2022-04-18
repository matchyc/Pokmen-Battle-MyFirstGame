#include "gymforbattle.h"
#include "ui_gymforbattle.h"

#include <QCoreApplication>
#include <QTime>
#include <QDebug>
#include <QElapsedTimer>
#include <QMessageBox>
GymForBattle::GymForBattle(bool is_duel, QWidget *parent) :
    QWidget(parent),is_duel_(is_duel),
    ui(new Ui::GymForBattle)
{
    ui->setupUi(this);
    fa = static_cast<PrepareBattle*>(this->parentWidget());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setUpMyGymUI();
    model = new QStandardItemModel(ui->tableView);
    ui->tableView->setModel(model);
    model->setColumnCount(1);
    model->setHeaderData(0,Qt::Horizontal,"战斗记录");
//    set_battle_field();


}

void GymForBattle::deal_info_come(QString info) const
{
    QStandardItem * item = new QStandardItem(info);
    item->setTextAlignment(Qt::AlignCenter);
    item->setEditable(false);
    model->appendRow(item);
    ui->tableView->scrollToBottom();
    qDebug() << "插入了战斗信息";
}

void GymForBattle::set_battle_field() const
{
//    QSize picSize(400,400);
    QPixmap p1_pic;
    QPixmap p2_pic;
    switch (p1->id_) {
    case 0:
        p1_pic.load(":/image/pikachu.png");
        break;
    case 1:
        p1_pic.load(":/image/charmander.png");
        break;
    case 2:
        p1_pic.load(":/image/bulbasaur.png");
        break;
    case 3:
        p1_pic.load(":/image/squirtle.png");
        break;
    }

    switch (p2->id_) {
    case 0:
        p2_pic.load(":/image/pikachu.png");
        break;
    case 1:
        p2_pic.load(":/image/charmander.png");
        break;
    case 2:
        p2_pic.load(":/image/bulbasaur.png");
        break;
    case 3:
        p2_pic.load(":/image/squirtle.png");
        break;
    }
//    p1_pic = p1_pic.scaled(picSize);
//    p2_pic = p2_pic.scaled(picSize);
    ui->p1_label->resize(400,400);
    ui->p2_label->resize(400,400);

    ui->p1_label->setPixmap(p1_pic);
    ui->p2_label->setPixmap(p2_pic);
    ui->p1_label->setScaledContents(true);
    ui->p2_label->setScaledContents(true);

    connect(p1, static_cast<void(Pokemon::*)(QString)>(&Pokemon::send_battle_info),
            this, &GymForBattle::deal_info_come);
    connect(p2, static_cast<void(Pokemon::*)(QString)>(&Pokemon::send_battle_info),
            this, &GymForBattle::deal_info_come);
}

void GymForBattle::setUpMyGymUI()
{
    this->setWindowTitle("Battle!!!");
    QSize picSize(1200,800);
    setAutoFillBackground(true);
    QPixmap back_ground(":/image/gym_background.jpg");
    back_ground = back_ground.scaled(picSize);
    QPalette p(this->palette());
    p.setBrush(QPalette::Window,QBrush(back_ground));
    this->setPalette(p);
}

void GymForBattle::battle_of_two(Pokemon *player1, Pokemon *player2) const
{
    int p1_f = 0;
    int p2_f = 0;
    ui->hp_p1->setRange(0,player1->hp_);
    ui->hp_p2->setRange(0,player2->hp_);

//    clock_t begin,finish;

    while(player1->hp_in_battle_ > 0 && player2->hp_in_battle_ > 0)
    {
        ++p1_f;
        ++p2_f;
        //达到p1攻击频率临界
        if(p1_f == player1->attack_interval_)
        {
            qDebug() << "p1 hit p2";
            player1->attack(player2);
            qDebug() << QString("finish p1 left %1 hp, p2 left %2 hp").arg(player1->hp_in_battle_).arg(player2->hp_in_battle_);
            p1_f = 0;
            QTime t;
            t.start();
            while(t.elapsed() < 1000)
            {
                QApplication::processEvents();
            }
        }
        //达到p2攻击频率临界
        if(p2_f == player2->attack_interval_)
        {
            qDebug() << "p2 hit p1";

            player2->attack(player1);
            QString out = QString("finish p1 left %1 hp, p2 left %2 hp").arg(player1->hp_in_battle_).arg(player2->hp_in_battle_);
            qDebug() << out;
            p2_f = 0;
            QTime t;
            t.start();
            while(t.elapsed() < 1000)
            {
                QApplication::processEvents();
            }
        }

        //如果某一方死亡
        if(player1->hp_in_battle_ <= 0)
            player1->hp_in_battle_ = 0;
        if(player2->hp_in_battle_ <= 0)
            player2->hp_in_battle_ = 0;
        //处理血条
        ui->hp_p1->setValue(player1->hp_in_battle_);
        ui->hp_p2->setValue(player2->hp_in_battle_);
    }

    Pokemon *winner = player1->hp_in_battle_ > 0?   player1 : player2;
    Pokemon *loser = player1->hp_in_battle_ > 0? player2 : player1;
    qDebug() << "Winner:" << winner->name_;

    QString str;
    if(winner->hp_in_battle_ <= 0 && loser->hp_in_battle_ <= 0)
    {
        QMessageBox::information(const_cast<GymForBattle*>(this), "战斗结束","居然是平局！缘分已到！");
        str = ("双方战平！");
    }
    else
    {
        str = QString("获胜者是："+winner->name_+"");
    }

    deal_info_come(str);

    //后续题目战斗完后的其他处理
    if(winner == p1 && winner->hp_in_battle_ > 0)//胜
        fight_complete(1);
    else if(loser->hp_in_battle_ <= 0 && winner->hp_in_battle_ <= 0)//平
        fight_complete(2);
    else//败
        fight_complete(0);
    //之前的
//    player1->hp_in_battle_ = player1->hp_;
//    player2->hp_in_battle_ = player2->hp_;
//    player1->mp_ = player1->mp_;
//    player2->mp_ = player2->mp_;
//    winner->get_exp(true);
//    loser->get_exp(false);

}

void GymForBattle::fight_complete(int flag) const
{
    //回血回蓝
    p1->hp_in_battle_ = p1->hp_;
    p1->mp_in_battle_ = p1->mp_;

    if(flag == 1)
    {
        //增加经验值
        p1->get_exp(true);
        //胜场数或败场数
        fa->send_win_count(true);
        //更新这个出战精灵的信息
        fa->send_pokemon_update(p1);

        //获得对战精灵
        if(is_duel_)
        fa->send_get_opponent(p2);
    }
    else if(flag == 0)
    {

        p1->get_exp(false);
        fa->send_win_count(false);
        fa->send_pokemon_update(p1);
        if(is_duel_)
        fa->battle_failed();
    }

    if(is_duel_)
    fa->medal_check();

    if(flag == 1 && !is_duel_)
    {
        QMessageBox::information(const_cast<GymForBattle*>(this), "战斗结束", "你获胜了！恭喜恭喜");
    }


    else if(flag == 0 && !is_duel_)
    {
        QMessageBox::information(const_cast<GymForBattle*>(this), "战斗结束", "你失败了！再接再厉");
    }
}

GymForBattle::~GymForBattle()
{
    delete ui;
}
