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

void MainWindow::set_text_in_textEdit(QString filename)
{
    QString fileContent;
    if(filename.isEmpty())
        return;
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    QTextStream in(&file);
    fileContent = in.readAll();
    file.close();
    ui->textEdit->clear();
    ui->textEdit->setPlainText(fileContent);
}

void MainWindow::on_pushButton_browse_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose File");
    set_text_in_textEdit(filename);

    // show all files
    dirname = filename.remove(filename.split('/').last());
    QDir dir(dirname);
    ui->listWidget->clear();
    for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
    {
        QListWidgetItem *item = new QListWidgetItem(file.fileName());
        item->setData(Qt::UserRole, file.absolutePath());
        ui->listWidget->addItem(item);
    }
    ui->lineEdit->setText(dirname);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)

{
    ui->statusbar->showMessage(item->text());

    QString filename = dirname + item->text();
    set_text_in_textEdit(filename);
}

MainWindow::~MainWindow()
{
    delete ui;
}
