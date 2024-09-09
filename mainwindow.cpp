#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidgetItem>

#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    this->resize(800, 660);
    this->setMinimumWidth(650);
    this->setMinimumHeight(550);
    this->statusBar()->showMessage("test status");

    file_menu = new QMenu("File", this);
    view_menu = new QMenu("View", this);
    help_menu = new QMenu("Help", this);
    about_menu = new QMenu("About", this);
    this->menuBar()->addMenu(file_menu);
    this->menuBar()->addMenu(view_menu);
    this->menuBar()->addMenu(help_menu);
    this->menuBar()->addMenu(about_menu);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QWidget *north_widget = new QWidget(this);
    QWidget *south_widget = new QWidget(this);
    north_widget->setMinimumHeight(200);
    north_widget->setMaximumHeight(200);

    mainLayout->addWidget(north_widget);
    mainLayout->addWidget(south_widget);
    setCentralWidget(centralWidget);

    QHBoxLayout *south_layout = new QHBoxLayout(south_widget);
    table_widget = new QTableWidget(south_widget);
    table_widget->setColumnCount(3);
    table_widget->setHorizontalHeaderItem(0, new QTableWidgetItem("Source"));
    table_widget->setHorizontalHeaderItem(1, new QTableWidgetItem("Matches"));
    table_widget->setHorizontalHeaderItem(2, new QTableWidgetItem("Lines"));

    text_edit = new QPlainTextEdit(south_widget);
    text_edit->setReadOnly(true);
    south_layout->addWidget(table_widget);
    south_layout->addWidget(text_edit);

    QVBoxLayout *north_layout = new QVBoxLayout(north_widget);
    // --- 1 ---- //
    QLabel *root_source_dir = new QLabel("Root source directory", north_widget);
    line_edit = new QLineEdit(north_widget);
    browse_button = new QPushButton("Browse", north_widget);
    QHBoxLayout *layout_1 = new QHBoxLayout();
    layout_1->addWidget(root_source_dir);
    layout_1->addWidget(line_edit);
    layout_1->addWidget(browse_button);

    // --- 2 ---- //
    QLabel *label_spinbox = new QLabel("Reporte duplicate chunks larger than");
    QLabel *label_language = new QLabel("Language");
    spin_box = new QSpinBox(north_widget);
    spin_box->setMaximum(1000);
    comboBox_language = new QComboBox(north_widget);
    comboBox_language->addItem("C++");
    comboBox_language->addItem("C");
    comboBox_language->addItem("Java");
    comboBox_language->addItem("Python");
    QHBoxLayout *layout_2 = new QHBoxLayout();
    layout_2->addWidget(label_spinbox);
    layout_2->addWidget(spin_box);
    layout_2->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_2->addWidget(label_language);
    layout_2->addWidget(comboBox_language);

    // --- 3 ---- //
    checkBox_subdirection = new QCheckBox("Also can be subdirection?", north_widget);
    checkBox_literals = new QCheckBox("Ignore literals?", north_widget);
    checkBox_identifiers = new QCheckBox("Ignore identifiers?", north_widget);
    QVBoxLayout *checkBox_layout = new QVBoxLayout();
    checkBox_layout->addWidget(checkBox_subdirection);
    checkBox_layout->addWidget(checkBox_literals);
    checkBox_layout->addWidget(checkBox_identifiers);
    list_widget = new QListWidget(north_widget);
    QHBoxLayout *layout_3 = new QHBoxLayout();
    layout_3->addLayout(checkBox_layout);
    layout_3->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_3->addWidget(list_widget);

    // --- 4 ---- //
    QLabel *label_file_encoding = new QLabel("File encoding");
    comboBox_encoding = new QComboBox(north_widget);
    comboBox_encoding->addItem("UTF-8");
    open_editor = new QPushButton("Open in Editor", north_widget);
    go_button = new QPushButton("Go", north_widget);
    QHBoxLayout *layout_4 = new QHBoxLayout();
    layout_4->addWidget(label_file_encoding);
    layout_4->addWidget(comboBox_encoding);
    layout_4->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_4->addWidget(open_editor);
    layout_4->addWidget(go_button);

    north_layout->addLayout(layout_1);
    north_layout->addLayout(layout_2);
    north_layout->addLayout(layout_3);
    north_layout->addLayout(layout_4);

    connect(browse_button, &QPushButton::clicked, this, &MainWindow::on_browse_button_clicked);
    connect(open_editor, &QPushButton::clicked, this, &MainWindow::on_open_editor_clicked);
}

void MainWindow::on_open_editor_clicked() {
    statusBar()->showMessage(filename);

    if (filename.isEmpty()) {
        statusBar()->showMessage("Browse file");
        return;
    }
    //QFileInfo file_info(filename);
    //const QString& filePath = file_info.absolutePath();
    const QString& filePath = "D:/cpp_programs/c++/c++17/optional.cpp";

    int line = 1;
    QString program = "C:/Users/vzila/AppData/Local/Programs/Microsoft VS Code/Code.exe";
    QStringList arguments;
    arguments << "--goto" << QString("%1:%2").arg(filePath).arg(line);
    bool success = QProcess::startDetached(program, arguments);
    if (!success) {
        qDebug() << "Failed to launch VS Code!";
    }
}

void MainWindow::on_browse_button_clicked()
{
    list_widget->clear();
    text_edit->clear();

    QString fileContent;
    QString filename_ = QFileDialog::getOpenFileName(this, "Choose File");
    this->filename = filename_;
    if(filename.isEmpty())
        return;

    QFile file(filename_);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    QTextStream in(&file);
    fileContent = in.readAll();
    file.close();
    text_edit->setPlainText(fileContent);

    QString dirname = filename_.remove(filename_.split('/').last());
    QDir dir(dirname);
    for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
    {
        QListWidgetItem *item = new QListWidgetItem(file.fileName());
        item->setData(Qt::UserRole, file.absolutePath()); // if you need absolute path of the file
        list_widget->addItem(item);
    }
    line_edit->setText(dirname);
}

MainWindow::~MainWindow()
{
    //delete ui;
}
