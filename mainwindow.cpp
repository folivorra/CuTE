#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "codeeditor.h"
#include "settingsdialog.h"
#include "editorsettings.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QKeySequence>
#include <QWheelEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QSettings>
#include <QStringListModel>
#include <QProcess>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    completer(nullptr),
    compilerProcess(new QProcess(this)),
    compilerPath(""),
    dialog(new settingsDialog(this))
{
    ui->setupUi(this);



    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    setCentralWidget(tabWidget);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    statusLabel = new QLabel("Ready");
    statusBar()->addPermanentWidget(statusLabel);

    createMenus();

    QWidget *labelPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(labelPage);
    QLabel *label = new QLabel("Курсовая работа по ООП студента МВС-122, Йай Эмиля", labelPage);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    labelPage->setLayout(layout);

    tabWidget->addTab(labelPage, tr("Welcome Tab"));

    setWindowTitle("СuTE");
        resize(900, 800);

    QStringList wordList = {"auto", "bool", "break", "case", "catch", "char", "class", "const", "constexpr",
                                "continue", "default", "delete", "do", "double", "else", "enum", "explicit",
                                "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline",
                                "int", "long", "mutable", "namespace", "new", "noexcept", "nullptr", "operator",
                                "private", "protected", "public", "register", "reinterpret_cast", "return",
                                "short", "signed", "sizeof", "static", "static_assert", "static_cast",
                                "struct", "switch", "template", "this", "thread_local", "throw", "true", "try",
                                "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
                                "void", "volatile", "wchar_t", "while", "std::string", "std::vector",
                                "std::map", "std::set", "std::list", "std::queue", "std::stack", "std::array",
                                "std::unordered_map", "std::unordered_set", "std::multimap", "std::multiset",
                                "std::cin", "std::cout", "std::cerr", "std::endl", "std::make_pair", "std::sort",
                                "std::reverse", "std::find", "std::max", "std::min", "std::swap", "std::begin",
                                "std::end", "#include", "#include <iostream>", "#include <vector>", "#include <map>",
                                "#include <set>", "#include <list>", "#include <queue>", "#include <stack>",
                                "#include <algorithm>", "#include <functional>", "#include <cmath>",
                                "#include <string>", "#include <thread>", "#include <mutex>", "std::forward",
                                "std::move", "std::unique_ptr", "std::shared_ptr", "std::weak_ptr",
                                "std::function", "std::bind", "std::thread::join", "std::thread::detach",
                                "std::async", "std::future", "std::packaged_task", "std::launch", "std::mutex",
                                "std::lock_guard", "std::unique_lock", "std::condition_variable", "std::atomic",
                                "std::memory_order", "std::chrono::system_clock", "std::chrono::steady_clock",
                                "std::chrono::high_resolution_clock", "std::chrono::duration", "std::chrono::time_point",
                                "std::filesystem::path", "std::filesystem::create_directory", "std::filesystem::exists",
                                "std::filesystem::file_size", "std::random_device", "std::mt19937", "std::uniform_int_distribution",
                                "std::uniform_real_distribution", "std::seed_seq", "std::generate", "std::generate_n", "std::copy",
                                "std::copy_n", "std::copy_if", "std::fill", "std::fill_n", "std::transform", "std::reduce",
                                "std::accumulate", "std::inner_product", "std::adjacent_difference", "std::partial_sum", "std::replace",
                                "std::replace_if", "std::replace_copy", "std::replace_copy_if", "std::iter_swap", "std::iota", "std::count",
                                "std::count_if", "std::search", "std::search_n", "std::mismatch", "std::find_if", "std::find_if_not",
                                "std::find_end", "std::find_first_of", "std::adjacent_find", "std::lower_bound", "std::upper_bound",
                                "std::equal_range", "std::binary_search", "std::merge", "std::inplace_merge", "std::includes",
                                "std::set_difference", "std::set_intersection", "std::set_union", "std::set_symmetric_difference",
                                "std::map", "std::multimap", "std::unordered_map", "std::unordered_multimap", "std::initializer_list",
                                "std::tuple", "std::pair", "std::bitset", "std::hash", "std::priority_queue", "std::deque", "std::forward_list",
                                "std::optional", "std::variant", "std::any", "std::nth_element", "std::partial_sort", "std::partial_sort_copy",
                                "std::stable_sort", "std::stable_partition", "std::partition", "std::partition_copy", "std::partition_point",
                                "std::is_partitioned", "std::is_sorted", "std::is_sorted_until", "std::all_of", "std::any_of", "std::none_of",
                                "std::next_permutation", "std::prev_permutation", "std::sample", "std::clamp", "std::exchange", "std::as_const",
                                "std::make_optional", "std::make_variant", "std::apply", "std::invoke", "std::visit", "std::holds_alternative",
                                "std::get_if", "std::get", "std::tie", "std::ignore", "forward", "move", "unique_ptr", "shared_ptr", "weak_ptr",
                                "function", "bind", "thread::join", "thread::detach", "async", "future", "packaged_task", "launch", "mutex",
                                "lock_guard", "unique_lock", "condition_variable", "atomic", "memory_order", "chrono::system_clock",
                                "chrono::steady_clock", "chrono::high_resolution_clock", "chrono::duration", "chrono::time_point",
                                "filesystem::path", "filesystem::create_directory", "filesystem::exists", "filesystem::file_size",
                                "random_device", "mt19937", "uniform_int_distribution", "uniform_real_distribution", "seed_seq", "generate",
                                "generate_n", "copy", "copy_n", "copy_if", "fill", "fill_n", "transform", "reduce", "accumulate",
                                "inner_product", "adjacent_difference", "partial_sum", "replace", "replace_if", "replace_copy", "replace_copy_if",
                                "iter_swap", "iota", "count", "count_if", "search", "search_n", "mismatch", "find_if", "find_if_not", "find_end",
                                "find_first_of", "adjacent_find", "lower_bound", "upper_bound", "equal_range", "binary_search", "merge",
                                "inplace_merge", "includes", "set_difference", "set_intersection", "set_union", "set_symmetric_difference",
                                "map", "multimap", "unordered_map", "unordered_multimap", "initializer_list", "tuple", "pair", "bitset", "hash",
                                "priority_queue", "deque", "forward_list", "optional", "variant", "any", "nth_element", "partial_sort",
                                "partial_sort_copy", "stable_sort", "stable_partition", "partition", "partition_copy", "partition_point",
                                "is_partitioned", "is_sorted", "is_sorted_until", "all_of", "any_of", "none_of", "next_permutation",
                                "prev_permutation", "sample", "clamp", "exchange", "as_const", "make_optional", "make_variant", "apply",
                                "invoke", "visit", "holds_alternative", "get_if", "get", "tie", "ignore", "<iostream>", "<vector>", "<map>",
                                "<set>", "<list>", "<queue>", "<stack>", "<algorithm>", "<functional>", "<cmath>", "<string>", "<thread>",
                                "<mutex>", "<fstream>", "<sstream>", "<iomanip>", "<iterator>", "<type_traits>", "<numeric>", "<utility>",
                                "<memory>", "<chrono>", "<random>", "<limits>", "<array>", "<tuple>", "<bitset>", "<forward_list>"
};

    completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    connect(dialog, &settingsDialog::pathChanged, this, &MainWindow::updateCompilerPath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    newAct = new QAction(tr("&Create new file..."), this);
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAct);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAct);
    saveAct->setEnabled(false);

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFileAs);
    fileMenu->addAction(saveAsAct);
    saveAsAct->setEnabled(false);

    closeAllTab = new QAction(tr("Close all &tabs"), this);
    closeAllTab->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_W));
    connect(closeAllTab, &QAction::triggered, this, &MainWindow::closeAllT);
    fileMenu->addAction(closeAllTab);

    fileMenu = menuBar()->addMenu(tr("&Tools"));

    QAction *settingsAct = new QAction(tr("&Settings"), this);
    connect(settingsAct, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    fileMenu->addAction(settingsAct);

    QAction *compileAct = new QAction(tr("Compile and &Run"), this);
    compileAct->setShortcut(QKeySequence(Qt::Key_F5));
    connect(compileAct, &QAction::triggered, this, &MainWindow::compileCode);
    fileMenu->addAction(compileAct);
}

