#include "coursemanage.h"
#include "ui_coursemanage.h"
#include "global.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

CourseManage::CourseManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseManage)
{
    ui->setupUi(this);
    setWindowTitle("课程管理");
    showAllCourses();
}

CourseManage::~CourseManage()
{
    delete ui;
}

void CourseManage::showAllCourses()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT cId AS '课程号', name AS '课程名', "
                    "teacher AS '任课教师', credit AS '学分' "
                    "FROM course");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

QString CourseManage::getCourseInfo(const QString &title, const QString &label, const QString &defaultValue)
{
    bool ok;
    QString text = QInputDialog::getText(this, title, label, QLineEdit::Normal, defaultValue, &ok);
    return ok ? text : QString();
}

void CourseManage::on_btnQuery_clicked()
{
    QString cid = ui->lineEditId->text();
    if (cid.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入课程号！");
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(QString("SELECT cId AS '课程号', name AS '课程名', "
                            "teacher AS '任课教师', credit AS '学分' "
                            "FROM course WHERE cId = '%1'").arg(cid));

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "提示", "未找到该课程！");
        delete model;
        return;
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void CourseManage::on_btnShowAll_clicked()
{
    showAllCourses();
}

void CourseManage::on_btnAdd_clicked()
{
    QString cid = getCourseInfo("添加课程", "课程号：");
    if (cid.isEmpty()) return;

    // 检查课程号是否已存在
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM course WHERE cId = ?");
    checkQuery.addBindValue(cid);
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "错误", "该课程号已存在，请使用其他课程号！");
        return;
    }

    QString name = getCourseInfo("添加课程", "课程名：");
    if (name.isEmpty()) return;

    QString teacher = getCourseInfo("添加课程", "任课教师：");
    if (teacher.isEmpty()) return;

    bool ok;
    int credit = QInputDialog::getInt(this, "添加课程", "学分：", 3, 1, 10, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("INSERT INTO course (cId, name, teacher, credit) VALUES (?, ?, ?, ?)");
    query.addBindValue(cid);
    query.addBindValue(name);
    query.addBindValue(teacher);
    query.addBindValue(credit);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "课程添加成功！");
        showAllCourses();
    } else {
        Global::showDbError("添加课程", query.lastError());
    }
}

void CourseManage::on_btnEdit_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要修改的课程！");
        return;
    }

    QString cid = index.sibling(index.row(), 0).data().toString();
    QString oldName = index.sibling(index.row(), 1).data().toString();
    QString oldTeacher = index.sibling(index.row(), 2).data().toString();
    int oldCredit = index.sibling(index.row(), 3).data().toInt();

    QString name = getCourseInfo("修改课程", "课程名：", oldName);
    if (name.isEmpty()) return;

    QString teacher = getCourseInfo("修改课程", "任课教师：", oldTeacher);
    if (teacher.isEmpty()) return;

    bool ok;
    int credit = QInputDialog::getInt(this, "修改课程", "学分：", oldCredit, 1, 10, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("UPDATE course SET name = ?, teacher = ?, credit = ? WHERE cId = ?");
    query.addBindValue(name);
    query.addBindValue(teacher);
    query.addBindValue(credit);
    query.addBindValue(cid);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "课程修改成功！");
        showAllCourses();
    } else {
        Global::showDbError("修改课程", query.lastError());
    }
}

void CourseManage::on_btnDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的课程！");
        return;
    }

    QString cid = index.sibling(index.row(), 0).data().toString();
    QString name = index.sibling(index.row(), 1).data().toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除课程 %1（%2）吗？\n注意：删除课程将同时删除相关成绩记录！").arg(name).arg(cid),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    //先删除相关成绩
    QSqlQuery queryScore;
    queryScore.prepare("DELETE FROM score WHERE cId = ?");
    queryScore.addBindValue(cid);
    queryScore.exec();

    // 再删除课程
    QSqlQuery query;
    query.prepare("DELETE FROM course WHERE cId = ?");
    query.addBindValue(cid);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "课程删除成功！");
        showAllCourses();
    } else {
        Global::showDbError("删除课程", query.lastError());
    }
}

void CourseManage::on_btnBack_clicked()
{
    this->close();
}

void CourseManage::on_btnImport_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "导入课程数据", QDir::homePath(),
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
        if (fields.size() < 4) {
            failCount++;
            continue;
        }

        QString cId     = fields[0].trimmed();
        QString name    = fields[1].trimmed();
        QString teacher = fields[2].trimmed();
        int credit      = fields[3].trimmed().toInt();

        // 检查课程号是否已存在
        QSqlQuery check;
        check.prepare("SELECT COUNT(*) FROM course WHERE cId = ?");
        check.addBindValue(cId);
        check.exec();
        check.next();

        if (check.value(0).toInt() > 0) {
            skipCount++;
            continue;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO course (cId, name, teacher, credit) VALUES (?, ?, ?, ?)");
        query.addBindValue(cId);
        query.addBindValue(name);
        query.addBindValue(teacher);
        query.addBindValue(credit);

        if (query.exec()) successCount++;
        else failCount++;
    }

    QSqlDatabase::database().commit();
    file.close();

    showAllCourses();
    QMessageBox::information(this, "导入完成",
                             QString("导入结果：\n成功：%1 条\n跳过（已存在）：%2 条\n失败：%3 条")
                                 .arg(successCount).arg(skipCount).arg(failCount));
}
