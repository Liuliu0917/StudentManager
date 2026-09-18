#ifndef SCOREMANAGE_H
#define SCOREMANAGE_H

#include <QWidget>

namespace Ui {
class ScoreManage;
}

class ScoreManage : public QWidget
{
    Q_OBJECT

public:
    explicit ScoreManage(QWidget *parent = nullptr);
    ~ScoreManage();

private slots:
    void on_btnQuery_clicked();
    void on_btnStat_clicked();
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_btnExport_clicked();
    void on_btnImport_clicked();

private:
    Ui::ScoreManage *ui;
    void showAllScores();
    void showStudentScores(const QString &sid);
};

#endif // SCOREMANAGE_H