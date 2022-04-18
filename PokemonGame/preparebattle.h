#ifndef PREPAREBATTLE_H
#define PREPAREBATTLE_H

#include <QWidget>
#include "pokemon.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>
#include <QList>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class PrepareBattle;
}

class PrepareBattle : public QWidget
{
    Q_OBJECT

public:
    explicit PrepareBattle(bool is_dule, QString opponent = "ai", QWidget *parent = nullptr);

    ~PrepareBattle();
    void setUpPreBattleUI();//建立界面
    void get_pokemon_table();//得到双方的精灵列表
    void get_account(QString);//工具函数，得到账号复制给类成员
    void send_win_count(bool);//发送是否胜利信号
    void send_pokemon_update(Pokemon *my_pkm);//发送更新宝可梦信息函数
    void send_get_opponent(Pokemon *new_pkm);//发送得到对方精灵的函数
    void battle_failed();//战败处理
    void medal_check() ;//奖牌更新处理

    bool is_duel_;//是否是决斗赛
    QString account_;//账号
    QString oppoent_;
signals:
    void win_or_lose(bool);//胜负信号
    void update_pokemon(Pokemon * my_pkm);//更新信号
    void get_opponent(Pokemon* new_pkm);//得到对手
    void discard_pokemon(int);//丢弃宝可梦
    void medal_update(QList<int>);//奖牌更新
    void play_main_music();//处理播放主音乐
private:
    Ui::PrepareBattle *ui;
    QSqlDatabase* db;//数据库
    QSqlQueryModel *model;//我的模型
    QSqlQueryModel *opponent_model;//对手模型
    QMediaPlayer *player;//音乐播放器
    bool click_flag = false;//是否
};

#endif // PREPAREBATTLE_H
