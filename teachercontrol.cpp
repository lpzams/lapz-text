#include "teachercontrol.h" // 包含头文件 teachercontrol.h，声明了 teacherControl 类
#include "ui_teachercontrol.h" // 包含 UI 头文件 ui_teachercontrol.h，定义了 UI 类

teacherControl::teacherControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::teacherControl)
{
    ui->setupUi(this); // 设置 UI，这里会初始化 UI 组件
    Init(); // 调用 Init() 方法进行初始化操作
}

teacherControl::~teacherControl()
{
    delete ui; // 释放 UI 对象的内存
}

void teacherControl::Init()
{
    db = QSqlDatabase::addDatabase("QMYSQL"); // 添加 MySQL 数据库连接

    db.setHostName(DB_HOSTNAME); // 设置数据库主机名
    db.setDatabaseName(DB_NAME); // 设置数据库名
    db.setUserName(DB_USERNAME); // 设置数据库用户名
    db.setPassword(DB_PASSWORD); // 设置数据库密码

    if (!db.open()) { // 如果数据库连接失败
        QMessageBox::warning(this, "错误", "打开数据库失败,错误信息\n" + db.lastError().text(),
                             QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        return; // 返回，不继续执行后面的操作
    }

    QSqlQuery query; // 创建数据库查询对象
    query.prepare("SELECT id,name FROM teacher WHERE id=:ID"); // 准备查询教师信息的 SQL 语句
    query.bindValue(":ID", LoginId); // 绑定查询参数 ID
    query.exec(); // 执行查询
    query.first(); // 移动到第一条记录

    t_name = query.value("name").toString(); // 获取教师姓名
    ui->nameLabel->clear(); // 清空姓名标签
    // 设置姓名标签显示教师工号、姓名和欢迎信息
    ui->nameLabel->setText("工号：" + query.value("id").toString() + " 姓名：" + query.value("name").toString() + "  欢迎您！");

    query.clear(); // 清空查询对象
    // 准备查询教师所教授课程及学生选课信息的 SQL 语句
    query.prepare("SELECT e.cid, c.name AS 课程名, e.sid, s.name AS 学生姓名, e.grade AS 成绩, e.credit AS 绩点 "
                  "FROM scourse e, student s, course c "
                  "WHERE e.sid=s.id AND c.id=e.cid AND c.teacher=:name");
    query.bindValue(":name", t_name); // 绑定教师姓名参数

    if (!query.exec()) { // 如果查询执行失败
        qDebug() << "error"; // 输出错误信息到调试输出
    }

    query.first(); // 移动到查询结果的第一条记录

    QSqlQueryModel *c_model = new QSqlQueryModel; // 创建 QSqlQueryModel 对象，用于显示查询结果
    c_model->setQuery(query); // 将查询结果设置到数据模型中
    c_model->setHeaderData(0, Qt::Horizontal, "课程号"); // 设置表头数据
    c_model->setHeaderData(1, Qt::Horizontal, "课程名");
    c_model->setHeaderData(2, Qt::Horizontal, "学号");
    c_model->setHeaderData(3, Qt::Horizontal, "姓名");
    c_model->setHeaderData(4, Qt::Horizontal, "成绩");
    c_model->setHeaderData(5, Qt::Horizontal, "绩点");

    ui->tableView->setModel(c_model); // 将数据模型设置到表格视图中显示
}

void teacherControl::on_selectbutton_clicked()
{
    if (ui->input_cid->text().isEmpty()) { // 如果课程号输入框为空
        QMessageBox::critical(this, "错误", "未填入课程号\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        Init(); // 调用 Init() 方法重新初始化界面
        return; // 返回，不继续执行后面的操作
    }

    QString _cid = ui->input_cid->text(); // 获取输入的课程号

    QSqlQuery query; // 创建数据库查询对象
    // 准备查询指定课程号下的学生选课信息的 SQL 语句
    query.prepare("SELECT e.cid, c.name AS 课程名, e.sid, s.name AS 学生姓名, e.grade AS 成绩, e.credit AS 绩点 "
                  "FROM scourse e, student s, course c "
                  "WHERE e.sid=s.id AND c.id=e.cid AND c.id=:course AND c.teacher=:name");
    query.bindValue(":course", _cid); // 绑定课程号参数
    query.bindValue(":name", t_name); // 绑定教师姓名参数

    if (!query.exec()) { // 如果查询执行失败
        QMessageBox::critical(this, "错误", "填入课程号错误\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        return; // 返回，不继续执行后面的操作
    }

    QSqlQueryModel *c_model = new QSqlQueryModel; // 创建 QSqlQueryModel 对象，用于显示查询结果
    c_model->setQuery(query); // 将查询结果设置到数据模型中
    c_model->setHeaderData(0, Qt::Horizontal, "课程号"); // 设置表头数据
    c_model->setHeaderData(1, Qt::Horizontal, "课程名");
    c_model->setHeaderData(2, Qt::Horizontal, "学号");
    c_model->setHeaderData(3, Qt::Horizontal, "姓名");
    c_model->setHeaderData(4, Qt::Horizontal, "成绩");
    c_model->setHeaderData(5, Qt::Horizontal, "绩点");

    ui->tableView->setModel(c_model); // 将数据模型设置到表格视图中显示
}

void teacherControl::on_deleteButton_clicked()
{
    // 清空课程号、学号和成绩输入框内容
    ui->input_cid->clear();
    ui->input_sid->clear();
    ui->input_grade->clear();
    Init(); // 调用 Init() 方法重新初始化界面
}

void teacherControl::on_pushButton_clicked()
{
    if (ui->input_cid->text().isEmpty()) { // 如果课程号输入框为空
        QMessageBox::critical(this, "错误", "未填入课程号\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        Init(); // 调用 Init() 方法重新初始化界面
        return; // 返回，不继续执行后面的操作
    }

    QString _cid = ui->input_cid->text(); // 获取输入的课程号

    if (ui->input_sid->text().isEmpty()) { // 如果学号输入框为空
        QMessageBox::critical(this, "错误", "未填入学号\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        return; // 返回，不继续执行后面的操作
    }
    QString _sid = ui->input_sid->text(); // 获取输入的学号

    if (ui->input_grade->text().isEmpty()) { // 如果成绩输入框为空
        QMessageBox::critical(this, "错误", "未填入成绩\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        return; // 返回，不继续执行后面的操作
    }
    QString _grade = ui->input_grade->text(); // 获取输入的成绩

    QString _credit; // 定义绩点变量
    // 根据成绩设置绩点等级
    if (_grade.toInt() >= 0 && _grade.toInt() < 60) {
        _credit = "0.0";
    } else if (_grade.toInt() >= 60 && _grade.toInt() < 64) {
        _credit = "1.0";
    } else if (_grade.toInt() >= 64 && _grade.toInt() < 68) {
        _credit = "1.7";
    } else if (_grade.toInt() >= 68 && _grade.toInt() < 72) {
        _credit = "2.0";
    } else if (_grade.toInt() >= 72 && _grade.toInt() < 75) {
        _credit = "2.3";
    } else if (_grade.toInt() >= 75 && _grade.toInt() < 78) {
        _credit = "2.7";
    } else if (_grade.toInt() >= 78 && _grade.toInt() < 82) {
        _credit = "3.0";
    } else if (_grade.toInt() >= 82 && _grade.toInt() < 85) {
        _credit = "3.3";
    } else if (_grade.toInt() >= 85 && _grade.toInt() < 90) {
        _credit = "3.7";
    } else if (_grade.toInt() >= 90 && _grade.toInt() <= 100) {
        _credit = "4.0";
    } else {
        QMessageBox::critical(this, "错误", "填入成绩错误（0-100）\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        Init(); // 调用 Init() 方法重新初始化界面
        return; // 返回，不继续执行后面的操作
    }

    QSqlQuery query; // 创建数据库查询对象
    // 准备删除指定课程号和学号的学生选课记录的 SQL 语句
    query.prepare("DELETE FROM scourse WHERE sid=:sid AND cid=:cid");
    query.bindValue(":sid", _sid); // 绑定学号参数
    query.bindValue(":cid", _cid); // 绑定课程号参数
    query.exec(); // 执行删除操作

    // 准备插入学生选课记录的 SQL 语句
    query.prepare("INSERT INTO scourse (cid, sid, grade, credit) "
                  "VALUES (:cid, :sid, :grade, :credit)");
    query.bindValue(":cid", _cid); // 绑定课程号参数
    query.bindValue(":sid", _sid); // 绑定学号参数
    query.bindValue(":grade", _grade); // 绑定成绩参数
    query.bindValue(":credit", _credit); // 绑定绩点参数

    if (!query.exec()) { // 如果插入操作执行失败
        QMessageBox::critical(this, "错误", "填入信息错误\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        Init(); // 调用 Init() 方法重新初始化界面
        return; // 返回，不继续执行后面的操作
    }

    // 再次查询更新后的课程和学生选课信息
    query.prepare("SELECT e.cid, c.name AS 课程名, e.sid, s.name AS 学生姓名, e.grade AS 成绩, e.credit AS 绩点 "
                  "FROM scourse e, student s, course c "
                  "WHERE e.sid=s.id AND c.id=e.cid AND c.id=:course AND c.teacher=:name");
    query.bindValue(":course", _cid); // 绑定课程号参数
    query.bindValue(":name", t_name); // 绑定教师姓名参数

    if (!query.exec()) { // 如果查询执行失败
        QMessageBox::critical(this, "错误", "填入课程号错误\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示错误消息对话框
        Init(); // 调用 Init() 方法重新初始化界面
        return; // 返回，不继续执行后面的操作
    }

    QSqlQueryModel *c_model = new QSqlQueryModel; // 创建 QSqlQueryModel 对象，用于显示查询结果
    c_model->setQuery(query); // 将查询结果设置到数据模型中
    c_model->setHeaderData(0, Qt::Horizontal, "课程号"); // 设置表头数据
    c_model->setHeaderData(1, Qt::Horizontal, "课程名");
    c_model->setHeaderData(2, Qt::Horizontal, "学号");
    c_model->setHeaderData(3, Qt::Horizontal, "姓名");
    c_model->setHeaderData(4, Qt::Horizontal, "成绩");
    c_model->setHeaderData(5, Qt::Horizontal, "绩点");

    ui->tableView->setModel(c_model); // 将数据模型设置到表格视图中显示

    QMessageBox::information(this, "提示", "编辑完成\n", QMessageBox::Ok, QMessageBox::NoButton); // 显示信息提示对话框
}
