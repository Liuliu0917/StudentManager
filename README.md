# StudentManager 学生管理系统

基于 **Qt 6 (Widgets) + MySQL** 的桌面学生管理系统，C++ 编写，使用 CMake 构建。

## 功能

- **登录认证**：管理员 / 学生两种角色登录
- **学生管理**：学生信息的增删改查
- **课程管理**：课程信息的增删改查
- **成绩管理**：学生成绩录入与查询，含统计视图

## 技术栈

- Qt 6.5+（Core / Widgets / Sql 模块）
- MySQL 8.0（通过 QODBC 驱动连接）
- CMake ≥ 3.19

## 目录结构

```
StudentManager/
├── main.cpp / mainwindow.*       # 程序入口与主窗口
├── logindialog.*                 # 登录对话框
├── studentmanage.*               # 学生管理模块
├── coursemanage.*                # 课程管理模块
├── scoremanage.*                 # 成绩管理模块
├── global.*                      # 数据库连接等全局配置
└── database/
    └── student_db.sql            # 数据库建库脚本（含示例数据）
```

## 运行方法

1. **导入数据库**：在 MySQL 中执行建库脚本
   ```bash
   mysql -u root -p < database/student_db.sql
   ```
   脚本会创建 `student_db` 数据库及 `student` / `course` / `score` / `admin` 表和视图，并附带示例数据。

2. **配置 ODBC 数据源**：在 Windows「ODBC 数据源（64 位）」中新建名为 `StudentDB` 的 MySQL ODBC 数据源，指向 `student_db`。

3. **填写数据库密码**：打开 `global.cpp`，将
   ```cpp
   db.setPassword("你的数据库密码");
   ```
   中的占位符改为你本地的 MySQL root 密码。

4. **构建运行**：用 Qt Creator 打开 `CMakeLists.txt`，配置 MinGW 64-bit 套件后构建运行。

## 默认账号

| 角色   | 用户名  | 密码 |
|--------|---------|------|
| 管理员 | admin   | admin123 |
| 学生   | student | 123456 |

> 以上账号见数据库 `admin` 表，请按实际情况修改。

## 数据库说明

- `student`（学生表）、`course`（课程表）、`score`（成绩表）、`admin`（账号表）
- 视图 `v_student_score`（学生成绩联查）、`v_course_stat`（课程成绩统计）
