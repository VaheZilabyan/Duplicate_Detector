#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidgetItem>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QProcess>
#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
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
    void findMostConsecutiveCommonLines(const std::string& file_a, const std::string& file_b, int& max_len, std::pair<int, int> &line_numbers);
    bool ends_with(const std::string& str, const std::string& suffix);
	void findDuplicates(bool includeSubdirs, QString language);
    void clearContent();
	~MainWindow();

private slots:
    void on_browse_button_clicked();
    void on_open_editor_clicked();
    void on_list_item_selected(QListWidgetItem *item);
    void on_go_button_clicked();
    void on_root_source_dir_line_edit_returnPressed();
    void findingsRowSelected();

private:
	// Size related constants
	static constexpr int MIN_WIDTH = 650;
	static constexpr int MIN_HEIGHT = 550;
    static constexpr int START_SOURCE_WIDE_SIZE = 400;
    static constexpr std::pair<int, int> StartSize = {1400, 900};

    QMenu *file_menu;
    QMenu *view_menu;
    QMenu *help_menu;
    QMenu *about_menu;
    QTableWidget *findings_table_widget;
    QPlainTextEdit *file_content_text_edit;
    QPlainTextEdit *right_file_content_text_edit; // for test
    QLineEdit *root_source_dir_line_edit;
    QPushButton *browse_button;
    QSpinBox *chunks_larger_spin_box;
    QComboBox *comboBox_language;
    QCheckBox *checkBox_subdirection;
    QCheckBox *checkBox_literals;
    QCheckBox *checkBox_identifiers;
    QListWidget *files_list_widget;
    QComboBox *comboBox_encoding;
    QPushButton *open_editor;
    QPushButton *go_button;
    QString filename;
};

#endif // MAINWINDOW_H