void MainWindow::newFile() {
    closeWelcomeTab();
    CodeEditor *editor = new CodeEditor;

    connect(editor, &CodeEditor::cursorPositionUpdated, this, &MainWindow::updateStatusBar);
    connect(editor, &CodeEditor::documentModified, this, &MainWindow::onDocumentModified);
    editor->setCompleter(completer);

    int newIndex = tabWidget->addTab(editor, tr("Untitled"));
    tabWidget->setCurrentIndex(newIndex);

    fileNames.append(QString());
    saveAct->setEnabled(false);
    saveAsAct->setEnabled(true);

    editor->isModified = false;
    emit editor->documentModified(false);

    updateZoomLevel(100);

    currentFilePath.clear();
}

void MainWindow::openFile()
{
    closeWelcomeTab();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "",
                                                    tr("C++ Files (*.cpp *.h);;All Files (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            CodeEditor *editor = new CodeEditor;
            connect(editor, &CodeEditor::cursorPositionUpdated, this, &MainWindow::updateStatusBar);
            connect(editor, &CodeEditor::documentModified, this, &MainWindow::onDocumentModified);
            editor->setCompleter(completer);
            editor->setPlainText(in.readAll());
            int newIndex = tabWidget->addTab(editor, QFileInfo(fileName).fileName());
            tabWidget->setCurrentIndex(newIndex);

            fileNames.append(fileName);

            editor->isModified = false;
            emit editor->documentModified(false);
            currentFilePath = fileName;  // Обновить текущий путь к файлу
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Cannot open file %1:\n%2.")
                                                        .arg(fileName, file.errorString()));
        }
    }
}

