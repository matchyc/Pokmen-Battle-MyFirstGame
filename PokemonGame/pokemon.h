#ifndef POKEMON_H
#define POKEMON_H

#include <QApplication>
#include <QString>
#include <string>
#include <memory>
#include <map>
#include <QObject>

#define MAX_LEVEL 15
#define EXPECTED_SKILL_USE 0.4
#define EXPECTED_IMPACTED 0.1
#define EXPECTED_EVADE 0.1
#define EXPECTED_CRITICAL 0.3

enum{NORMAL_ATTACK = 1, SKILL_ATTACK};

enum{DEXTERITY_KIND, STRENGTH_KIND, TANK_KIND, DEFENSE_KIND};
enum{ELECTRIC_TYPE, FIRE_TYPE, GRASS_TYPE, WATER_TYPE};
enum{PIKACHU, CHARMANDER, BULBASAUR, SQUIRTLE};

extern double property_map_table[4][4];

//攻击携带的信息
typedef struct AttackData
{
public:
    int damage_;//伤害
    int attack_type_;//普通攻击或技能攻击
    int attack_attribute_;//电、水、火、草系攻击
    int impacted_;//是否遭受影响
    QString skill_;//技能名字
}AttackData;

//精灵类
class Pokemon : public QObject
{
    Q_OBJECT
public:
    Pokemon();

    static void batlle_of_two(Pokemon* player1, Pokemon* player2);
//    static void fight(Pokemon* player1, Pokemon* player2);
    virtual void attack(Pokemon* enemy) = 0;//攻击enemy
    virtual void hurted(AttackData* atk_data) = 0;//受到enemy的攻击

    void get_exp(bool win);//比赛完后获得exp
    void go_level_up();
    virtual void kind_level_up() = 0;
    void check_level_up();

    virtual ~Pokemon();
public:
    //基础
    int no_;//玩家的某一个精灵，独特标识。
    int id_; //某个精灵，例如皮卡丘、杰尼龟
    int kind_;//种类，例如，敏捷型、力量型
    QString name_;//名字
    int level_;//等级
    int exp_;//经验
    int attack_power_;//攻击力
    int defense_;//防御力
    int hp_;//生命值
    int attack_interval_;//攻击间隔
    int hp_in_battle_;//战斗时所用的hp
    int mp_in_battle_;//战斗时所用的mp
    //添加
    int mp_;//魔法值
    int type_;//精灵属性
    QString skill_name_;//精灵独有技能
    bool is_impacted_ = false;//是否被影响

    bool calc_random_probability(double);//计算各种可能是否能发生

signals:
    void send_battle_info(QString);

};




//力量型
class StrengthPokemon : public Pokemon
{
    Q_OBJECT
public:
    StrengthPokemon();
    virtual void kind_level_up();
    virtual ~StrengthPokemon();
};

//敏捷型
class DexterityPokemon : public Pokemon
{
    Q_OBJECT
public:
    DexterityPokemon();
    void kind_level_up();
    virtual ~DexterityPokemon();
};

//肉盾型
class TankPokemon : public Pokemon
{
    Q_OBJECT
public:
    TankPokemon();
    void kind_level_up();
    virtual ~TankPokemon();
};

//防御型
class DefensePokemon : public Pokemon
{
    Q_OBJECT
public:
    DefensePokemon();
    void kind_level_up();
    virtual ~DefensePokemon();
};

//皮卡丘 敏捷型
class Pikachu : public DexterityPokemon
{

public:
    Pikachu();
    Pikachu(QString name, int level, int exp, int attack_power, int defense, int hp,
            int attack_interval,int mp);
    void attack(Pokemon* enemy);
    void hurted(AttackData* atk_data);


};


//小火龙 力量型
class Charmander : public StrengthPokemon
{

public:
    Charmander();
    Charmander(QString name, int level, int exp, int attack_power, int defense, int hp,
               int attack_interval,int mp);
    void attack(Pokemon* enemy) override;
    void hurted(AttackData* atk_data) override;
};

//妙蛙种子 肉盾型
class Bulbasaur : public TankPokemon
{

public:
    Bulbasaur();
    Bulbasaur(QString name, int level, int exp, int attack_power, int defense, int hp,
              int attack_interval,int mp);
    void attack(Pokemon* enemy);
    void hurted(AttackData* atk_data);
};

//杰尼龟 防御型
class Squirtle : public DefensePokemon
{

public:
    Squirtle();
    Squirtle(QString name, int level, int exp, int attack_power, int defense, int hp,
             int attack_interval,int mp);
    void attack(Pokemon* enemy);
    void hurted(AttackData* atk_data);
};

bool calc_random_probability(double);

#endif // POKEMON_H
