#include "pokemon.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QString>

//#pragma execution_character_set("utf-8")

//ELECTRIC_TYPE, FIRE_TYPE, GRASS_TYPE, WATER_TYPE
double property_map_table[4][4] = {{1,1,0.9,1.5},
                                   {1,1,1.5,0.9},
                                   {1.5,0.9,1,1},
                                   {0.9,1.5,1,1}};
/*
template<class T>
void attack(T *me, T *enemy)
{
    AttackData* atk_ptr = new AttackData;
    if(calc_random_probability(EXPECTED_SKILL_USE))
    {
        atk_ptr->attack_type_ = SKILL_ATTACK;
        //计算技能攻击的伤害
        atk_ptr->damage_ = me->attack_power_ + 5;
        atk_ptr->attack_attribute_ = me->type_;
        atk_ptr->skill_ = me->skill_name_;
        qDebug() <<QString("%1 带有着属性攻击，并且使用技能 %2").arg(me->name_).arg(me->skill_name_);
        if(calc_random_probability(EXPECTED_IMPACTED))
        {
            atk_ptr->impacted_ = true;
        }
    }
    else
    {
        atk_ptr->attack_type_ = NORMAL_ATTACK;
        atk_ptr->damage_ = me->attack_power_;
        atk_ptr->attack_attribute_ = me->type_;
        atk_ptr->impacted_ = false;
        atk_ptr->skill_ = "";
        qDebug() << QString("%1 使用了普通攻击").arg(me->name_);
    }
    enemy->hurted(atk_ptr);
}

template <class T1, class T2>
void hurted(T1 *me, T2 atk_data)
{
    bool is_evade = calc_random_probability(EXPECTED_EVADE);
    if(!is_evade) //evade failed
    {
        int power = 0;
        if(me->atk_data->attack_type_ == SKILL_ATTACK)
        {
            power = atk_data->damage_ * property_map_table[atk_data->attack_attribute_][me->type_];

            if(atk_data->impacted_ == true)
            {
                me->is_impacted_ = true;
            }
        }
        else
        {
            power = atk_data->damage_;
            bool is_critical = calc_random_probability(EXPECTED_CRITICAL);
            if(is_critical)
            {
                power = static_cast<int>(power * 2);
            }
        }
        me->hp_in_battle_ -= power;
    }
    else
    {
        //evade successfully
    }
}
*/


Pokemon::Pokemon()
{

}

StrengthPokemon::StrengthPokemon():Pokemon ()
{
    kind_ = STRENGTH_KIND;
}

DexterityPokemon::DexterityPokemon():Pokemon ()
{
    kind_ = DEXTERITY_KIND;
}

TankPokemon::TankPokemon():Pokemon ()
{
    kind_ = TANK_KIND;
}

DefensePokemon::DefensePokemon():Pokemon ()
{
    kind_ = DEFENSE_KIND;
}


//to do: id_ is not the no in mysql!!!!!!!!!!!!

Pikachu::Pikachu():DexterityPokemon ()
{
    id_ = PIKACHU;
}

Charmander::Charmander():StrengthPokemon ()
{
    id_ = CHARMANDER;
}

Bulbasaur::Bulbasaur():TankPokemon ()
{
    id_ = BULBASAUR;
}

Squirtle::Squirtle():DefensePokemon ()
{
    id_ = SQUIRTLE;
}



void Pokemon::get_exp(bool win)
{
    if(win)
    {
        this->exp_ += 9 * level_;//只是一个经验值增加规则
    }
    else
    {
        this->exp_ += 3 * level_;
    }
    //加完经验值检查升级
    check_level_up();
}

void Pokemon::check_level_up()
{
    qDebug() << "进入是否能够升级的检查";
    if(this->exp_ >= this->level_ * 10 * (this->level_ + 5))//just a rule
    {
        if(this->level_ < 15)
        go_level_up();
    }
}

