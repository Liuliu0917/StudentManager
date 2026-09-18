#include "logindialog.h"
#include "ui_logindialog.h"
#include "global.h"
#include <QMessageBox>
#include <QSqlQuery>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("登录 - 学生管理系统");
    // 不需要手动 connect，Qt 会自动连接 on_btnLogin_clicked
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空！");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT role FROM admin WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (query.exec() && query.next()) {
        QString role = query.value(0).toString();

        //根据角色设置全局权限
        if (role == "admin") {
            Global::currentUserRole = "admin";
            QMessageBox::information(this, "成功", "管理员登录成功！\n拥有全部权限");
        } else {
            Global::currentUserRole = "student";
            QMessageBox::information(this, "成功", "学生登录成功！\n仅可查询成绩");
        }

        Global::currentUsername = username;
        accept();
    } else {
        QMessageBox::warning(this, "错误", "用户名或密码错误！");
    }
}

void LoginDialog::on_btnCancel_clicked()
{
    reject();
}