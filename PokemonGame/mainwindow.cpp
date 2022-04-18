#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pokemon.h"
#include "loginwidget.h"
#include "signupdialog.h"
#include "playerlistdialog.h"
#include "gymforbattle.h"
#include "preparebattle.h"
#include "challengedialog.h"

#include <QtSql/QSqlDatabase>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    log_in_widget = new logInWidget;
    log_in_widget->show();

    myClient_ = new Client(this);

    //连接客户端和各个模块
    connect(log_in_widget, static_cast<void(logInWidget::*)(QString, QString)>(&logInWidget::trans_signUp),
            myClient_, &Client::deal_with_signUp);
    connect(log_in_widget, static_cast<void(logInWidget::*)(QString, QString)>(&logInWidget::signin_signal),
            myClient_, &Client::deal_with_signIn);
    connect(log_in_widget, static_cast<void(logInWidget::*)(QString, QString)>(&logInWidget::signin_signal),
            this,[=](QString account, QString password){this->account_ = account; password = "";});
    connect(myClient_, &Client::sign_in_success, this, &MainWindow::success_signin);

    setMyPokemonUI();

    //各个按钮功能
    connect(ui->checkUserButton, &QPushButton::clicked, this, &MainWindow::deal_checkuser_button);

    connect(ui->trainGameButton, &QPushButton::clicked, this, &MainWindow::deal_trainGame_button);

    connect(ui->duelGameButton, &QPushButton::clicked, this, &MainWindow::deal_duelGame_button);

    connect(ui->challengeButton, &QPushButton::clicked, this, &MainWindow::deal_challenge_button);

//    Pikachu* pp1 = new Pikachu("Pikachu", 1, 0, 12, 6, 100, 700, 10);
//    Charmander* pp2 = new Charmander("Charmander", 1, 0, 15, 8, 100, 900, 10);
//    for(int i = 1; i <= 5; ++i)
//    {
//        qDebug() <<QString("第 %1 局").arg(i);
//        Pokemon::batlle_of_two(pp1,pp2);
//    }

}

void MainWindow::setMyPokemonUI()
{
    this->setWindowTitle("Pokemon!!!");
    QSize picSize(1200,800);
    setAutoFillBackground(true);
    QPixmap back_ground(":/image/Onega_Town.png");
    back_ground = back_ground.scaled(picSize);
    QPalette p(this->palette());
    p.setBrush(QPalette::Window,QBrush(back_ground));
    this->setPalette(p);


}

void MainWindow::deal_challenge_button() const
{
    ChallengeDialog *myDialog = new ChallengeDialog(const_cast<MainWindow*>(this));

    connect(myDialog, &ChallengeDialog::give_opponent, this, &MainWindow::challenge_battle);
    myDialog->setModal(true);
    myDialog->exec();
}

void MainWindow::challenge_battle(QString opponent)
{
    PrepareBattle *battle_widget = new PrepareBattle(false, opponent);
    connect(this, static_cast<void(MainWindow::*)(QString)>(&MainWindow::give_account),
                                   battle_widget, &PrepareBattle::get_account);
    player->pause();

//    qDebug() << account_;
    emit give_account(account_);

    battle_widget->get_pokemon_table();

    battle_widget->show();

    //胜败
    connect(battle_widget, static_cast<void(PrepareBattle::*)(bool)>(&PrepareBattle::win_or_lose),
            myClient_, &Client::deal_win_lose_count);
    //更新精灵信息
    connect(battle_widget, static_cast<void(PrepareBattle::*)(Pokemon*)>(&PrepareBattle::update_pokemon),
            myClient_, &Client::deal_update_pokemon);
    //得到对方精灵
    connect(battle_widget, static_cast<void(PrepareBattle::*)(Pokemon*)>(&PrepareBattle::get_opponent),
            myClient_, &Client::deal_get_opponent);
    //丢弃精灵
    connect(battle_widget, static_cast<void(PrepareBattle::*)(int)>(&PrepareBattle::discard_pokemon),
            myClient_, &Client::deal_discard_pokemon);
    //奖牌更新
    connect(battle_widget, static_cast<void(PrepareBattle::*)(QList<int>)>(&PrepareBattle::medal_update),
            myClient_, &Client::deal_medal_update);
    //继续播放城镇音乐
    connect(battle_widget, &PrepareBattle::play_main_music, this, &MainWindow::deal_replay_music);
}