void Pokemon::go_level_up()
{
    ++this->level_;
    this->exp_ = 0; //clear
    this->attack_power_ += 4;
    this->defense_ += 2;
    this->hp_ += 30;
    this->mp_ += 0;
    this->attack_interval_ -= 20;
    kind_level_up();
    qDebug() << name_ << "成功升级" << "现在等级" << this->level_;
}

void StrengthPokemon::kind_level_up()
{
    this->attack_power_ += 2;
}

void DexterityPokemon::kind_level_up()
{
    this->attack_interval_ -= 10;
}

void TankPokemon::kind_level_up()
{
    this->hp_ += 8;
}

void DefensePokemon::kind_level_up()
{
    this->defense_ += 5;
}

void Pokemon::batlle_of_two(Pokemon *player1, Pokemon *player2)
{
    int p1_f = 0;
    int p2_f = 0;

    while(player1->hp_in_battle_ > 0 && player2->hp_in_battle_ > 0)
    {
        ++p1_f;
        ++p2_f;

        if(p1_f == player1->attack_interval_)
        {
            qDebug() << "p1 hit p2";
            player1->attack(player2);
            qDebug() << QString("finish p1 left %1 hp, p2 left %2 hp").arg(player1->hp_in_battle_).arg(player2->hp_in_battle_);
            p1_f = 0;

        }
        if(p2_f == player2->attack_interval_)
        {
            qDebug() << "p2 hit p1";
            player2->attack(player1);
            QString out = QString("finish p1 left %1 hp, p2 left %2 hp").arg(player1->hp_in_battle_).arg(player2->hp_in_battle_);
            qDebug() << out;
            p2_f = 0;
        }
    }

    Pokemon *winner = player1->hp_in_battle_ > 0?   player1 : player2;
    Pokemon *loser = player1->hp_in_battle_ > 0? player2 : player1;
    qDebug() << "Winner:" << winner->name_;


    //后续题目战斗完后的其他处理
    player1->hp_in_battle_ = player1->hp_;
    player2->hp_in_battle_ = player2->hp_;
    player1->mp_ = player1->mp_;
    player2->mp_ = player2->mp_;
    winner->get_exp(true);
    loser->get_exp(false);

}

void Pikachu::attack(Pokemon *enemy)
{
    AttackData* atk_ptr = new AttackData;
    QString str;//用于传递战斗信息
    //首先判断是否被击晕
    if(this->is_impacted_ == true)
    {
        str = QString("%1被击晕了！不能进行攻击！").arg(this->name_);
        this->is_impacted_ = false;
        emit send_battle_info(str);
    }
    else
    {
        //判断概率是否能够使用技能
        if(calc_random_probability(EXPECTED_SKILL_USE) && this->mp_in_battle_ > 0)
        {
            atk_ptr->attack_type_ = SKILL_ATTACK;
            //计算技能攻击的伤害
            atk_ptr->damage_ = this->attack_power_ + 5;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->skill_ = this->skill_name_;
            str = QString("%1使用了带有着属性的攻击，并且使用技能%2").arg(name_).arg(skill_name_);

            qDebug() <<QString("%1 带有着属性攻击，并且使用技能 %2").arg(name_).arg(skill_name_);
            if(calc_random_probability(EXPECTED_IMPACTED))
            {
                atk_ptr->impacted_ = true;
            }
            --this->mp_in_battle_;
        }
        else
        {
            //普通攻击
            atk_ptr->attack_type_ = NORMAL_ATTACK;
            atk_ptr->damage_ = this->attack_power_;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->impacted_ = false;
            atk_ptr->skill_ = "";

            str = QString("%1 使用了普通攻击").arg(name_);
            qDebug() << QString("%1 使用了普通攻击").arg(name_);

        }
        emit send_battle_info(str);
        enemy->hurted(atk_ptr);
    }
}

