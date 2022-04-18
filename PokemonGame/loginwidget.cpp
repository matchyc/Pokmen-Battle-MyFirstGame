#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "signupdialog.h"
#include "mainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
logInWidget::logInWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logInWidget)
{
    ui->setupUi(this);
    setMyLogInWidget();
    connect(ui->signUpButton, &QPushButton::clicked, this, &logInWidget::on_signUpButton_clicked);
    connect(ui->signInButton, &QPushButton::clicked, this, &logInWidget::deal_with_signin);
}

void logInWidget::deal_with_signin()
{
    emit signin_signal(ui->account->text(), ui->password->text());
}

void logInWidget::on_signUpButton_clicked() const
{
    signUpDialog myDialog(const_cast<logInWidget*>(this));

    connect(&myDialog, static_cast<void(signUpDialog::*)(QString, QString)>(&signUpDialog::signUp_signal),
            this, &logInWidget::deal_son_signUp);

    myDialog.setModal(true);
    myDialog.exec();
}

void logInWidget::deal_son_signUp(QString account, QString password)
{
    qDebug() << "注册请求传到login窗口了";
    emit trans_signUp(account, password);
}

void logInWidget::setMyLogInWidget()
{
    this->setWindowTitle("登录窗口");

    setAutoFillBackground(true);
    QPixmap back_ground(":/image/loginback.jpg");
    QPalette p(this->palette());
    p.setBrush(QPalette::Window,QBrush(back_ground));
    this->setPalette(p);


    QPalette p2;
    p2.setColor(QPalette::WindowText, Qt::white);
    QFont ft;
    ft.setPointSize(12);
    ft.setBold(8);
    ui->label->setFont(ft);
    ui->label_2->setFont(ft);
    ui->label->setPalette(p2);
    ui->label_2->setPalette(p2);
}


logInWidget::~logInWidget()
{
    delete ui;
}
