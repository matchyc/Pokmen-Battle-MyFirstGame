#include "signupdialog.h"
#include "client.h"
#include "ui_signupdialog.h"
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>

signUpDialog::signUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signUpDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("注册窗口");

    ui->buttonBox->setEnabled(false);
    connect(ui->password, &QLineEdit::textChanged, this, [=](){
       if(ui->password->text() != ui->againPassword->text())
       {
            ui->buttonBox->setEnabled(false);
       }
       else if(ui->account->text() != "")
       {
            ui->buttonBox->setEnabled(true);
       }
    });

    connect(ui->againPassword, &QLineEdit::textChanged, this, [=](){
       if(ui->password->text() != ui->againPassword->text())
       {
            ui->buttonBox->setEnabled(false);
       }
       else if(ui->account->text() != "")
       {
            ui->buttonBox->setEnabled(true);
       }
    });

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [=](){
        QString account = ui->account->text();
        QString password = ui->againPassword->text();
        qDebug() << "发送注册请求";
        emit signUp_signal(account, password);
    });
}

signUpDialog::~signUpDialog()
{
    delete ui;
}
