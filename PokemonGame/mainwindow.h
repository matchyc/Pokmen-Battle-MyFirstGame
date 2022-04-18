#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "loginwidget.h"
#include "client.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMyPokemonUI();//建立游戏窗口
    void success_signin();//处理成功登录时
    void deal_checkuser_button();//处理“查询游戏用户”按钮按下
//    void check_user();
    void deal_trainGame_button();//处理升级赛按钮按下
    void deal_duelGame_button();//处理决斗赛按钮按下
    void deal_replay_music() const;//处理背景音乐播放
    void deal_challenge_button() const;//处理挑战按钮
    void challenge_battle(QString opponent);

public:
    logInWidget* log_in_widget;//登录窗口
    Client* myClient_;//客户端类
    QString account_;//该登录者的账号
    QMediaPlayer *player;//音乐播放器

protected:
    void mousePressEvent(QMouseEvent*);//开发测试用函数，确定位置坐标

signals:
    void give_account(QString);//用信号发送账号

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
