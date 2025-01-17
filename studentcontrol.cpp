#include "studentcontrol.h"     // 包含自定义的学生控制窗口头文件
#include "ui_studentcontrol.h"  // 包含自动生成的UI头文件
#include <QMessageBox>          // 包含消息框类
#include <QSqlQuery>            // 包含SQL查询类
#include <QSqlError>            // 包含SQL错误类
#include <QSqlQueryModel>       // 包含SQL查询模型类
#include <QDebug>               // 包含调试输出类

studentControl::studentControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentControl)
{
    ui->setupUi(this);  // 设置UI界面
    Init();             // 初始化函数
}

studentControl::~studentControl()
{
    delete ui;  // 释放UI对象内存
}

void studentControl::Init()
{
    db = QSqlDatabase::addDatabase("QMYSQL");   // 添加MySQL数据库驱动
    db.setHostName(DB_HOSTNAME);                // 设置数据库主机名
    db.setDatabaseName(DB_NAME);                // 设置数据库名
    db.setUserName(DB_USERNAME);                // 设置数据库用户名
    db.setPassword(DB_PASSWORD);                // 设置数据库密码

    if (!db.open()) {   // 尝试打开数据库连接
        QMessageBox::warning(this, "错误", "打开数据库失败,错误信息\n" + db.lastError().text(),
                             QMessageBox::Ok, QMessageBox::NoButton);   // 如果失败，显示错误消息框并返回
        return;
    }

    openTable();    // 打开数据表
}

