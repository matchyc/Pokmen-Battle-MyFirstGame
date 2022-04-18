#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QObject>
#include <QWidget>
#include "pokemon.h"
enum{SIGN_UP, SIGN_IN, FIGHT_RESULT, UPDATE_PKM, GET_OPPONENT, DISCARD_PKM, MEDAL_UPDATE};

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QWidget* parent = nullptr);

    void on_read_data() const;
    void deal_with_signUp(QString account, QString password);
    void my_send_data(QString);
    void deal_with_signIn(QString account, QString password);
    void deal_win_lose_count(bool win);
    void deal_update_pokemon(Pokemon *pkm);
    void deal_get_opponent(Pokemon *pkm);
    void deal_discard_pokemon(int no);
    void deal_medal_update(QList<int> medal_list);

public:
    QWidget* fa;
    QTcpSocket *tcpClient;
//    QString serverHost = "127.0.0.1";
    QString serverHost = "129.211.84.160";
    quint16 port = 14444;

signals:
    void sign_in_success() const;
};

#endif // CLIENT_H
