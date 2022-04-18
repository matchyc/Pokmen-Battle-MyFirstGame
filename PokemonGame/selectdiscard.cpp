#include "selectdiscard.h"
#include "ui_selectdiscard.h"
#include <QRandomGenerator>
#include <QDebug>
SelectDiscard::SelectDiscard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDiscard)
{
    ui->setupUi(this);

    QSize picSize(632,403);
    setAutoFillBackground(true);
    QPixmap back_ground(":/image/forest.jpg");
    back_ground = back_ground.scaled(picSize);
    QPalette p(this->palette());
    p.setBrush(QPalette::Window,QBrush(back_ground));
    this->setPalette(p);

    ui->buttonBox->setEnabled(false);
    QPixmap pic(":/image/ball.jpg");

    ui->label->setPixmap(pic);
    ui->label->resize(250,250);
    ui->label->setScaledContents(true);
    connect(this, &QDialog::accepted, this, [=](){
        qDebug() << "丢弃" << decision_;
       emit discard_pkm_no(decision_);
    });

}

void SelectDiscard::set_plot()
{
    ui->tableView->setModel(this->model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(1, true);
    ui->tableView->setColumnHidden(2, true);
    connect(ui->tableView, &QTableView::clicked, this,[=](QModelIndex index){
        int row = index.row();
        ui->buttonBox->setEnabled(true);

        index = model->index(row, 1);

        QPixmap pic;
        QModelIndex idx = model->index(row,0);
        decision_ = idx.data().toInt();
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



SelectDiscard::~SelectDiscard()
{
    delete ui;
}