void studentControl::on_deletebutton_clicked()
{
    QString _cid = ui->input_lesson->text();    // 获取输入的课程号
    if (_cid.isEmpty()) {   // 如果课程号为空，显示错误消息框并返回
        QMessageBox::critical(this, "错误", "未填入课程号", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 检查学生是否存在
    QSqlQuery queryStudent;
    queryStudent.prepare("SELECT id,name,institute,sex,age FROM student WHERE id=:ID");
    queryStudent.bindValue(":ID", LoginId);
    if (!queryStudent.exec() || !queryStudent.first()) {
        QMessageBox::critical(this, "错误", "未找到该学生信息", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 查询是否选了该课程
    QSqlQuery queryCourse;
    queryCourse.prepare("SELECT cid FROM scourse WHERE cid=:cid AND sid=:sid AND ISNULL(grade)=1");
    queryCourse.bindValue(":cid", _cid);
    queryCourse.bindValue(":sid", LoginId);
    if (!queryCourse.exec() || !queryCourse.first()) {
        QMessageBox::critical(this, "错误", "未找到选课记录或填入课程号错误", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 执行退课操作
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM scourse WHERE cid=:cid AND sid=:sid AND ISNULL(grade)=1");
    deleteQuery.bindValue(":cid", _cid);
    deleteQuery.bindValue(":sid", LoginId);
    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "错误", "退课失败", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 刷新界面
    Init();
    QMessageBox::information(this, "提示", "退课成功", QMessageBox::Ok, QMessageBox::NoButton);
}

void studentControl::on_closebutton_clicked()
{
    this->close();  // 关闭当前窗口
}

void studentControl::on_reportButton_clicked()
{
    this->hide();   // 隐藏当前窗口
    report *teacher = new report;   // 创建报告窗口对象
    teacher->show();    // 显示报告窗口
}

void studentControl::on_selectbutton_clicked()
{
    QString _cid = ui->input_lesson->text();    // 获取输入的课程号
    if (_cid.isEmpty()) {   // 如果课程号为空，显示错误消息框并返回
        QMessageBox::critical(this, "错误", "未填入课程号", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 检查学生是否存在
    QSqlQuery queryStudent;
    queryStudent.prepare("SELECT id,name,institute,sex,age FROM student WHERE id=:ID");
    queryStudent.bindValue(":ID", LoginId);
    if (!queryStudent.exec() || !queryStudent.first()) {
        QMessageBox::critical(this, "错误", "未找到该学生信息", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 执行选课操作
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO scourse (cid, sid, grade, credit) VALUES (:cid, :sid, null, null)");
    insertQuery.bindValue(":cid", _cid);
    insertQuery.bindValue(":sid", LoginId);
    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "错误", "选课失败或填入课程号错误", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 刷新界面
    Init();
    QMessageBox::information(this, "提示", "选课成功", QMessageBox::Ok, QMessageBox::NoButton);
}

void studentControl::openTable()
{
    // 显示学生基本信息
    QSqlQuery queryStudentInfo;
    queryStudentInfo.prepare("SELECT id,name,institute,sex,age FROM student WHERE id=:ID");
    queryStudentInfo.bindValue(":ID", LoginId);
    if (!queryStudentInfo.exec() || !queryStudentInfo.first()) {
        QMessageBox::critical(this, "错误", "未找到该学生信息", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    // 将查询结果显示在文本框中
    ui->student_info->clear();
    ui->student_info->append("学号：" + queryStudentInfo.value("id").toString());
    ui->student_info->append("姓名：" + queryStudentInfo.value("name").toString());
    ui->student_info->append("院系：" + queryStudentInfo.value("institute").toString());
    ui->student_info->append("性别：" + queryStudentInfo.value("sex").toString());
    ui->student_info->append("年龄：" + queryStudentInfo.value("age").toString());

    // 查询已修课程
    QSqlQuery queryFinishedCourses;
    queryFinishedCourses.prepare("SELECT s.cid, c.name AS 课程名, c.credit AS 学分, s.grade AS 成绩, s.credit AS 绩点 "
                                 "FROM scourse s "
                                 "JOIN course c ON s.cid = c.id "
                                 "WHERE s.sid=:ID AND s.grade IS NOT NULL");
    queryFinishedCourses.bindValue(":ID", LoginId);
    if (!queryFinishedCourses.exec()) {
        qDebug() << "查询已修课程失败：" << queryFinishedCourses.lastError().text();
        return;
    }

    // 将查询结果显示在表格中
    QSqlQueryModel *finishedModel = new QSqlQueryModel;
    finishedModel->setQuery(queryFinishedCourses);
    ui->finished_lesson->setModel(finishedModel);

    // 查询已选课程
    QSqlQuery querySelectedCourses;
    querySelectedCourses.prepare("SELECT s.cid, c.name AS 课程名, c.credit AS 学分, c.institute AS 学院, c.teacher AS 教师 "
                                 "FROM scourse s "
                                 "JOIN course c ON s.cid = c.id "
                                 "WHERE s.sid=:ID AND s.grade IS NULL");
    querySelectedCourses.bindValue(":ID", LoginId);
    if (!querySelectedCourses.exec()) {
        qDebug() << "查询已选课程失败：" << querySelectedCourses.lastError().text();
        return;
    }

    // 将查询结果显示在表格中
    QSqlQueryModel *selectedModel = new QSqlQueryModel;
    selectedModel->setQuery(querySelectedCourses);
    ui->selected_lesson->setModel(selectedModel);

    // 查询可选课程
    QSqlQuery querySelectableCourses;
    querySelectableCourses.prepare("SELECT id AS 课程号, name AS 课程名, credit AS 学分, institute AS 学院, teacher AS 教师 "
                                   "FROM course "
                                   "WHERE id NOT IN (SELECT cid FROM scourse WHERE sid=:ID)");
    querySelectableCourses.bindValue(":ID", LoginId);
    if (!querySelectableCourses.exec()) {
        qDebug() << "查询可选课程失败：" << querySelectableCourses.lastError().text();
        return;
    }

    // 将查询结果显示在表格中
    QSqlQueryModel *selectableModel = new QSqlQueryModel;
    selectableModel->setQuery(querySelectableCourses);
    ui->canSelect_lesson->setModel(selectableModel);
}
