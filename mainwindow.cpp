#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "studentmanage.h"
#include "coursemanage.h"
#include "scoremanage.h"
#include "global.h"
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("学生管理系统 - 主界面");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    //根据权限控制按钮可用性
    if (Global::currentUserRole == "student") {
        // 学生：只能查询成绩，不能管理学生/课程
        ui->btnStudent->setEnabled(false);
        ui->btnStudent->setText("学生管理（无权限）");
        ui->btnCourse->setEnabled(false);
        ui->btnCourse->setText("课程管理（无权限）");
        ui->btnScore->setText("我的成绩");

        QMessageBox::information(this, "权限提示",
                                 "您以学生身份登录，仅可查询成绩！");
    } else {
        // 管理员：全部权限
        ui->btnStudent->setEnabled(true);
        ui->btnCourse->setEnabled(true);
    }
}

void MainWindow::on_btnStudent_clicked()
{
    StudentManage *win = new StudentManage(this);
    win->show();
}

void MainWindow::on_btnCourse_clicked()
{
    CourseManage *win = new CourseManage(this);
    win->show();
}

void MainWindow::on_btnScore_clicked()
{
    ScoreManage *win = new ScoreManage(this);
    win->show();
}

void MainWindow::on_btnExit_clicked()
{
    QApplication::quit();
}