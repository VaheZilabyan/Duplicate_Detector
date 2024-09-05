#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QListWidget>
#include <QTextEdit>
#include <QTableWidget>

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
    void on_browse_button_clicked();

private:
    //Ui::MainWindow *ui;
    QMenu *file_menu;
    QMenu *view_menu;
    QMenu *help_menu;
    QMenu *about_menu;
    QTableWidget *table_widget;
    QTextEdit *text_edit;
    QLineEdit *line_edit;
    QPushButton *browse_button;
    QSpinBox *spin_box;
    QComboBox *comboBox_language;
    QCheckBox *checkBox_subdirection;
    QCheckBox *checkBox_literals;
    QCheckBox *checkBox_identifiers;
    QListWidget *list_widget;
    QComboBox *comboBox_encoding;
    QPushButton *go_button;
};

#endif // MAINWINDOW_H