void MainWindow::saveFile() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex == -1) {
        return;
    }

    CodeEditor *editor = qobject_cast<CodeEditor *>(tabWidget->widget(currentIndex));
    if (!editor) {
        return;
    }

    QString tabText = tabWidget->tabText(currentIndex);

    // Проверяем, если вкладка имеет название "Untitled*" или "Untitled"
    if (tabText.startsWith("Untitled")) {
        saveFileAs(); // Перенаправляем на saveFileAs, если файл имеет название "Untitled"
    } else {
        QString currentFile = fileNames[currentIndex];
        if (currentFile.isEmpty()) {
            saveFileAs(); // Перенаправляем на saveFileAs, если файл не имеет имени
        } else {
            if (saveToFile(currentFile, currentIndex)) {
                editor->isModified = false;
                emit editor->documentModified(false);
                currentFilePath = currentFile;  // Обновить текущий путь к файлу
            }
        }
    }
}

void MainWindow::saveFileAs() {
    int currentIndex = tabWidget->currentIndex();
    if (currentIndex == -1) {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), "",
                                                    tr("C++ Files (*.cpp *.h);;All Files (*)"));
    if (!fileName.isEmpty()) {
        if (saveToFile(fileName, currentIndex)) {
            if (currentIndex < fileNames.size()) {
                fileNames[currentIndex] = fileName;
            } else {
                fileNames.append(fileName);
            }
            QString tabText = QFileInfo(fileName).fileName();
            tabWidget->setTabText(currentIndex, tabText);

            CodeEditor *editor = qobject_cast<CodeEditor *>(tabWidget->widget(currentIndex));
            if (editor) {
                editor->isModified = false;
                emit editor->documentModified(false);
            }
            currentFilePath = fileName;
        }
    }
}

bool MainWindow::saveToFile(const QString &fileName, int index) {
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("File name is empty."));
        return false;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot save file %1:\n%2.")
                                                    .arg(fileName, file.errorString()));
        return false;
    }

    QTextStream out(&file);
    CodeEditor *editor = qobject_cast<CodeEditor *>(tabWidget->widget(index));
    if (!editor) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to get the editor widget."));
        return false;
    }

    out << editor->toPlainText();

    // Проверяем, что индекс файла существует в списке fileNames
    if (index < fileNames.size()) {
        fileNames[index] = fileName;
    } else {
        fileNames.append(fileName);
    }

    return true;
}


void MainWindow::closeAllT()
{
    fileNames.clear(); // Clear fileNames to prevent out of range issues
    int i = 0;
    while (tabWidget->count() != 0)
    {
        QWidget *widget = tabWidget->widget(i);
        delete widget;
    }
}

void MainWindow::closeTab(int index) {
    QWidget *widget = tabWidget->widget(index);
    if (widget) {
        tabWidget->removeTab(index);
        delete widget;
        if (index < fileNames.size()) {
            fileNames.removeAt(index);
        }
    }
}


