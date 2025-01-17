#include "text.h"
#include "ui_text.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QRegularExpression>
#include <QTextDocument>
#include <QMessageBox>
#include <QDebug>

text::text(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::text)
{
    ui->setupUi(this);
    setupUI();  // 调用自定义的界面设置函数
}

text::~text()
{
    delete ui;
}

void text::setupUI()
{
    setWindowTitle("文本阅读器");  // 设置窗口标题
    resize(800, 600);  // 设置窗口初始大小

    // 创建布局和按钮
    titleWin = new QWidget(this);
    titleLayout = new QHBoxLayout(titleWin);
    readButton = new QPushButton("读取文本", this);  // 创建读取文本按钮
    bigFont = new QPushButton("字体变大", this);  // 创建字体放大按钮
    smallFont = new QPushButton("字体变小", this);  // 创建字体缩小按钮
    backgroundColor = new QPushButton("随机背景颜色", this);  // 创建随机背景颜色按钮
    openMarkdownBtn = new QPushButton("打开Markdown", this);  // 新增的打开Markdown文件按钮
    saveButton = new QPushButton("保存文本", this);  // 新增的保存文本按钮

    // 添加按钮到布局
    titleLayout->addWidget(readButton);
    titleLayout->addWidget(openMarkdownBtn);  // 将新按钮添加到布局中
    titleLayout->addWidget(saveButton);  // 将保存按钮添加到布局中
    titleLayout->addWidget(backgroundColor);
    titleLayout->addWidget(bigFont);
    titleLayout->addWidget(smallFont);

    // 设置菜单栏
    menuDock = new QDockWidget("菜单", this);
    menuDock->setWidget(titleWin);
    menuDock->setFeatures(QDockWidget::NoDockWidgetFeatures);  // 设置菜单栏不可脱离
    addDockWidget(Qt::TopDockWidgetArea, menuDock);

    // 设置目录栏
    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderHidden(true);  // 隐藏表头
    listDock = new QDockWidget("目录栏", this);
    listDock->setWidget(treeWidget);
    addDockWidget(Qt::LeftDockWidgetArea, listDock);

    // 主文本编辑区
    mainText = new QTextEdit(this);
    setCentralWidget(mainText);
    fontSize = 12;  // 设置初始字号
    mainText->setFontPointSize(fontSize);

    // 连接按钮的信号和槽
    connect(readButton, &QPushButton::clicked, this, &text::readBook);  // 连接读取文本按钮信号和槽
    connect(bigFont, &QPushButton::clicked, this, &text::fontBig);  // 连接字体变大按钮信号和槽
    connect(smallFont, &QPushButton::clicked, this, &text::fontSmall);  // 连接字体变小按钮信号和槽
    connect(backgroundColor, &QPushButton::clicked, this, &text::updateBackground);  // 连接随机背景颜色按钮信号和槽
    connect(treeWidget, &QTreeWidget::itemClicked, this, &text::onItemClicked);  // 连接目录树节点点击信号和槽
    connect(openMarkdownBtn, &QPushButton::clicked, this, &text::openMarkdownFile);  // 连接打开Markdown文件按钮信号和槽
    connect(saveButton, &QPushButton::clicked, this, &text::saveBook);  // 连接保存文本按钮信号和槽
}

void text::readBook() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择文本文件", "", "Text files (*.txt)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray rawData = file.readAll();
            QString content = QString::fromUtf8(rawData);

            chapterIndexDict.clear();  // 清空章节索引字典

            QTextStream in(&content);

            QString chapterName;  // 用于存储当前章节名
            int chapterStartIndex = 0; // 用于存储当前章节的起始行索引
            int lineIndex = 0; // 行索引

            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();  // 去除行首尾的空白字符

                // 尝试匹配章节名
                QRegularExpression pattern("第\\s*([一二三四五六七八九十]+)\\s*章\\s+(.*)");
                QRegularExpressionMatch match = pattern.match(line);
                if (match.hasMatch()) {
                    QString chapterNumber = match.captured(1); // 获取章节号
                    QString chapterNameCandidate = match.captured(2).trimmed(); // 获取章节名候选
                    if (!chapterNumber.isEmpty() && !chapterNameCandidate.isEmpty()) {
                        // 格式化章节名，去除多余的 "第"
                        chapterName = QString("第%1章 %2").arg(chapterNumber, chapterNameCandidate);
                        chapterStartIndex = lineIndex;
                    }
                } else {
                    // 如果当前行不是章节名，但已经有章节名存在，则将内容添加到上一个章节
                    if (!chapterName.isEmpty()) {
                        chapterIndexDict.insert(chapterName, qMakePair(chapterStartIndex, lineIndex));
                        chapterName.clear(); // 清空章节名，准备记录下一个章节
                    }
                }

                lineIndex++;
            }

            // 处理最后一个章节，如果文件以章节结束而不是空行结束
            if (!chapterName.isEmpty()) {
                chapterIndexDict.insert(chapterName, qMakePair(chapterStartIndex, lineIndex - 1));
            }

            file.close();

            // 构建章节目录
            updateChapterTreeWidget(content); // 调用更新目录树的函数

            // 显示文件内容
            mainText->setText(content);
        }
    }
}

