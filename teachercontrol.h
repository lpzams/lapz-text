#ifndef TEACHERCONTROL_H
#define TEACHERCONTROL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QMessageBox>
#include "globle.h"

namespace Ui {
class teacherControl;
}

class teacherControl : public QWidget
{
    Q_OBJECT

public:
    explicit teacherControl(QWidget *parent = nullptr);
    ~teacherControl();
private slots:
    void Init();
    void on_selectbutton_clicked();
    void on_pushButton_clicked();
    void on_deleteButton_clicked();


private:
    Ui::teacherControl *ui;
    QSqlDatabase db;
    QString t_name;
};

#endif // TEACHERCONTROL_H