void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        CodeEditor *editor = qobject_cast<CodeEditor *>(tabWidget->currentWidget());
        if (editor) {
            int delta = event->angleDelta().y();
            int newFontSize = qMax(1, editor->font().pointSize() + delta / 120);
            QFont font = editor->font();
            font.setPointSize(newFontSize);
            editor->setFont(font);
            updateZoomLevel(newFontSize * 100 / 14);
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void MainWindow::onDocumentModified(bool modified) {
    int index = tabWidget->currentIndex();
    QString title = tabWidget->tabText(index);
    if (modified) {
        if (!title.endsWith("*")) {
            tabWidget->setTabText(index, title + "*");
        }
        saveAct->setEnabled(true);
    } else {
        if (title.endsWith("*")) {
            tabWidget->setTabText(index, title.chopped(1));
        }
        saveAct->setEnabled(false);
    }
}

void MainWindow::updateStatusBar(int line, int col) {
    QString statusMessage = QString("Line: %1, Column: %2").arg(line).arg(col);
    statusLabel->setText(statusMessage);
}

void MainWindow::updateZoomLevel(int zoomLevel) {
    statusLabel->setText(QString("Zoom: %1%").arg(zoomLevel));
}

void MainWindow::showSettingsDialog() {
    if (dialog->exec() == QDialog::Accepted)
    {
        EditorSettings::setFont(dialog->currentFont());
        EditorSettings::setFontSize(dialog->currentFontSize());

        updateAllEditors();
    }
}

void MainWindow::updateAllEditors() {
    for (int i = 0; i < tabWidget->count(); ++i) {
        CodeEditor *editor = qobject_cast<CodeEditor *>(tabWidget->widget(i));
        if (editor) {
            editor->updateSettings();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    settingsDialog::clearSettings();
    QMainWindow::closeEvent(event); // Вызов базового метода для завершения закрытия
}

void MainWindow::closeWelcomeTab() {
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == "Welcome Tab") {
            QWidget *widget = tabWidget->widget(i);
            tabWidget->removeTab(i);
            delete widget;
            break;
        }
    }
}

void MainWindow::compileCode() {
    if (!currentFilePath.isEmpty()) {
        if (compilerPath.size() == 0) {
            QMessageBox::warning(this, tr("Error"), tr("Compiler path is not set."));
            return;
        }
        // Путь к исполняемому файлу
        QString outputExecutable = QFileInfo(currentFilePath).absolutePath() + "/output.exe";
        // Аргументы компилятора
        QStringList arguments;
        arguments << QDir::toNativeSeparators(currentFilePath) << "-o" << QDir::toNativeSeparators(outputExecutable);
        QString compileCommand = QString("%1 %2").arg(compilerPath, arguments.join(" "));
        QString runCommand = QString("%1").arg(outputExecutable);
        QString fullCommand = QString("%1 && %2 && pause").arg(compileCommand, runCommand);

        QStringList cmdArguments;
        cmdArguments << "/C" << "start" << "cmd.exe" << "/K" << fullCommand;

        qDebug() << "Starting compilation with command:" << cmdArguments.join(" ");

        // Открытие нового окна терминала и выполнение команды
        QProcess::startDetached("cmd.exe", cmdArguments);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("No file to compile."));
    }
}


void MainWindow::updateCompilerPath(const QString &newPath) {
    compilerPath = newPath;
}


void MainWindow::handleCompilationFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray standardOutput = process->readAllStandardOutput();
        QByteArray standardError = process->readAllStandardError();

        qDebug() << "Standard Output:" << standardOutput;
        qDebug() << "Standard Error:" << standardError;

        if (exitStatus == QProcess::NormalExit) {
            if (exitCode == 0) {
                QMessageBox::information(this, tr("Compilation"), tr("Compilation successful.\n\nOutput:\n") + standardOutput);
            } else {
                QMessageBox::critical(this, tr("Compilation Error"), tr("Compilation failed with exit code %1.\n\nError:\n%2").arg(exitCode).arg(QString(standardError)));
            }
        } else {
            QMessageBox::critical(this, tr("Compilation Error"), tr("The compilation process crashed.\n\nError:\n%1").arg(QString(standardError)));
        }
        process->deleteLater();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to cast sender to QProcess."));
    }
}
