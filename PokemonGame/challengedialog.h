#ifndef CHALLENGEDIALOG_H
#define CHALLENGEDIALOG_H
#include "preparebattle.h"
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
namespace Ui {
class ChallengeDialog;
}

class ChallengeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChallengeDialog(QWidget *parent = nullptr);
    ~ChallengeDialog();
    void setChallengeUI();
    void challenge_battle(QModelIndex index);

    QSqlDatabase *db;
    QString opponent_;

signals:
    void give_opponent(QString opponent);

private:
    Ui::ChallengeDialog *ui;
    QSqlQueryModel *model;
    PrepareBattle *battle_widget;

};

#endif // CHALLENGEDIALOG_H
