#include "client.h"
#include <QMessageBox>
#include <QDebug>
#include <QList>

#include "mainwindow.h"

Client::Client(QWidget* parent)
{
    tcpClient = new QTcpSocket(this);
    tcpClient->connectToHost(serverHost, 14444);
    fa = parent;
    if(!tcpClient->waitForConnected())
    {
        QMessageBox::warning(parent,"warning", tcpClient->errorString());
        return;
    }
    else
    {
        QMessageBox::information(parent, "notice", "连接服务器成功");
    }

    connect(tcpClient, &QTcpSocket::readyRead, this, &Client::on_read_data);
    connect(tcpClient, &QTcpSocket::disconnected, this, [=](){
        tcpClient->disconnectFromHost();
        tcpClient->deleteLater();
        tcpClient->close();
    });
}

void Client::on_read_data() const
{
    QByteArray buffer;
    buffer = tcpClient->readAll();
    QList<QByteArray> recv_list = buffer.split('|');
    int operation = recv_list.front().toInt();
    switch(operation)
   {
    case SIGN_UP:
    {
        qDebug() << "接收到了注册请求返回信息";
        if(recv_list.back() == "y")
        {
            QMessageBox::information(fa, "提示", "注册成功！");
        }
        else {
            QMessageBox::warning(fa, "提示", "注册失败！用户名已存在或网络连接失败");
        }
        break;
    }
    case SIGN_IN:
    {
        qDebug("收到了登录请求返回信息");
        qDebug() << QString(buffer);
        if(recv_list.back() == "y")
        {
            QMessageBox::information(fa, "提示", "登陆成功");
            emit sign_in_success();
        }
        else {
            QMessageBox::warning(fa, "警告", "密码错误或用户名不存在");
        }
        break;
    }
   }
}


void Client::my_send_data(QString data)
{
    data += '\n';
    QByteArray send_buffer(data.toUtf8());

    if(tcpClient->write(send_buffer) == -1)
    {
        QMessageBox::warning(fa, "warning", "发送数据失败，检查网络");
    }
    tcpClient->flush();
}

void Client::deal_discard_pokemon(int no)
{
    qDebug() << "处理战败丢弃一个宝可梦";
    QString discard_info;
    discard_info.append(QString::number(DISCARD_PKM));
    discard_info += '|';
    discard_info.append(QString::number(no));
    discard_info += '|';

    my_send_data(discard_info);
}

void Client::deal_medal_update(QList<int> medal_list)
{
    qDebug() << "处理奖牌更新";
    QString info;
    info.append(QString::number(MEDAL_UPDATE));
    info += '|';
    for(auto i = medal_list.begin(); i != medal_list.end(); ++i)
    {
        info += QString::number(*i);
        info += '|';
    }

    my_send_data(info);
}

void Client::deal_win_lose_count(bool win)
{
    qDebug() << "处理战斗结束，胜场数+还是输场数+";
    QString fight_res;
    fight_res.append(QString::number(FIGHT_RESULT));
    fight_res += '|';
    if(win)
        fight_res += "y";
    else
        fight_res += "n";

    fight_res += '|';
    qDebug() << fight_res;
    my_send_data(fight_res);
}

void Client::deal_update_pokemon(Pokemon *pkm)
{
    qDebug() << "处理更新宝可梦信息请求" << pkm->name_ << "经验" << pkm->exp_;

    QString info;
    info.append(QString::number(UPDATE_PKM));
    info += '|';
    info.append(QString::number(pkm->no_));
    info += '|';
    info += QString::number(pkm->id_);
    info += '|';
    info += QString::number(pkm->kind_);
    info += '|';
    info += pkm->name_;
    info += '|';
    info += QString::number(pkm->level_);
    info += '|';
    info += QString::number(pkm->exp_);
    info += '|';
    info += QString::number(pkm->attack_power_);
    info += '|';
    info += QString::number(pkm->defense_);
    info += '|';
    info += QString::number(pkm->attack_interval_);
    info += '|';
    info += QString::number(pkm->type_);
    info += '|';
    info += pkm->skill_name_;
    info += '|';
    info += QString::number(pkm->hp_);
    info += '|';
    info += QString::number(pkm->mp_);
    info += '|';
    my_send_data(info);
}

void Client::deal_get_opponent(Pokemon *pkm)
{
    qDebug() << "战胜，获得对方精灵";
    QString info;
    info.append(QString::number(GET_OPPONENT));
    info += '|';
    info += QString::number(pkm->id_);
    info += '|';
    info += QString::number(pkm->kind_);
    info += '|';
    info += pkm->name_;
    info += '|';
    info += QString::number(pkm->level_);
    info += '|';
    info += QString::number(pkm->exp_);
    info += '|';
    info += QString::number(pkm->attack_power_);
    info += '|';
    info += QString::number(pkm->defense_);
    info += '|';
    info += QString::number(pkm->attack_interval_);
    info += '|';
    info += QString::number(pkm->type_);
    info += '|';
    info += pkm->skill_name_;
    info += '|';
    info += QString::number(pkm->hp_);
    info += '|';
    info += QString::number(pkm->mp_);
    info += '|';

    my_send_data(info);
}

void Client::deal_with_signIn(QString account, QString password)
{
    qDebug() << "处理登录请求";

    QString user_info;
    user_info.append(QString::number(SIGN_IN));
    user_info.append('|');
    user_info.append(account);
    user_info.append('|');
    user_info.append(password);
    user_info += '|';
    my_send_data(user_info);

}

void Client::deal_with_signUp(QString account, QString password)
{
    qDebug() << account;
    qDebug() << password;
    QString user_info;
    user_info.append(QString::number(SIGN_UP));
    user_info.append('|');
//    user_info.append(SIGN_UP);
    user_info.append(account);
    user_info.append('|');
    user_info.append(password);
    user_info += '|';
    my_send_data(user_info);
}
