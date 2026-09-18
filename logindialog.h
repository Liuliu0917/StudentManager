#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_btnLogin_clicked();   // 必须是这个格式：on_按钮名_clicked
    void on_btnCancel_clicked();  // 取消按钮同理

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H