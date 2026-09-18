#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Global {
    extern QSqlDatabase db;
    extern QString currentUserRole;
    extern QString currentUsername;

    bool connectDB();
    void closeDB();

    // ★★★ 新增：显示数据库错误 ★★★
    inline void showDbError(const QString &operation, const QSqlError &error) {
        QMessageBox::warning(nullptr, "数据库错误",
            QString("%1失败：\n%2").arg(operation).arg(error.text()));
    }
}

#endif // GLOBAL_H
