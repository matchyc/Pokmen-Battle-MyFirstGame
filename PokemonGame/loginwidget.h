#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class logInWidget;
}

class logInWidget : public QWidget
{
    Q_OBJECT

public:
    explicit logInWidget(QWidget *parent = nullptr);
    ~logInWidget();
    void setMyLogInWidget();//建立界面
    void on_signUpButton_clicked() const;//注册按钮点击
    void deal_with_signin();//处理登录
    void deal_son_signUp(QString, QString);//转发注册界面的信号
signals:
    void trans_signUp(QString,QString);//转发参数信号到主窗口
    void signin_signal(QString, QString);//登录参数信号，转发到Client以便给服务端校验
private:
    Ui::logInWidget *ui;
};

#endif // LOGINWIDGET_H
