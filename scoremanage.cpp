#include "scoremanage.h"
#include "ui_scoremanage.h"
#include "global.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QSet>

ScoreManage::ScoreManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreManage)
{
    ui->setupUi(this);
    setWindowTitle("成绩管理");
    showAllScores();   // 启动时显示所有成绩
}

ScoreManage::~ScoreManage()
{
    delete ui;
}

// 列结构说明（v_student_score 视图）：
// 0学号 | 1姓名 | 2专业 | 3课程名称 | 4任课教师 | 5分数 | 6等级
void ScoreManage::showAllScores()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM v_student_score ORDER BY 学号");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void ScoreManage::showStudentScores(const QString &sid)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    // ★ 改用视图查询，与全部视图列结构完全一致
    model->setQuery(QString("SELECT * FROM v_student_score WHERE 学号 = '%1'").arg(sid));

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "提示", "该学生没有成绩记录！");
        delete model;
        return;
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void ScoreManage::on_btnQuery_clicked()
{
    QString sid = ui->lineEditSid->text();
    if (sid.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入学号！");
        return;
    }
    showStudentScores(sid);
}

void ScoreManage::on_btnStat_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM v_course_stat");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    QMessageBox::information(this, "统计", "已显示各课程统计信息（视图）！");
}

void ScoreManage::on_btnAdd_clicked()
{
    bool ok;

    QString sid = QInputDialog::getText(this, "添加成绩", "学号：", QLineEdit::Normal, "", &ok);
    if (!ok || sid.isEmpty()) return;

    // 验证学生是否存在
    QSqlQuery checkStudent;
    checkStudent.prepare("SELECT name FROM student WHERE sId = ?");
    checkStudent.addBindValue(sid);
    if (!checkStudent.exec() || !checkStudent.next()) {
        QMessageBox::warning(this, "错误", "学号不存在！");
        return;
    }
    QString studentName = checkStudent.value(0).toString();

    // 获取课程列表
    QSqlQuery courseQuery;
    courseQuery.exec("SELECT cId, name FROM course");
    QStringList courseList;
    QMap<QString, QString> courseMap;

    while (courseQuery.next()) {
        QString cId = courseQuery.value(0).toString();
        QString cName = courseQuery.value(1).toString();
        courseList << cName;
        courseMap[cName] = cId;
    }

    if (courseList.isEmpty()) {
        QMessageBox::warning(this, "错误", "没有课程数据！");
        return;
    }

    // 选择课程
    QString courseName = QInputDialog::getItem(this, "添加成绩",
                                               QString("为 %1 选择课程：").arg(studentName),
                                               courseList, 0, false, &ok);
    if (!ok || courseName.isEmpty()) return;

    QString cId = courseMap[courseName];

    // 检查是否已有成绩
    QSqlQuery checkScore;
    checkScore.prepare("SELECT score FROM score WHERE sId = ? AND cId = ?");
    checkScore.addBindValue(sid);
    checkScore.addBindValue(cId);
    if (checkScore.exec() && checkScore.next()) {
        int existingScore = checkScore.value(0).toInt();
        QMessageBox::warning(this, "错误",
                             QString("该学生此课程已有成绩（%1分），请使用修改功能！").arg(existingScore));
        return;
    }

    // 输入分数
    int score = QInputDialog::getInt(this, "添加成绩", "分数：", 60, 0, 100, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("INSERT INTO score (sId, cId, score) VALUES (?, ?, ?)");
    query.addBindValue(sid);
    query.addBindValue(cId);
    query.addBindValue(score);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "成绩添加成功！");
        showStudentScores(sid);
    } else {
        Global::showDbError("添加成绩", query.lastError());
    }
}

void ScoreManage::on_btnEdit_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要修改的成绩！");
        return;
    }

    // ★ 防止在统计视图下误操作（统计视图第0列是"课程号"）
    QString firstHeader = ui->tableView->model()
                              ->headerData(0, Qt::Horizontal).toString();
    if (firstHeader == "课程号") {
        QMessageBox::warning(this, "提示",
                             "当前是统计视图，不支持修改操作！\n请先切换到成绩视图。");
        return;
    }

    // ★ 列索引修正：3=课程名称，5=分数
    QString sid         = index.sibling(index.row(), 0).data().toString();
    QString studentName = index.sibling(index.row(), 1).data().toString();
    QString courseName  = index.sibling(index.row(), 3).data().toString();
    int oldScore        = index.sibling(index.row(), 5).data().toInt();

    bool ok;
    int newScore = QInputDialog::getInt(this, "修改成绩",
                                        QString("%1 的 %2 成绩：").arg(studentName).arg(courseName),
                                        oldScore, 0, 100, 1, &ok);
    if (!ok) return;

    // 获取课程号（★ 加保护：查不到则报错，杜绝"假成功"）
    QSqlQuery courseQuery;
    courseQuery.prepare("SELECT cId FROM course WHERE name = ?");
    courseQuery.addBindValue(courseName);
    if (!courseQuery.exec() || !courseQuery.next()) {
        QMessageBox::warning(this, "错误", "未找到对应课程，修改失败！");
        return;
    }
    QString cId = courseQuery.value(0).toString();

    QSqlQuery query;
    query.prepare("UPDATE score SET score = ? WHERE sId = ? AND cId = ?");
    query.addBindValue(newScore);
    query.addBindValue(sid);
    query.addBindValue(cId);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "成绩修改成功！");
        showStudentScores(sid);
    } else {
        Global::showDbError("修改成绩", query.lastError());
    }
}