void Charmander::attack(Pokemon *enemy)
{
    qDebug() << "攻击力" << this->attack_power_;
    AttackData* atk_ptr = new AttackData;
    QString str;
    if(this->is_impacted_ == true)
    {
        str = QString("%1被击晕了！不能进行攻击！").arg(this->name_);
        this->is_impacted_ = false;
        emit send_battle_info(str);
    }
    else
    {
        if(calc_random_probability(EXPECTED_SKILL_USE) && this->mp_in_battle_ > 0)
        {
            atk_ptr->attack_type_ = SKILL_ATTACK;
            //计算技能攻击的伤害
            atk_ptr->damage_ = this->attack_power_ + 5;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->skill_ = this->skill_name_;
            str = QString("%1使用了带有着属性的攻击，并且使用技能%2").arg(name_).arg(skill_name_);

            qDebug() <<QString("%1 带有着属性攻击，并且使用技能 %2").arg(name_).arg(skill_name_);
            if(calc_random_probability(EXPECTED_IMPACTED))
            {
                atk_ptr->impacted_ = true;
            }
            --this->mp_in_battle_;
        }
        else
        {
            atk_ptr->attack_type_ = NORMAL_ATTACK;
            atk_ptr->damage_ = this->attack_power_;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->impacted_ = false;
            atk_ptr->skill_ = "";

            str = QString("%1 使用了普通攻击").arg(name_);
            qDebug() << QString("%1 使用了普通攻击").arg(name_);
        }
        emit send_battle_info(str);
        enemy->hurted(atk_ptr);
    }
}

void Bulbasaur::attack(Pokemon *enemy)
{
    AttackData* atk_ptr = new AttackData;
    QString str;
    if(this->is_impacted_ == true)
    {
        str = QString("%1被击晕了！不能进行攻击！").arg(this->name_);
        this->is_impacted_ = false;
        emit send_battle_info(str);
    }
    else
    {
        if(calc_random_probability(EXPECTED_SKILL_USE) && this->mp_in_battle_ > 0)
        {
            atk_ptr->attack_type_ = SKILL_ATTACK;
            //计算技能攻击的伤害
            atk_ptr->damage_ = this->attack_power_ + 5;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->skill_ = this->skill_name_;
            str = QString("%1使用了带有着属性的攻击，并且使用技能%2").arg(name_).arg(skill_name_);

            qDebug() <<QString("%1 带有着属性攻击，并且使用技能 %2").arg(name_).arg(skill_name_);
            if(calc_random_probability(EXPECTED_IMPACTED))
            {
                atk_ptr->impacted_ = true;
            }
            --this->mp_in_battle_;
        }
        else
        {
            atk_ptr->attack_type_ = NORMAL_ATTACK;
            atk_ptr->damage_ = this->attack_power_;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->impacted_ = false;
            atk_ptr->skill_ = "";

            str = QString("%1 使用了普通攻击").arg(name_);
            qDebug() << QString("%1 使用了普通攻击").arg(name_);
        }
        emit send_battle_info(str);
        enemy->hurted(atk_ptr);
    }
}

void Squirtle::attack(Pokemon *enemy)
{
    AttackData* atk_ptr = new AttackData;
    QString str;
    if(this->is_impacted_ == true)
    {
        str = QString("%1被击晕了！不能进行攻击！").arg(this->name_);
        this->is_impacted_ = false;
        emit send_battle_info(str);
    }
    else
    {
        if(calc_random_probability(EXPECTED_SKILL_USE) && this->mp_in_battle_ > 0)
        {
            atk_ptr->attack_type_ = SKILL_ATTACK;
            //计算技能攻击的伤害
            atk_ptr->damage_ = this->attack_power_ + 5;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->skill_ = this->skill_name_;
            str = QString("%1使用了带有着属性的攻击，并且使用技能%2").arg(name_).arg(skill_name_);

            qDebug() <<QString("%1 带有着属性攻击，并且使用技能 %2").arg(name_).arg(skill_name_);
            if(calc_random_probability(EXPECTED_IMPACTED))
            {
                atk_ptr->impacted_ = true;
            }
            --this->mp_in_battle_;
        }
        else
        {
            atk_ptr->attack_type_ = NORMAL_ATTACK;
            atk_ptr->damage_ = this->attack_power_;
            atk_ptr->attack_attribute_ = this->type_;
            atk_ptr->impacted_ = false;
            atk_ptr->skill_ = "";
            str = QString("%1 使用了普通攻击").arg(name_);
            qDebug() << QString("%1 使用了普通攻击").arg(name_);
        }
        emit send_battle_info(str);
        enemy->hurted(atk_ptr);
    }
}

