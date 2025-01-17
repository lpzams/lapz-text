#ifndef TEXT_H
#define TEXT_H

#include "qboxlayout.h"
#include <QMainWindow>
#include <QPushButton>
#include <QDockWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QMap>
#include <QTextBlock>

QT_BEGIN_NAMESPACE
namespace Ui { class text; }
QT_END_NAMESPACE

class text : public QMainWindow
{
    Q_OBJECT

signals:
    void windowClosed();  // 定义一个信号，用于通知窗口关闭

public:
    text(QWidget *parent = nullptr);
    ~text();

private slots:
    void setupUI();
    void readBook();
    void updateChapterTreeWidget(const QString &content);
    int convertChapterNameToInt(const QString &chapterName);
    void fontBig();
    void fontSmall();
    void updateBackground();
    void onItemClicked(QTreeWidgetItem *item, int column);
    void openMarkdownFile();
    void saveBook(); // 新添加的保存函数

private:
    Ui::text *ui;
    QWidget *titleWin;
    QHBoxLayout *titleLayout;
    QPushButton *readButton;
    QPushButton *bigFont;
    QPushButton *smallFont;
    QPushButton *backgroundColor;
    QPushButton *openMarkdownBtn;
    QDockWidget *menuDock;
    QTreeWidget *treeWidget;
    QDockWidget *listDock;
    QTextEdit *mainText;
    QMap<QString, QPair<int, int>> chapterIndexDict;
    int fontSize;
    QPushButton *saveButton; // 新添加的保存按钮

};
#endif // TEXT_H
