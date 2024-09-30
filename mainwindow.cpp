#include "mainwindow.h"

//
// Constructor
// Setup the main window layouts
//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(StartSize.first, StartSize.second);
    this->setMinimumWidth(MIN_WIDTH);
    this->setMinimumHeight(MIN_HEIGHT);
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
    findings_table_widget = new QTableWidget(south_widget);
    findings_table_widget->setColumnCount(3);
    findings_table_widget->setRowCount(0);
    findings_table_widget->setColumnWidth(0, START_SOURCE_WIDE_SIZE);
    findings_table_widget->setHorizontalHeaderItem(0, new QTableWidgetItem("Source"));
    findings_table_widget->setHorizontalHeaderItem(1, new QTableWidgetItem("Matches"));
    findings_table_widget->setHorizontalHeaderItem(2, new QTableWidgetItem("Lines"));
    findings_table_widget->setSortingEnabled(true);
    //
    // Make it not editable
    //
    findings_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    file_content_text_edit = new QPlainTextEdit(south_widget);
    file_content_text_edit->setReadOnly(true);
    right_file_content_text_edit = new QPlainTextEdit(south_widget);
    right_file_content_text_edit->setReadOnly(true);
    right_file_content_text_edit->hide();

    south_layout->addWidget(findings_table_widget);
    south_layout->addWidget(file_content_text_edit);
    south_layout->addWidget(right_file_content_text_edit);

    QVBoxLayout *north_layout = new QVBoxLayout(north_widget);
    // --- 1 ---- //
    QLabel *root_source_dir_label = new QLabel("Root source directory", north_widget);
    root_source_dir_line_edit = new QLineEdit(north_widget);
    browse_button = new QPushButton("Browse", north_widget);
    QHBoxLayout *layout_1 = new QHBoxLayout();
    layout_1->addWidget(root_source_dir_label);
    layout_1->addWidget(root_source_dir_line_edit);
    layout_1->addWidget(browse_button);

    // --- 2 ---- //
    QLabel *label_spinbox = new QLabel("Report duplicate chunks larger than");
    QLabel *label_language = new QLabel("Language");
    chunks_larger_spin_box = new QSpinBox(north_widget);
    chunks_larger_spin_box->setMaximum(1000);
    comboBox_language = new QComboBox(north_widget);
    comboBox_language->addItem("C++");
    comboBox_language->addItem("C");
    comboBox_language->addItem("Java");
    comboBox_language->addItem("Python");
    QHBoxLayout *layout_2 = new QHBoxLayout();
    layout_2->addWidget(label_spinbox);
    layout_2->addWidget(chunks_larger_spin_box);
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
    files_list_widget = new QListWidget(north_widget);
    QHBoxLayout *layout_3 = new QHBoxLayout();
    layout_3->addLayout(checkBox_layout);
    layout_3->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_3->addWidget(files_list_widget);

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

    //
    // Make connections
    //
    connect(findings_table_widget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::findingsRowSelected);
    connect(browse_button, &QPushButton::clicked, this, &MainWindow::on_browse_button_clicked);
    connect(open_editor, &QPushButton::clicked, this, &MainWindow::on_open_editor_clicked);
    connect(go_button, &QPushButton::clicked, this, &MainWindow::on_go_button_clicked);
    connect(files_list_widget, &QListWidget::itemClicked, this, &MainWindow::on_list_item_selected);
	connect(root_source_dir_line_edit, &QLineEdit::returnPressed, this, &MainWindow::on_root_source_dir_line_edit_returnPressed);
}

//
// Opens the file in Editor (VS Code)
//
void MainWindow::on_open_editor_clicked() {
    statusBar()->showMessage(filename);

    if (filename.isEmpty()) {
        statusBar()->showMessage("Browse file");
        return;
    }

    int line = 1;
    const QString program = "C:/Users/vzila/AppData/Local/Programs/Microsoft VS Code/Code.exe";
    QStringList arguments;
    arguments << "--goto" << QString("%1:%2").arg(filename).arg(line);
    bool success = QProcess::startDetached(program, arguments);
    if (!success) {
        qDebug() << "Failed to launch VS Code!";
    }
}

