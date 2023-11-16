#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::on_pushButton_browse_clicked()
{
    ui->listWidget->clear();
    ui->textEdit->clear();

    QString fileContent;
    QString filename = QFileDialog::getOpenFileName(this, "Choose File");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    QTextStream in(&file);
    fileContent = in.readAll();
    file.close();
    ui->textEdit->setPlainText(fileContent);

    QString dirname = filename.remove(filename.split('/').last());
    QDir dir(dirname);
    for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
    {
        QListWidgetItem *item = new QListWidgetItem(file.fileName());
        item->setData(Qt::UserRole, file.absolutePath()); // if you need absolute path of the file
        ui->listWidget->addItem(item);
    }
    ui->lineEdit->setText(dirname);
}

MainWindow::~MainWindow()
{
    delete ui;
}