void MainWindow::deal_duelGame_button()
{

    PrepareBattle *battle_widget = new PrepareBattle(true);//传入的参数表示是否时决斗赛
    connect(this, static_cast<void(MainWindow::*)(QString)>(&MainWindow::give_account),
                                   battle_widget, &PrepareBattle::get_account);

    player->pause();//进入战斗，暂停城镇音乐
//    qDebug() << account_;
    emit give_account(account_);
    //显示双方所拥有的精灵，以便进行选择
    battle_widget->get_pokemon_table();
    battle_widget->show();
    //胜败
    connect(battle_widget, static_cast<void(PrepareBattle::*)(bool)>(&PrepareBattle::win_or_lose),
            myClient_, &Client::deal_win_lose_count);
    //更新精灵信息
    connect(battle_widget, static_cast<void(PrepareBattle::*)(Pokemon*)>(&PrepareBattle::update_pokemon),
            myClient_, &Client::deal_update_pokemon);
    //得到对方精灵
    connect(battle_widget, static_cast<void(PrepareBattle::*)(Pokemon*)>(&PrepareBattle::get_opponent),
            myClient_, &Client::deal_get_opponent);
    //丢弃精灵
    connect(battle_widget, static_cast<void(PrepareBattle::*)(int)>(&PrepareBattle::discard_pokemon),
            myClient_, &Client::deal_discard_pokemon);
    //奖牌更新
    connect(battle_widget, static_cast<void(PrepareBattle::*)(QList<int>)>(&PrepareBattle::medal_update),
            myClient_, &Client::deal_medal_update);
    //继续播放城镇音乐
    connect(battle_widget, &PrepareBattle::play_main_music, this, &MainWindow::deal_replay_music);
}

void MainWindow::deal_trainGame_button()
{
    PrepareBattle *battle_widget = new PrepareBattle(false);
    connect(this, static_cast<void(MainWindow::*)(QString)>(&MainWindow::give_account),
                                   battle_widget, &PrepareBattle::get_account);
    player->pause();

//    qDebug() << account_;
    emit give_account(account_);

    battle_widget->get_pokemon_table();

    battle_widget->show();

    //胜败
    connect(battle_widget, static_cast<void(PrepareBattle::*)(bool)>(&PrepareBattle::win_or_lose),
            myClient_, &Client::deal_win_lose_count);
    //更新精灵信息
    connect(battle_widget, static_cast<void(PrepareBattle::*)(Pokemon*)>(&PrepareBattle::update_pokemon),
            myClient_, &Client::deal_update_pokemon);
    //得到对方精灵
    connect(battle_widget, static_cast<void(PrepareBattle::*)(Pokemon*)>(&PrepareBattle::get_opponent),
            myClient_, &Client::deal_get_opponent);
    //丢弃精灵
    connect(battle_widget, static_cast<void(PrepareBattle::*)(int)>(&PrepareBattle::discard_pokemon),
            myClient_, &Client::deal_discard_pokemon);
    //奖牌更新
    connect(battle_widget, static_cast<void(PrepareBattle::*)(QList<int>)>(&PrepareBattle::medal_update),
            myClient_, &Client::deal_medal_update);
    //继续播放城镇音乐
    connect(battle_widget, &PrepareBattle::play_main_music, this, &MainWindow::deal_replay_music);
}


void MainWindow::success_signin()
{
//    qDebug() << "成功登录";
    log_in_widget->close();

    this->show();

    player = new QMediaPlayer(this);

    QMediaPlaylist *play_list = new QMediaPlaylist(player);
    play_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    play_list->addMedia(QUrl("qrc:/music/main_music.mp3"));
    play_list->setCurrentIndex(1);
    play_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player->setPlaylist(play_list);
    player->play();
}

void MainWindow::deal_replay_music() const
{
//    player->setMedia(QUrl("qrc:/music/main_music.mp3"));
    player->play();
}

void MainWindow::deal_checkuser_button()
{
    PlayerListDialog myDialog(this);

    myDialog.setModal(true);
    myDialog.exec();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    ui->locLabel->setText(QString("位置(%1,%2)").arg(event->x()).arg(event->y()));
}

MainWindow::~MainWindow()
{
    myClient_->tcpClient->disconnectFromHost();
    myClient_->tcpClient->close();
    delete log_in_widget;
    delete ui;
}

