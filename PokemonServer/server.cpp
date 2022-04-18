#include "server.h"
#include <QList>


Server::Server(QObject *parent) : QObject(parent)
{

    tcpServer = new QTcpServer(this);

    if(!tcpServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Failed to start Listening";
    }
    else
    {
        qDebug() << "Server started";
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::deal_with_connection);
//    connect(tcpSocket, &QTcpSocket::disconnected, this, &Server::deal_with_disconnection);
    //数据库
    db = new MyGameSql;
}

void Server::deal_with_connection()
{
    qDebug() << "有一个连接请求到来";

    tcpSocket = tcpServer->nextPendingConnection();

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Server::on_read_data);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Server::deal_with_disconnection);

}

void Server::on_read_data()
{
    QByteArray buffer;
    QTcpSocket *nowSocket = qobject_cast<QTcpSocket*>(sender());
    //自行在报文后添加'\n'，每次读取一行，解决粘包问题
    while(nowSocket->canReadLine())
    {
        buffer = nowSocket->readLine();
//        qDebug() << buffer;
        QList<QByteArray> recv_list = buffer.split('|');

        int operation = recv_list.front().toInt();
        switch (operation)
       {
        case SIGN_UP:
        {
            qDebug() << "注册请求到来";
            QString account = recv_list[1];
            QString password = recv_list[2];
            sign_up(account, password, nowSocket);
            break;
        }
        case SIGN_IN:
        {
            qDebug() << "登录请求到来";
            QString account = recv_list[1];
            QString password = recv_list[2];
            online_map.insert(nowSocket,account);
            sign_in(account, password, nowSocket);
            break;
        }
        case FIGHT_RESULT:
        {
            qDebug() << "胜场数加还是输场数加请求到来";
            QString account = online_map[nowSocket];
            bool win;
            qDebug() << recv_list[1];
            if(recv_list[1] == "y")
            {
                qDebug() << "胜利";
                win = true;
            }
            else
            {
                qDebug() << "失败";
                win = false;
            }
            win_lose_count(account, win);
            break;
        }
        case UPDATE_PKM:
        {
            qDebug() << "更新宝可梦信息";
            int no = recv_list[1].toInt();
            recv_list.pop_front();
            recv_list.pop_front();
            recv_list.pop_front();
            recv_list.pop_front();
            QString account = online_map[nowSocket];
            pokemon_update(account, no, recv_list);
            break;
        }
        case GET_OPPONENT:
        {
            qDebug() << "获得战胜精灵";
            QString account = online_map[nowSocket];
            recv_list.pop_front();
            get_opponent(account, recv_list);
            break;
        }
        case DISCARD_PKM:
        {
            qDebug() << "战败，丢弃精灵";
            QString account = online_map[nowSocket];
            discard_pokemon(account, recv_list[1].toInt());
            break;
        }
        case MEDAL_UPDATE:
        {
            qDebug() << "处理奖章更新";
            QString account = online_map[nowSocket];
            recv_list.pop_front();
            medal_update(account, recv_list);
            break;
        }

       }
    }

}

void Server::medal_update(QString account, QList<QByteArray> list)
{
    if(db->medal_update(account, list))
    {
        qDebug() << "更新奖牌信息成功";
    }
    else
    {
        qDebug() << "更新奖牌信息失败";
    }
}


void Server::discard_pokemon(QString account, int no)
{
    if(db->discard_pokemon(account, no))
    {
        qDebug() << "战败，丢弃精灵成功";
    }
    else
    {
        qDebug() << "丢弃精灵失败";
    }
}

void Server::get_opponent(QString account, QList<QByteArray> list)
{
    if(db->get_opponent(account, list))
    {
        qDebug() << "战胜，获得对方精灵成功";
    }
    else
    {
        qDebug() << "战胜，但是获得对方精灵失败";
    }
}

void Server::pokemon_update(QString account, int no, QList<QByteArray> list)
{
    if(db->pokemon_update(account, no, list) == true)
    {
        qDebug() << account <<"更新精灵成功";
    }
    else
    {
        qDebug() << account <<"更新精灵失败";
    }
}

void Server::win_lose_count(QString account, bool win)
{
    if(db->win_lose_count(account,win))
    {
        qDebug() << account <<"更新胜场数成功";
    }
    else
    {
        qDebug() << account << "更新胜场数失败";
    }
}

void Server::new_user_pokemon(QString account)
{
    if(db->new_user_pokemon(account))
    {
        qDebug() << "为新用户" << account << "分配3个1级精灵成功";
    }
    else
    {
        qDebug() << "为新用户" << account << "分配精灵失败";
    }
}

void Server::sign_in(QString account, QString password, QTcpSocket *nowSocket)
{
    QString send_buffer;
    if(db->sign_in(account, password))
    {
        send_buffer.append(QString::number(SIGN_IN));
        send_buffer += '|';
        send_buffer.append("y");
    }
    else
    {
        send_buffer.append(QString::number(SIGN_IN));
        send_buffer += '|';
        send_buffer.append("n");
    }
    nowSocket->write(send_buffer.toUtf8());
}

void Server::sign_up(QString account, QString password, QTcpSocket* clientSocket)
{
    QString send_buffer;
    if(db->is_exist(account) == false)
    {
        send_buffer.append(QString::number(SIGN_UP));
        send_buffer += '|';
        send_buffer.append("n");
    }
    else
    {
        if(db->sign_up(account, password) == false)
        {
            send_buffer.append(QString::number(SIGN_UP));
            send_buffer += '|';
            send_buffer.append("n");
        }
        else {
            send_buffer.append(QString::number(SIGN_UP));
            send_buffer += '|';
            send_buffer.append("y");
            new_user_pokemon(account);
        }
    }
    clientSocket->write(send_buffer.toUtf8());
}

void Server::deal_with_disconnection()
{
    QTcpSocket *disSocket = qobject_cast<QTcpSocket*>(sender());
    QString s = disSocket->peerAddress().toString();
    qDebug() << s << "断开连接";
    db->off_line(online_map[disSocket]);
    disSocket->deleteLater();
    disSocket->disconnectFromHost();
    disSocket->close();
}
