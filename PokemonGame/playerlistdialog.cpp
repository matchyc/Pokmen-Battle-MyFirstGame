#include "playerlistdialog.h"
#include "ui_playerlistdialog.h"
#include "watchpokemon.h"
#include "mainwindow.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QDialogButtonBox>
#include <QModelIndex>
#include <QVBoxLayout>
#include <QSqlQuery>



PlayerListDialog::PlayerListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerListDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("用户列表");
//    ui->buttonBox->setParent(this);

    //连接数据库
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    db->setHostName("129.211.84.160");
    db->setUserName("wechat");
    db->setPassword("bupt666");
    db->setDatabaseName("pokemon");
    ui->textEdit->setReadOnly(true);
    connect(ui->pushButton, &QPushButton::clicked, this, &PlayerListDialog::close);
    if(db->open() == false)
    {
        qDebug() << "Failed to connect mysql";
//        exit(1);
    }
    else {
        qDebug() << "Connect to mysql successfully";
    }

    ui->comboBox->addItem("全部用户");
    ui->comboBox->addItem("仅查看在线用户");

    setUpPlayerListUI();

    connect(ui->comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
            , this, &PlayerListDialog::setUpPlayerListUI);

    //点击用户名字查看用户所拥有的精灵
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
}

void PlayerListDialog::setUpPlayerListUI()
{
    this->setWindowTitle("用户列表");
    QSize picSize(800, 500);
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

    QSqlQueryModel *model = new QSqlQueryModel(this);
    QString sql;
    if(ui->comboBox->currentIndex() == 0)
    {
        sql = "select * from tb_player";
    }
    else {
        sql = "select * from tb_player where `online` = 1";
    }
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
    ui->tableView->setColumnHidden(1, true);
    ui->tableView->show();

    //query.value能得到值
    MainWindow* fa = reinterpret_cast<MainWindow*>(parentWidget());
    QString sql_2("select count(*) from `"+fa->account_+"`");
    QSqlQuery query;
    query.exec(sql_2);
    qDebug() << sql_2;
    int pkm_num = 0;
    if(query.next())
    {
        pkm_num = query.value(0).toInt();
    }
    else {
        qDebug() << "查询pokemon数量错误";
    }
    sql_2 = QString("update `tb_player` set `pkm_num` = %1 where `id` = '"+fa->account_+"'").arg(pkm_num);
    qDebug() << sql_2;
    query.exec(sql_2);

}

PlayerListDialog::~PlayerListDialog()
{
    delete ui;
    delete db;
}