void Pikachu::hurted(AttackData *atk_data)
{
    QString str;//战斗信息
    //判断是否闪避
    bool is_evade = calc_random_probability(EXPECTED_EVADE);
    if(!is_evade) //evade failed
    {
        int power = 0;
        if(atk_data->attack_type_ == SKILL_ATTACK)
        {
            power =  static_cast<int>(atk_data->damage_ *
                   (property_map_table[atk_data->attack_attribute_][this->type_]));

            if(atk_data->impacted_ == true)
            {
                this->is_impacted_ = true;
            }
        }
        else
        {
            power = atk_data->damage_;
            bool is_critical = calc_random_probability(EXPECTED_CRITICAL);
            if(is_critical)
            {
                power = static_cast<int>(power * 1.5);
            }
         }
        power = power - static_cast<int>(defense_ * 0.3);
        if(power < 0)
            power = 0;
        this->hp_in_battle_ -= power;
        str = QString("%1受到了%2 的伤害").arg(name_).arg(power);

        qDebug() << QString("%1 受到了 %2 的伤害").arg(name_).arg(power);
    }
    else//躲避成功
    {
        //evade successfully
        str = QString("%1躲开了！").arg(name_);
        qDebug() << QString("%1躲开了！").arg(name_);
    }
    emit send_battle_info(str);
}



void Charmander::hurted(AttackData *atk_data)
{
    QString str;
    bool is_evade = calc_random_probability(EXPECTED_EVADE);
    if(!is_evade) //evade failed
    {
        int power = 0;
        if(atk_data->attack_type_ == SKILL_ATTACK)
        {
            power =  static_cast<int>(atk_data->damage_ *
                   (property_map_table[atk_data->attack_attribute_][this->type_]));

            if(atk_data->impacted_ == true)
            {
                this->is_impacted_ = true;
            }
        }
        else
        {
            power = atk_data->damage_;
            bool is_critical = calc_random_probability(EXPECTED_CRITICAL);
            if(is_critical)
            {
                power = static_cast<int>(power * 1.5);
            }
        }
        power = power - static_cast<int>(defense_ * 0.3);
        if(power < 0)
            power = 0;
        this->hp_in_battle_ -= power;
        str = QString("%1受到了%2 的伤害").arg(name_).arg(power);
        qDebug() << QString("%1 受到了 %2 的伤害").arg(name_).arg(power);
    }
    else
    {
        //evade successfully
        str = QString("%1躲开了！").arg(name_);
        qDebug() << "小火龙躲开了";
    }
    emit send_battle_info(str);
}

void Bulbasaur::hurted(AttackData *atk_data)
{
    QString str;
    bool is_evade = calc_random_probability(EXPECTED_EVADE);
    if(!is_evade) //evade failed
    {
        int power = 0;
        if(atk_data->attack_type_ == SKILL_ATTACK)
        {
            power =  static_cast<int>(atk_data->damage_ *
                   (property_map_table[atk_data->attack_attribute_][this->type_]));

            if(atk_data->impacted_ == true)
            {
                this->is_impacted_ = true;
            }
        }
        else
        {
            power = atk_data->damage_;
            bool is_critical = calc_random_probability(EXPECTED_CRITICAL);
            if(is_critical)
            {
                power = static_cast<int>(power * 1.5);
            }
        }
        power = power - static_cast<int>(defense_ * 0.3);
        if(power < 0)
            power = 0;
        this->hp_in_battle_ -= power;
        str = QString("%1受到了%2的伤害").arg(name_).arg(power);
    }
    else
    {
        //evade successfully

        str = QString("%1躲开了！").arg(name_);
    }
    emit send_battle_info(str);
}

