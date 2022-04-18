#include "challengedialog.h"
#include "ui_challengedialog.h"
#include "watchpokemon.h"
#include "preparebattle.h"
#include "mainwindow.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QModelIndex>
#include <QtDebug>
ChallengeDialog::ChallengeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChallengeDialog)
{
    ui->setupUi(this);
    //连接数据库
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
    else {
        qDebug() << "Connect to mysql successfully";
    }

    setChallengeUI();

    connect(ui->tableView, &QTableView::clicked, this,[=](QModelIndex index){
        if(index.column() == 0)
        {
            QString account = index.data().toString();

            WatchPokemon myDialog(this);

            myDialog.setWindowTitle(""+account+"的精灵");

            QString sql("select `id`,`name`,`level`,`skill` from `"+account+"`");
            QSqlQueryModel *model = new QSqlQueryModel(this);
            myDialog.model = model;
            myDialog.model->setQuery(sql);

            myDialog.model->setHeaderData(0, Qt::Horizontal, tr("名字"));
            myDialog.model->setHeaderData(1, Qt::Horizontal, tr("等级"));
            myDialog.model->setHeaderData(2, Qt::Horizontal, tr("技能"));

            myDialog.setWatchUI();
            myDialog.setModal(true);
            myDialog.exec();

        }

    });



    connect(ui->tableView, &QTableView::clicked, this, &ChallengeDialog::challenge_battle);
    connect(this, &QDialog::accepted, this, [=](){
       emit give_opponent(opponent_);
    });

}

void ChallengeDialog::challenge_battle(QModelIndex index)
{
    if(index.column() != 0)
    {
        index = model->index(index.row(),0);
        opponent_ = index.data().toString();
    }
}

void ChallengeDialog::setChallengeUI()
{
    this->setWindowTitle("挑战大厅");
    QSize picSize(702, 515);
    setAutoFillBackground(true);
    QPixmap back_ground(":/image/pokemon_bg.jpg");
    back_ground = back_ground.scaled(picSize);
    QPalette p(this->palette());
    p.setBrush(QPalette::Window,QBrush(back_ground));
    this->setPalette(p);
    ui->tableView->setStyleSheet("background-color:transparent");

    QPalette p2;
    p2.setColor(QPalette::WindowText, Qt::white);
    QFont ft;
    ft.setPointSize(12);
    ft.setBold(8);
    ui->tableView->setFont(ft);
    ui->tableView->setPalette(p2);

    model = new QSqlQueryModel(this);
    QString sql;

    sql = "select * from tb_player where `online` = 1";

    model->setQuery(sql);

    model->setHeaderData(0, Qt::Horizontal, tr("用户"));
    model->setHeaderData(1, Qt::Horizontal, tr("是否在线"));
    model->setHeaderData(2, Qt::Horizontal, tr("宠物小精灵个数"));
    model->setHeaderData(3, Qt::Horizontal, tr("胜场数"));
    model->setHeaderData(4, Qt::Horizontal, tr("输场数"));
    model->setHeaderData(5, Qt::Horizontal, tr("金宠物数勋章"));
    model->setHeaderData(6, Qt::Horizontal, tr("银宠物数勋章"));
    model->setHeaderData(7, Qt::Horizontal, tr("铜宠物数勋章"));
    model->setHeaderData(8, Qt::Horizontal, tr("金高级宠物勋章"));
    model->setHeaderData(9, Qt::Horizontal, tr("银高级宠物勋章"));
    model->setHeaderData(10, Qt::Horizontal, tr("铜高级宠物勋章"));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(1);
    ui->tableView->show();
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

ChallengeDialog::~ChallengeDialog()
{
    delete ui;
}
