#ifndef PLAYERLISTDIALOG_H
#define PLAYERLISTDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
namespace Ui {
class PlayerListDialog;
}

class PlayerListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerListDialog(QWidget *parent = nullptr);
    ~PlayerListDialog();
    void setUpPlayerListUI();//建立UI

private:
    Ui::PlayerListDialog *ui;
    QSqlDatabase * db;
};

#endif // PLAYERLISTDIALOG_H