//
// Clears table and files content 
//
void MainWindow::clearContent()
{
    files_list_widget->clear();
    file_content_text_edit->clear();
}

//
// Handles enter key press on root source line edit. It should update the root source dir path
//
void MainWindow::on_root_source_dir_line_edit_returnPressed()
{
    clearContent();

    QString dirname = root_source_dir_line_edit->text();
    QDir dir(dirname);
    for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
    {
        QListWidgetItem *item = new QListWidgetItem(file.fileName());
        item->setData(Qt::UserRole, file.absolutePath());
        files_list_widget->addItem(item);
    }

}

//
// Handle findings table row selection which should open file content in the text edit area at right.
//
void MainWindow::findingsRowSelected() {
    //
    // Clear files list widget selection to avoid confusion.
    //
    files_list_widget->clearSelection();

    QString directory = root_source_dir_line_edit->text();
    int selectedRow = findings_table_widget->currentRow();
    QString source = "";
    QString source_2 = "";

    if (selectedRow >= 0) {
        source = findings_table_widget->item(selectedRow, 0)->text().section(' ', 0, 0);
        source_2 = findings_table_widget->item(selectedRow, 0)->text().section(' ', 2, 2);
    }

    statusBar()->showMessage(directory);

    QString filename = directory + "/" + source;
    QString filename_2 = directory + "/" + source_2;
    QFile file(filename);
    QFile file_2(filename_2);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) || !file_2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QTextStream in_2(&file_2);
    QString content_2 = in_2.readAll();
    file_2.close();

	//
	// Display the content of the selected file in text_edit
	//
    file_content_text_edit->setPlainText(content);
    right_file_content_text_edit->show();
    right_file_content_text_edit->setPlainText(content_2);
}

//
// Handles browse button click. It should open file browser.
//
void MainWindow::on_browse_button_clicked()
{
    clearContent();

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
    file_content_text_edit->setPlainText(fileContent);

    QString dirname = filename_.remove(filename_.split('/').last());
    QDir dir(dirname);
    for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
    {
        QListWidgetItem *item = new QListWidgetItem(file.fileName());
        item->setData(Qt::UserRole, file.absolutePath());
        files_list_widget->addItem(item);
    }
    root_source_dir_line_edit->setText(dirname);
    right_file_content_text_edit->hide();
}

//
// Handles file selection. It should preview the selected file in text edit.
//
void MainWindow::on_list_item_selected(QListWidgetItem *item) {
    //
    // Clear findings table widget row selection to avoid confusion.
    //
    findings_table_widget->clearSelection();

    if (!item) {
        return; // Guard against null pointer
    }

    QString filePath = item->data(Qt::UserRole).toString();
    statusBar()->showMessage(filePath);

    QString filename = filePath + "/" + item->text();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

	//
	// Display the content of the selected file in text_edit
	//
    file_content_text_edit->setPlainText(content);
    right_file_content_text_edit->hide();
}

//
// Main algorithm, finds most consecutive common lines between 2 files.
//
void MainWindow::findMostConsecutiveCommonLines(const std::string& file_a, const std::string& file_b, int& max_len, std::pair<int,int> &line_numbers) {
    std::ifstream fa(file_a);
    std::ifstream fb(file_b);
    
    if (!fa.is_open() || !fb.is_open()) {
        std::cerr << "Error opening files: " << file_a << " or " << file_b << std::endl;
        return;
    }
    
    std::vector<std::string> lines_a, lines_b;
    std::string line;
    //
    // Read lines from both files
    //
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

	//
    // Dynamic programming table to find longest common consecutive lines
	//
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

    line_numbers.first = max_pos.first - max_len + 1;
    line_numbers.second = max_pos.second - max_len + 1;

	/*
    // Extract the longest common sequence
    if (max_len > 0) {
        //std::cout << "Most common duplicate lines between given files are " << max_len << " consecutive lines:" << std::endl;
        for (int i = max_pos.first - max_len; i < max_pos.first; ++i) {
            //std::cout << lines_a[i] << std::endl;
        }
    } else {
        //std::cout << "No common consecutive lines found." << std::endl;
    }*/
}

