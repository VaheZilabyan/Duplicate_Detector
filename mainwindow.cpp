#include "mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidgetItem>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QProcess>
#include <QDebug>

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
    table_widget->setRowCount(0);
    table_widget->setHorizontalHeaderItem(0, new QTableWidgetItem("Source"));
    table_widget->setHorizontalHeaderItem(1, new QTableWidgetItem("Matches"));
    table_widget->setHorizontalHeaderItem(2, new QTableWidgetItem("Lines"));

    /*for (int i = 0; i < 5; ++i) {
        int row = table_widget->rowCount();  // Get the current number of rows
        table_widget->insertRow(row);  // Insert a new row at the end
        table_widget->setItem(row, 0, new QTableWidgetItem(QString("Item %1").arg(i)));
        table_widget->setItem(row, 1, new QTableWidgetItem(QString::number(i * 10)));
    }*/

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
    checkBox_subdirection->setChecked(true);
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
    connect(go_button, &QPushButton::clicked, this, &MainWindow::on_go_button_clicked);
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
    connect(list_widget, &QListWidget::itemClicked, this, &MainWindow::on_list_item_selected);
}

void MainWindow::on_list_item_selected(QListWidgetItem *item) {
    if (!item) {
        return; // Guard against null pointer
    }

    QString filePath = item->data(Qt::UserRole).toString(); // Get the absolute path if you stored it
    statusBar()->showMessage(filePath); // Display the selected file path in the status bar

    QString filename = filePath + "/" + item->text();
    // Optionally, open the file or perform another action based on the selection
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return; // Handle error if necessary
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    text_edit->setPlainText(content);  // Display the content of the selected file in text_edit
}

void MainWindow::findMostConsecutiveCommonLines(const std::string& file_a, const std::string& file_b, size_t &max_len, std::pair<int,int> &line_numbers) {
    std::ifstream fa(file_a);
    std::ifstream fb(file_b);
    
    if (!fa.is_open() || !fb.is_open()) {
        std::cerr << "Error opening files: " << file_a << " or " << file_b << std::endl;
        return;
    }
    
    std::vector<std::string> lines_a, lines_b;
    std::string line;

    // Read lines from both files
    while (std::getline(fa, line)) {
        lines_a.push_back(line);
    }
    while (std::getline(fb, line)) {
        lines_b.push_back(line);
    }

    int len_a = lines_a.size();
    int len_b = lines_b.size();
    std::vector<std::vector<int>> dp(len_a + 1, std::vector<int>(len_b + 1, 0));

    max_len = 0; // Length of longest match
    std::pair<int, int> max_pos(0, 0); // End position of the longest match

    // Dynamic programming table to find longest common consecutive lines
    for (int i = 1; i <= len_a; ++i) {
        for (int j = 1; j <= len_b; ++j) {
            if (lines_a[i - 1] == lines_b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > max_len) {
                    max_len = dp[i][j];
                    max_pos = {i, j};
                }
            }
        }
    }


    // Extract the longest common sequence
    if (max_len > 0) {
        std::cout << "Most common duplicate lines between given files are " << max_len << " consecutive lines:" << std::endl;
        for (int i = max_pos.first - max_len; i < max_pos.first; ++i) {
            std::cout << lines_a[i] << std::endl;
        }
    } else {
        std::cout << "No common consecutive lines found." << std::endl;
    }
}
bool MainWindow::ends_with(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false; // str is shorter than suffix
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

void MainWindow::on_go_button_clicked() {
    QString directory = line_edit->text();  // Get the directory entered in the line edit.
    int threshold = spin_box->value();       // Get the value from the spin box.
    QString language = comboBox_language->currentText(); // Get the selected language from the combo box.
    //bool ignoreLiterals = checkBox_literals->isChecked(); // Check if "Ignore literals?" is checked.
    //bool ignoreIdentifiers = checkBox_identifiers->isChecked(); // Check if "Ignore identifiers?" is checked.
    bool includeSubdirs = checkBox_subdirection->isChecked(); // Check if "Also can be subdirection?" is checked.

    // Here, you would typically start the process of analyzing the source code in the specified directory.
    // You could emit a signal, call a method, or start a new process that performs your analysis.

    // For demonstration, we'll just update the status bar with the parameters chosen.
    QString message = QString("Going to analyze %1 with threshold %2 for %3").arg(directory).arg(threshold).arg(language);
    statusBar()->showMessage(message);
	std::vector<std::string> valid_extensions;

	if (language == "C++") {
		valid_extensions = {".cpp", ".h", ".hpp", ".c", ".cc"};
	}
	// TODO: add else ifs for other languages
	else {
		valid_extensions = {".cpp", ".h", ".hpp", ".c", ".cc"};
		std::cout << "We should never reach there, there is a bug in the code: Call 911" << std::endl;
	}

	//std::cout << "MY DIR: " << directory.toStdString();
	// Changing dir    
	if (chdir(directory.toStdString().c_str()) != 0) {
        perror("chdir failed");
    }

    if(includeSubdirs) {
    	std::vector<std::string> files;
		// Traverse the directory and collect valid files
		for (const auto& entry : std::filesystem::directory_iterator("./")) {
		    if (entry.is_regular_file()) {
                std::string file_path = entry.path().string();
		        for (const auto& extension : valid_extensions) {
		            if (ends_with(file_path, extension)) {
		                files.push_back(file_path);
		                break;
		            }
		        }
		    }
		}

		////
		for (size_t i = 0; i < files.size(); ++i) {
            std::cout << files[i] << " -- ";
        } std::cout << "\n";
		////

		// Compare each file with every other file
        size_t match_count = 0;
        std::pair<int, int> line_numbers;
		for (size_t i = 0; i < files.size(); ++i) {
            for (size_t j = i + 1; j < files.size(); ++j) {
		        if (i != j) {  // Avoid processing the same file
		            std::cout << "Processing " << files[i] << " with " << files[j] << std::endl;
                    findMostConsecutiveCommonLines(files[i], files[j], match_count, line_numbers);
                    int row = table_widget->rowCount();  // Get the current number of rows
                    table_widget->insertRow(row);  // Insert a new row at the end
                    table_widget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(files[i]) + " and " + QString::fromStdString(files[j])));
                    table_widget->setItem(row, 1, new QTableWidgetItem(QString::number(match_count)));   //matches (count)
                    table_widget->setItem(row, 2, new QTableWidgetItem(QString::number(line_numbers.first) + "-" + QString::number(line_numbers.second)));   //line numbers
		        }
		    }
        }
    }
    else {
        qDebug() << "TODO: Not implemented";
    }
    // You may want to call another method for actual processing, e.g.:
    // startAnalysis(directory, threshold, language, ignoreLiterals, ignoreIdentifiers, includeSubdirs);
}

