#ifndef COURSEMANAGE_H
#define COURSEMANAGE_H

#include <QWidget>

namespace Ui {
class CourseManage;
}

class CourseManage : public QWidget
{
    Q_OBJECT

public:
    explicit CourseManage(QWidget *parent = nullptr);
    ~CourseManage();

private slots:
    void on_btnQuery_clicked();
    void on_btnShowAll_clicked();
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_btnImport_clicked();

private:
    Ui::CourseManage *ui;
    void showAllCourses();
    QString getCourseInfo(const QString &title, const QString &label, const QString &defaultValue = "");
};

#endif // COURSEMANAGE_H