void Squirtle::hurted(AttackData *atk_data)
{
    QString str;
    bool is_evade = calc_random_probability(EXPECTED_EVADE);
    if(!is_evade) //evade failed
    {
        int power = 0;
        if(atk_data->attack_type_ == SKILL_ATTACK)
        {
            power =  static_cast<int>(atk_data->damage_ *
                   (property_map_table[atk_data->attack_attribute_][this->type_]));

            if(atk_data->impacted_ == true)
            {
                this->is_impacted_ = true;
            }
        }
        else
        {
            power = atk_data->damage_;
            bool is_critical = calc_random_probability(EXPECTED_CRITICAL);
            if(is_critical)
            {
                power = static_cast<int>(power * 1.5);
            }
        }
        power = power - static_cast<int>(defense_ * 0.3);
        if(power < 0)
            power = 0;
        this->hp_in_battle_ -= power;
        str = QString("%1受到了%2 的伤害").arg(name_).arg(power);

    }
    else
    {
        //evade successfully

        str = QString("%1躲开了！").arg(name_);
    }
    emit send_battle_info(str);
}


//构造函数
Pikachu::Pikachu(QString name, int level, int exp, int attack_power, int defense, int hp,
                 int attack_interval,int mp)
{

    name_ = name;
    level_ = level;
    exp_ = exp;

    attack_power_ = attack_power;
    defense_ = defense;

    hp_ = hp;
    hp_in_battle_ = hp;

    attack_interval_ = attack_interval;

    mp_ = mp;
    mp_in_battle_ = mp;

    type_ = ELECTRIC_TYPE;
    skill_name_ = "十万伏特";

    is_impacted_ = false;
}

Charmander::Charmander(QString name, int level, int exp, int attack_power, int defense, int hp,
                       int attack_interval,int mp)
{

    name_ = name;
    level_ = level;
    exp_ = exp;

    attack_power_ = attack_power;
    defense_ = defense;

    hp_ = hp;
    hp_in_battle_ = hp;

    attack_interval_ = attack_interval;

    mp_ = mp;
    mp_in_battle_ = mp;

    type_ = ELECTRIC_TYPE;
    skill_name_ = "喷射火焰";

    is_impacted_ = false;
}

Bulbasaur::Bulbasaur(QString name, int level, int exp, int attack_power, int defense, int hp,
                     int attack_interval,int mp)
{

    name_ = name;
    level_ = level;
    exp_ = exp;

    attack_power_ = attack_power;
    defense_ = defense;

    hp_ = hp;
    hp_in_battle_ = hp;

    attack_interval_ = attack_interval;

    mp_ = mp;
    mp_in_battle_ = mp;

    type_ = ELECTRIC_TYPE;
    skill_name_ = "飞叶快刀";

    is_impacted_ = false;
}

Squirtle::Squirtle(QString name, int level, int exp, int attack_power, int defense, int hp,
                   int attack_interval,int mp)
{

    name_ = name;
    level_ = level;
    exp_ = exp;

    attack_power_ = attack_power;
    defense_ = defense;

    hp_ = hp;
    hp_in_battle_ = hp;

    attack_interval_ = attack_interval;

    mp_ = mp;
    mp_in_battle_ = mp;

    type_ = ELECTRIC_TYPE;
    skill_name_ = "冰冻之风";

    is_impacted_ = false;
}


bool Pokemon::calc_random_probability(double expected)
{
    int num = QRandomGenerator::global()->bounded(1,10);
    double p = static_cast<int>(num) / 10.0;
    if(p <= expected)
        return true;
    else
        return false;
}

Pokemon::~Pokemon()
{

}


DefensePokemon::~DefensePokemon()
{

}

TankPokemon::~TankPokemon()
{

}

DexterityPokemon::~DexterityPokemon()
{

}

StrengthPokemon::~StrengthPokemon()
{

}
