#ifndef SELECTDISCARD_H
#define SELECTDISCARD_H

#include <QDialog>
#include <QVector>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlTableModel>
namespace Ui {
class SelectDiscard;
}

class SelectDiscard : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDiscard(QWidget *parent = nullptr);
    void set_plot();//建立界面
    ~SelectDiscard();

    QSqlQueryModel *model;

signals:
    void discard_pkm_no(int);//发送要删除的Pokemon的主键

private:
    Ui::SelectDiscard *ui;
    int decision_;//要删除的Pokemon


};

#endif // SELECTDISCARD_H
