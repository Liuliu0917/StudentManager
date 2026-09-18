#ifndef STUDENTMANAGE_H
#define STUDENTMANAGE_H

#include <QWidget>

namespace Ui {
class StudentManage;
}

class StudentManage : public QWidget
{
    Q_OBJECT

public:
    explicit StudentManage(QWidget *parent = nullptr);
    ~StudentManage();

private slots:
    void on_btnQuery_clicked();
    void on_btnShowAll_clicked();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_btnEdit_clicked();
    void on_btnImport_clicked();

private:
    Ui::StudentManage *ui;
    void showAllStudents();
};

#endif // STUDENTMANAGE_H