#ifndef STUDENTCONTROL_H
#define STUDENTCONTROL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QMessageBox>
#include "report.h"

#include "globle.h"

namespace Ui {
class studentControl;
}

class studentControl : public QWidget
{
    Q_OBJECT

public:
    explicit studentControl(QWidget *parent = nullptr);
    ~studentControl();

private slots:
//    void on_currentChanged(const QModelIndex &current,const QModelIndex &previous);

     void Init();

     void on_selectbutton_clicked();
     void on_deletebutton_clicked();
     void on_closebutton_clicked();
     void on_reportButton_clicked();

//    void on_actQuit_triggered();

//    void on_actRecAppend_triggered();

//    void on_actRecDelete_triggered();

//    void on_actRecInsert_triggered();

//    void on_actSubmit_triggered();

//    void on_actRevert_triggered();


private:
    Ui::studentControl *ui;
    QSqlDatabase db;


    QSqlRelationalTableModel *tabModelSCourse;
    QItemSelectionModel *theSelectionSCourse;

    QSqlTableModel *tabModelAdmin;
    QItemSelectionModel *theSelectionAdmin;

    void openTable();
//    void getFieldNames();
};

#endif // STUDENTCONTROL_H
