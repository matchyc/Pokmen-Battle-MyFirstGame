#include "watchpokemon.h"
#include "ui_watchpokemon.h"

WatchPokemon::WatchPokemon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WatchPokemon)
{
    ui->setupUi(this);

    QSize picSize(700,420);
    setAutoFillBackground(true);
    QPixmap back_ground(":/image/forest.jpg");
    back_ground = back_ground.scaled(picSize);
    QPalette p(this->palette());
    p.setBrush(QPalette::Window,QBrush(back_ground));
    this->setPalette(p);

    QPixmap pic(":/image/ball.jpg");

    ui->label->setPixmap(pic);
    ui->label->resize(250,250);
    ui->label->setScaledContents(true);


}

void WatchPokemon::setWatchUI() const
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    connect(ui->tableView, &QTableView::clicked, this,[=](QModelIndex index){
        int row = index.row();


        index = model->index(row, 0);

        QPixmap pic;

        switch (index.data().toInt())
       {
        case 0:
            pic.load(":/image/pikachu.png");
            break;
        case 1:
            pic.load(":/image/charmander.png");
            break;
        case 2:
            pic.load(":/image/bulbasaur.png");
            break;
        case 3:
            pic.load(":/image/squirtle.png");
            break;
       }
        ui->label->resize(250,250);
        ui->label->setPixmap(pic);
        ui->label->setScaledContents(true);
    });
}

WatchPokemon::~WatchPokemon()
{
    delete ui;
}
