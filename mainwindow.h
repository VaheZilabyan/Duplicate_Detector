
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_browse_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
private:
    void set_text_in_textEdit(QString filename);
private:
    Ui::MainWindow *ui;
    QString dirname;
};

#endif // MAINWINDOW_H
