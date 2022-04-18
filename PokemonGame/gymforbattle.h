#ifndef GYMFORBATTLE_H
#define GYMFORBATTLE_H

#include <QWidget>
#include "pokemon.h"
#include "preparebattle.h"
#include <QStandardItemModel>
#include <QStandardItem>
namespace Ui {
class GymForBattle;
}

class GymForBattle : public QWidget
{
    Q_OBJECT

public:
    explicit GymForBattle(bool is_duel, QWidget *parent = nullptr);
    ~GymForBattle();
    void setUpMyGymUI();//建立UI
    void battle_of_two(Pokemon *player1, Pokemon *player2) const;//双方战斗
    void deal_info_come(QString info) const;//战斗信息到来处理
    void set_battle_field() const;//设置战斗双方
    void fight_complete(int) const;//战斗完成

    bool is_duel_;

public:
    Pokemon *p1;//我方
    Pokemon *p2;//对方
    QStandardItemModel *model;
    PrepareBattle* fa;//指向发窗口，用以发送信号

private:
    Ui::GymForBattle *ui;
};

#endif // GYMFORBATTLE_H