//
// Checks whether string end withs some pattern (needed for valid extensions).
//
bool MainWindow::ends_with(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

//
// Based on the parameters, calls the main algorithm for each 2 files
//
void MainWindow::findDuplicates(bool includeSubdirs, QString language) {
    std::vector<std::string> valid_extensions;
    if (language == "C++") {
        valid_extensions = {".cpp", ".h", ".hpp", ".c", ".cc"};
    } else {
        valid_extensions = {".cpp", ".h", ".hpp", ".c", ".cc"};
        std::cout << "We should never reach there, there is a bug in the code: Call 911" << std::endl;
    }

    //
    // Helper lambda for iterator type
    //
    auto collectFiles = [&](auto it) {
        std::vector<std::string> files;
        for (const auto& entry : it) {
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
        return files;
    };

    //
    // Collect files using appropriate iterator. Recursive iterators are for subdirs.
    //
    std::vector<std::string> files;
    if (includeSubdirs) {
        files = collectFiles(std::filesystem::recursive_directory_iterator("./"));
    } else {
        files = collectFiles(std::filesystem::directory_iterator("./"));
    }

    //
    // Compare each file with every other file
    //
    int match_count = 0;
    std::pair<int, int> line_numbers;
    for (size_t i = 0; i < files.size(); ++i) {
        for (size_t j = i + 1; j < files.size(); ++j) {
            //
            // Call the function that finds the most consecutive common lines
            //
            findMostConsecutiveCommonLines(files[i], files[j], match_count, line_numbers);
            int row = findings_table_widget->rowCount();
            findings_table_widget->insertRow(row);
            findings_table_widget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(files[i]) + " and " + QString::fromStdString(files[j])));
            //
            // This is needed for numerical sorting ability for match item column
            //
            QTableWidgetItem *matchesItem = new QTableWidgetItem();
            matchesItem->setData(Qt::DisplayRole, match_count);
            matchesItem->setData(Qt::UserRole, match_count);
            matchesItem->setText(QString::number(match_count));
            findings_table_widget->setItem(row, 1, matchesItem);
            findings_table_widget->setItem(row, 2, new QTableWidgetItem(QString::number(line_numbers.first) + "-" + QString::number(line_numbers.second)));
        }
    }
}

//
// Go button should run the findDuplicates which runs main algorithm based on parameters.
//
void MainWindow::on_go_button_clicked() {
    //
    // At first, clear the findings table widget.
    //
    findings_table_widget->setRowCount(0);

    //
    // Get the parameters.
    //
    QString directory = root_source_dir_line_edit->text();
    int threshold = chunks_larger_spin_box->value();
    QString language = comboBox_language->currentText();
    //bool ignoreLiterals = checkBox_literals->isChecked();
    //bool ignoreIdentifiers = checkBox_identifiers->isChecked();
    bool includeSubdirs = checkBox_subdirection->isChecked();

	//
    // For demonstration, we'll just update the status bar with the parameters chosen.
	//
    QString message = QString("Going to analyze %1 with threshold %2 for %3").arg(directory).arg(threshold).arg(language);
    statusBar()->showMessage(message);

	//
	// Make sure directory exists.
	// We should really never reach there but who knows, even the whale has ale in it. Let's handle this as well.
	//
	if (chdir(directory.toStdString().c_str()) != 0) {
    	QString error_message = QString("The directory \"%1\" does not exist.").arg(directory);
    	QMessageBox::warning(this, "Directory Error", error_message);
    	return;
    }

    //
    // Call the main algorithm now with parameters
    //
    findDuplicates(includeSubdirs, language);
}

//
// Destructor
//
MainWindow::~MainWindow()
{
    delete file_menu;
    delete view_menu;
    delete help_menu;
    delete about_menu;

    delete findings_table_widget;
    delete file_content_text_edit;

    delete root_source_dir_line_edit;
    delete browse_button;

    delete chunks_larger_spin_box;
    delete comboBox_language;
  
    delete checkBox_subdirection;
    delete checkBox_literals;
    delete checkBox_identifiers;
  
    delete files_list_widget;

    delete comboBox_encoding;
    delete open_editor;
    delete go_button;
}

