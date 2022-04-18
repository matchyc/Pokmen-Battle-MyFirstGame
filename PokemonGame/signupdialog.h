#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class signUpDialog;
}

class signUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit signUpDialog(QWidget *parent = nullptr);
    ~signUpDialog();

signals:
    void signUp_signal(QString, QString);

private:
    Ui::signUpDialog *ui;
};

#endif // SIGNUPDIALOG_H
