#include "mainwindow.h"
#include "logindialog.h"
#include "global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 连接数据库
    if (!Global::connectDB()) {
        return -1;
    }

    // 先显示登录对话框
    LoginDialog loginDlg;
    if (loginDlg.exec() != QDialog::Accepted) {
        Global::closeDB();
        return 0;
    }

    // 登录成功后才显示主窗口
    MainWindow w;
    w.show();

    QObject::connect(&a, &QApplication::aboutToQuit, &Global::closeDB);

    return a.exec();
}