#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShowEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_btnStudent_clicked();
    void on_btnCourse_clicked();
    void on_btnScore_clicked();
    void on_btnExit_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H