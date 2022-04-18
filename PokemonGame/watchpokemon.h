#ifndef WATCHPOKEMON_H
#define WATCHPOKEMON_H

#include <QDialog>
#include <QSqlQueryModel>
namespace Ui {
class WatchPokemon;
}

class WatchPokemon : public QDialog
{
    Q_OBJECT

public:
    explicit WatchPokemon(QWidget *parent = nullptr);
    ~WatchPokemon();
    void setWatchUI() const;

    QSqlQueryModel *model;
private:
    Ui::WatchPokemon *ui;

};

#endif // WATCHPOKEMON_H
