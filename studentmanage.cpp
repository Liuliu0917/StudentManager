#include "studentmanage.h"
#include "ui_studentmanage.h"
#include "global.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

StudentManage::StudentManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentManage)
{
    ui->setupUi(this);
    setWindowTitle("学生管理");
    showAllStudents();
}

StudentManage::~StudentManage()
{
    delete ui;
}

void StudentManage::showAllStudents()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT sId AS '学号', name AS '姓名', sex AS '性别', "
                    "age AS '年龄', major AS '专业', classId AS '班级' "
                    "FROM student");

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void StudentManage::on_btnQuery_clicked()
{
    QString keyword = ui->lineEditId->text();

    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入查询关键词！");
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);
    // 模糊查询：支持学号、姓名、专业模糊匹配
    model->setQuery(QString(
                        "SELECT sId AS '学号', name AS '姓名', sex AS '性别', "
                        "age AS '年龄', major AS '专业', classId AS '班级' "
                        "FROM student "
                        "WHERE sId LIKE '%%1%' "
                        "   OR name LIKE '%%1%' "
                        "   OR major LIKE '%%1%' "
                        "   OR classId LIKE '%%1%'"
                        ).arg(keyword));

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "提示", "未找到匹配的学生！");
        delete model;
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    QMessageBox::information(this, "查询结果",
        QString("找到 %1 条记录").arg(model->rowCount()));
}

void StudentManage::on_btnShowAll_clicked()
{
    showAllStudents();
}

void StudentManage::on_btnAdd_clicked()
{
    bool ok;

    QString sid = QInputDialog::getText(this, "添加学生", "学号：",
                                        QLineEdit::Normal, "", &ok);
    if (!ok || sid.isEmpty()) return;

    // ★★★ 检查学号是否已存在 ★★★
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM student WHERE sId = ?");
    checkQuery.addBindValue(sid);
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "错误", "该学号已存在，请使用其他学号！");
        return;
    }

    QString name = QInputDialog::getText(this, "添加学生", "姓名：",
                                         QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString sex = QInputDialog::getItem(this, "添加学生", "性别：",
                                        {"男", "女"}, 0, false, &ok);
    if (!ok) return;

    int age = QInputDialog::getInt(this, "添加学生", "年龄：",
                                   18, 15, 30, 1, &ok);
    if (!ok) return;

    QString major = QInputDialog::getText(this, "添加学生", "专业：",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || major.isEmpty()) return;

    QString classId = QInputDialog::getText(this, "添加学生", "班级：",
                                            QLineEdit::Normal, "", &ok);
    if (!ok || classId.isEmpty()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO student (sId, name, sex, age, major, classId) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(sid);
    query.addBindValue(name);
    query.addBindValue(sex);
    query.addBindValue(age);
    query.addBindValue(major);
    query.addBindValue(classId);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "学生添加成功！");
        showAllStudents();
    } else {
        Global::showDbError("添加学生", query.lastError());
    }
}

void StudentManage::on_btnEdit_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要修改的学生！");
        return;
    }

    QString sid = index.sibling(index.row(), 0).data().toString();
    QString oldName = index.sibling(index.row(), 1).data().toString();
    QString oldSex = index.sibling(index.row(), 2).data().toString();
    int oldAge = index.sibling(index.row(), 3).data().toInt();
    QString oldMajor = index.sibling(index.row(), 4).data().toString();
    QString oldClass = index.sibling(index.row(), 5).data().toString();

    bool ok;

    QString name = QInputDialog::getText(this, "修改学生", "姓名：",
                                         QLineEdit::Normal, oldName, &ok);
    if (!ok || name.isEmpty()) return;

    QString sex = QInputDialog::getItem(this, "修改学生", "性别：",
                                        {"男", "女"}, oldSex == "男" ? 0 : 1, false, &ok);
    if (!ok) return;

    int age = QInputDialog::getInt(this, "修改学生", "年龄：", oldAge, 15, 30, 1, &ok);
    if (!ok) return;

    QString major = QInputDialog::getText(this, "修改学生", "专业：",
                                          QLineEdit::Normal, oldMajor, &ok);
    if (!ok || major.isEmpty()) return;

    QString classId = QInputDialog::getText(this, "修改学生", "班级：",
                                            QLineEdit::Normal, oldClass, &ok);
    if (!ok || classId.isEmpty()) return;

    QSqlQuery query;
    query.prepare("UPDATE student SET name = ?, sex = ?, age = ?, major = ?, classId = ? "
                  "WHERE sId = ?");
    query.addBindValue(name);
    query.addBindValue(sex);
    query.addBindValue(age);
    query.addBindValue(major);
    query.addBindValue(classId);
    query.addBindValue(sid);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "学生信息修改成功！");
        showAllStudents();
    } else {
        Global::showDbError("修改学生", query.lastError());
    }
}