void ScoreManage::on_btnDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的成绩！");
        return;
    }

    // ★ 防止在统计视图下误操作（统计视图第0列是"课程号"）
    QString firstHeader = ui->tableView->model()
                              ->headerData(0, Qt::Horizontal).toString();
    if (firstHeader == "课程号") {
        QMessageBox::warning(this, "提示",
                             "当前是统计视图，不支持删除操作！\n请先切换到成绩视图。");
        return;
    }

    // ★ 列索引修正：3=课程名称
    QString sid         = index.sibling(index.row(), 0).data().toString();
    QString studentName = index.sibling(index.row(), 1).data().toString();
    QString courseName  = index.sibling(index.row(), 3).data().toString();

    // 删除前：判断当前是"全部视图"还是"单个学生视图"
    bool isAllView = true;
    QAbstractItemModel *oldModel = ui->tableView->model();
    if (oldModel) {
        QSet<QString> viewSids;
        for (int row = 0; row < oldModel->rowCount(); ++row) {
            viewSids.insert(oldModel->data(oldModel->index(row, 0)).toString());
        }
        // 表格中有多种学号 → 全部视图；只有一种 → 单个学生视图
        isAllView = (viewSids.size() > 1);
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除 %1 的 %2 成绩吗？").arg(studentName).arg(courseName),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    // 获取课程号（★ 加保护：查不到则报错，杜绝"假成功"）
    QSqlQuery courseQuery;
    courseQuery.prepare("SELECT cId FROM course WHERE name = ?");
    courseQuery.addBindValue(courseName);
    if (!courseQuery.exec() || !courseQuery.next()) {
        QMessageBox::warning(this, "错误", "未找到对应课程，删除失败！");
        return;
    }
    QString cId = courseQuery.value(0).toString();

    QSqlQuery query;
    query.prepare("DELETE FROM score WHERE sId = ? AND cId = ?");
    query.addBindValue(sid);
    query.addBindValue(cId);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "成绩删除成功！");

        // 删除后：停留在原视图
        if (isAllView) {
            showAllScores();
        } else {
            // 单个学生视图：若该学生成绩已删光，则回退到全部视图
            QSqlQuery check;
            check.prepare("SELECT COUNT(*) FROM score WHERE sId = ?");
            check.addBindValue(sid);
            check.exec();
            check.next();
            if (check.value(0).toInt() > 0) {
                showStudentScores(sid);
            } else {
                showAllScores();
            }
        }
    } else {
        Global::showDbError("删除成绩", query.lastError());
    }
}

void ScoreManage::on_btnBack_clicked()
{
    this->close();
}

void ScoreManage::on_btnExport_clicked()
{
    QAbstractItemModel *model = ui->tableView->model();
    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "警告", "没有数据可导出！");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this, "导出成绩数据",
        QDir::homePath() + "/scores_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
        "CSV Files (*.csv);;Text Files (*.txt)");

    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法创建文件！");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // 写入表头
    for (int col = 0; col < model->columnCount(); ++col) {
        out << model->headerData(col, Qt::Horizontal).toString();
        if (col < model->columnCount() - 1) out << ",";
    }
    out << "\n";

    // 写入数据
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (data.contains(",")) data = "\"" + data + "\"";
            out << data;
            if (col < model->columnCount() - 1) out << ",";
        }
        out << "\n";
    }

    file.close();

    QMessageBox::information(this, "成功",
                             QString("数据已导出到：\n%1\n\n共导出 %2 条记录").arg(filePath).arg(model->rowCount()));
}

void ScoreManage::on_btnImport_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "导入成绩数据", QDir::homePath(),
        "CSV Files (*.csv);;Text Files (*.txt)");

    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件！");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    if (!in.atEnd()) in.readLine();  // 跳过表头

    QSqlDatabase::database().transaction();

    int successCount = 0, skipCount = 0, failCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList fields = line.split(",");
        if (fields.size() < 3) {
            failCount++;
            continue;
        }

        QString sid = fields[0].trimmed();
        QString cId = fields[1].trimmed();
        int score   = fields[2].trimmed().toInt();

        // 验证学生和课程是否存在
        QSqlQuery checkStu;
        checkStu.prepare("SELECT COUNT(*) FROM student WHERE sId = ?");
        checkStu.addBindValue(sid);
        checkStu.exec();
        checkStu.next();

        QSqlQuery checkCourse;
        checkCourse.prepare("SELECT COUNT(*) FROM course WHERE cId = ?");
        checkCourse.addBindValue(cId);
        checkCourse.exec();
        checkCourse.next();

        if (checkStu.value(0).toInt() == 0 || checkCourse.value(0).toInt() == 0) {
            failCount++;  // 学号或课程号不存在
            continue;
        }

        // 检查成绩是否已存在
        QSqlQuery checkScore;
        checkScore.prepare("SELECT COUNT(*) FROM score WHERE sId = ? AND cId = ?");
        checkScore.addBindValue(sid);
        checkScore.addBindValue(cId);
        checkScore.exec();
        checkScore.next();

        if (checkScore.value(0).toInt() > 0) {
            skipCount++;
            continue;
        }

        // 验证分数范围
        if (score < 0 || score > 100) {
            failCount++;
            continue;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO score (sId, cId, score) VALUES (?, ?, ?)");
        query.addBindValue(sid);
        query.addBindValue(cId);
        query.addBindValue(score);

        if (query.exec()) successCount++;
        else failCount++;
    }

    QSqlDatabase::database().commit();
    file.close();

    showAllScores();
    QMessageBox::information(this, "导入完成",
                             QString("导入结果：\n成功：%1 条\n跳过（已存在）：%2 条\n失败（数据异常）：%3 条")
                                 .arg(successCount).arg(skipCount).arg(failCount));
}