void text::updateChapterTreeWidget(const QString &content) {
    // 获取章节名及其起始行索引对
    QList<QPair<int, QString>> chapters;
    for (const auto &chapter : chapterIndexDict.keys()) {
        chapters.append(qMakePair(chapterIndexDict.value(chapter).first, chapter));
    }

    // 根据章节起始行索引排序
    std::sort(chapters.begin(), chapters.end(), [](const QPair<int, QString> &a, const QPair<int, QString> &b) {
        return a.first < b.first;
    });

    // 清空目录树
    treeWidget->clear();

    // 添加排序后的章节名到目录栏
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget, QStringList("目录"));
    QStringList addedChapters; // 用于避免重复添加章节名
    for (const auto &chapter : chapters) {
        if (!addedChapters.contains(chapter.second)) {
            new QTreeWidgetItem(rootItem, QStringList(chapter.second));
            addedChapters.append(chapter.second);
        }
    }

    // 显示文件内容
    mainText->setText(content);
}

int text::convertChapterNameToInt(const QString &chapterName) {
    static QRegularExpression numberPattern("第([一二三四五六七八九十]+)章");
    QRegularExpressionMatch match = numberPattern.match(chapterName);
    if (match.hasMatch()) {
        QString chineseNumber = match.captured(1);
        static QMap<QString, int> numberMap = {
            {"一", 1}, {"二", 2}, {"三", 3}, {"四", 4}, {"五", 5},
            {"六", 6}, {"七", 7}, {"八", 8}, {"九", 9}, {"十", 10}
        };
        return numberMap.value(chineseNumber, 0); // 返回对应的数字，未匹配时返回 0
    }
    return 0;
}

void text::fontBig() {
    fontSize += 1;
    mainText->setFontPointSize(fontSize);
}

void text::fontSmall() {
    fontSize -= 1;
    if (fontSize < 1) {
        fontSize = 1;  // 避免字号小于 1
    }
    mainText->setFontPointSize(fontSize);
}

void text::updateBackground() {
    QColor randomColor(rand() % 256, rand() % 256, rand() % 256);
    QPalette pal = mainText->palette();
    pal.setColor(QPalette::Base, randomColor);
    mainText->setPalette(pal);
}

void text::onItemClicked(QTreeWidgetItem *item, int column) {
    QString chapterName = item->text(column);
    if (chapterIndexDict.contains(chapterName)) {
        QPair<int, int> indices = chapterIndexDict.value(chapterName);
        QTextCursor cursor(mainText->document()->findBlockByLineNumber(indices.first));
        cursor.setPosition(mainText->document()->findBlockByLineNumber(indices.second).position(), QTextCursor::KeepAnchor);
        mainText->setTextCursor(cursor);
        mainText->ensureCursorVisible();  // 确保光标可见
    }
}


void text::openMarkdownFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择Markdown文件", "", "Markdown files (*.md)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray rawData = file.readAll();
            QString content = QString::fromUtf8(rawData);
            file.close();

            // 显示Markdown内容
            mainText->setMarkdown(content);
        }
    }
}

void text::saveBook() {
    QString filePath = QFileDialog::getSaveFileName(this, "保存文本文件", "", "Text files (*.txt)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << mainText->toPlainText();
            file.close();
            QMessageBox::information(this, "保存成功", "文本文件保存成功！");
        } else {
            QMessageBox::warning(this, "保存失败", "无法保存文本文件！");
        }
    }
}
