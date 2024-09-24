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
#include <QPlainTextEdit>
#include <QTableWidget>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <string>
#include <filesystem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
	void findMostConsecutiveCommonLines(const std::string& file_a, const std::string& file_b);
	bool ends_with(const std::string& str, const std::string& suffix);

private slots:
    void on_browse_button_clicked();
    void on_open_editor_clicked();
    void on_list_item_selected(QListWidgetItem *item);
    void on_go_button_clicked();

private:
    //Ui::MainWindow *ui;
    QMenu *file_menu;
    QMenu *view_menu;
    QMenu *help_menu;
    QMenu *about_menu;
    QTableWidget *table_widget;
    QPlainTextEdit *text_edit;
    QLineEdit *line_edit;
    QPushButton *browse_button;
    QSpinBox *spin_box;
    QComboBox *comboBox_language;
    QCheckBox *checkBox_subdirection;
    QCheckBox *checkBox_literals;
    QCheckBox *checkBox_identifiers;
    QListWidget *list_widget;
    QComboBox *comboBox_encoding;
    QPushButton *open_editor;
    QPushButton *go_button;
    QString filename;
};

#endif // MAINWINDOW_H