void StudentManage::on_btnDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的学生！");
        return;
    }

    QString sid = index.sibling(index.row(), 0).data().toString();
    QString name = index.sibling(index.row(), 1).data().toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除学生 %1（%2）吗？\n注意：删除学生将同时删除其所有成绩！").arg(name).arg(sid),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    // ★★★ 先删除该学生的所有成绩 ★★★
    QSqlQuery deleteScore;
    deleteScore.prepare("DELETE FROM score WHERE sId = ?");
    deleteScore.addBindValue(sid);
    deleteScore.exec();

    // 再删除学生
    QSqlQuery query;
    query.prepare("DELETE FROM student WHERE sId = ?");
    query.addBindValue(sid);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "删除成功！");
        showAllStudents();
    } else {
        Global::showDbError("删除学生", query.lastError());
    }
}

void StudentManage::on_btnBack_clicked()
{
    this->close();
}

void StudentManage::on_btnImport_clicked()
{
    // 1. 选择文件
    QString filePath = QFileDialog::getOpenFileName(
        this, "导入学生数据", QDir::homePath(),
        "CSV Files (*.csv);;Text Files (*.txt)");

    if (filePath.isEmpty()) return;

    // 2. 打开文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件！");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // 3. 跳过表头
    if (!in.atEnd()) in.readLine();

    // 4. 使用事务批量插入
    QSqlDatabase::database().transaction();

    int successCount = 0;
    int skipCount = 0;
    int failCount = 0;
    int lineNum = 1;  // 从第2行数据开始

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        lineNum++;

        // 按逗号分割（注意：简单处理，不支持字段内含逗号的情况）
        QStringList fields = line.split(",");
        if (fields.size() < 6) {
            failCount++;
            continue;
        }

        QString sid   = fields[0].trimmed();
        QString name  = fields[1].trimmed();
        QString sex   = fields[2].trimmed();
        int age       = fields[3].trimmed().toInt();
        QString major = fields[4].trimmed();
        QString classId = fields[5].trimmed();

        // 5. 检查学号是否已存在（已存在则跳过）
        QSqlQuery check;
        check.prepare("SELECT COUNT(*) FROM student WHERE sId = ?");
        check.addBindValue(sid);
        check.exec();
        check.next();

        if (check.value(0).toInt() > 0) {
            skipCount++;
            continue;
        }

        // 6. 插入数据
        QSqlQuery query;
        query.prepare("INSERT INTO student (sId, name, sex, age, major, classId) "
                      "VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(sid);
        query.addBindValue(name);
        query.addBindValue(sex);
        query.addBindValue(age);
        query.addBindValue(major);
        query.addBindValue(classId);

        if (query.exec()) {
            successCount++;
        } else {
            failCount++;
        }
    }

    // 7. 提交事务
    QSqlDatabase::database().commit();
    file.close();

    // 8. 显示结果
    showAllStudents();
    QMessageBox::information(this, "导入完成",
                             QString("导入结果：\n成功：%1 条\n跳过（已存在）：%2 条\n失败：%3 条")
                                 .arg(successCount).arg(skipCount).arg(failCount));
}