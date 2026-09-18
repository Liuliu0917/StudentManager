#include "global.h"

namespace Global {
    QSqlDatabase db;
    QString currentUserRole = "";
    QString currentUsername = "";

    bool connectDB() {
        db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName("StudentDB");
        db.setUserName("root");
        db.setPassword("你的数据库密码");  // 改成你的密码

        if (!db.open()) {
            QMessageBox::critical(nullptr, "数据库连接失败",
                                  "错误信息：" + db.lastError().text());
            return false;
        }
        return true;
    }

    void closeDB() {
        if (db.isOpen()) {
            db.close();
        }
    }
}