#include "studentwindow.h"      // 包含自定义的学生窗口头文件
#include "ui_studentwindow.h"   // 包含自动生成的UI头文件
#include <QDebug>               // 包含调试输出类

StudentWindow::StudentWindow(QWidget *parent) :
    QMainWindow(parent),        // 调用父类的构造函数
    ui(new Ui::StudentWindow)   // 创建UI对象
{
    ui->setupUi(this);          // 设置UI界面

    this->setWindowTitle("学生面板");   // 设置窗口标题
    this->setFixedSize(400,300);        // 设置窗口固定大小
    this->setAttribute(Qt::WA_DeleteOnClose);  // 设置在关闭时释放内存

    openTable();  // 打开学生信息表格
}

StudentWindow::~StudentWindow()
{
    delete ui;  // 释放UI对象内存
}

void StudentWindow::openTable()
{
    db = QSqlDatabase::addDatabase("QMYSQL");   // 添加MySQL数据库驱动

    db.setHostName(DB_HOSTNAME);        // 设置数据库主机名
    db.setDatabaseName(DB_NAME);        // 设置数据库名
    db.setUserName(DB_USERNAME);        // 设置数据库用户名
    db.setPassword(DB_PASSWORD);        // 设置数据库密码

    if (!db.open())   // 尝试打开数据库连接
    {
        // 如果打开失败，显示错误消息框并返回
        QMessageBox::warning(this, "错误", "打开数据库失败,错误信息\n"
                                               + db.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QSqlQuery query;    // 创建查询对象
    query.prepare("SELECT id, name, class, password FROM student WHERE id=:ID");  // 准备查询语句，根据学生ID查询信息
    query.bindValue(":ID", LoginId);    // 绑定查询参数ID为LoginId
    query.exec();   // 执行查询
    query.first();  // 定位到第一条记录

    // 将查询结果显示到相应的文本框中
    ui->editId->setText(query.value("id").toString());
    ui->editClass->setText(query.value("class").toString());
    ui->editName->setText(query.value("name").toString());
    ui->editPassword->setText(query.value("password").toString());
}

void StudentWindow::on_btnChange_clicked()
{
    QSqlQuery modify;   // 创建更新查询对象
    modify.prepare("UPDATE student SET password=? WHERE id=?");   // 准备更新语句，更新学生密码
    modify.bindValue(0, ui->editPassword->text());   // 绑定密码的新值
    modify.bindValue(1, LoginId);   // 绑定学生ID

    if (modify.exec())  // 执行更新操作
        QMessageBox::information(this, "提示", "密码修改成功");   // 如果更新成功，显示提示框
    else
        QMessageBox::warning(this, "错误", "修改密码失败,错误信息\n"
                                               + modify.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);  // 更新失败，显示错误消息框
}
