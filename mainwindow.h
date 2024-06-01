#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"
#include <QMainWindow>
#include <QLabel>
#include <QCompleter>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void closeAllT();
    void closeTab(int indexTab);
    void wheelEvent(QWheelEvent *event);
    void closeEvent(QCloseEvent *event);
    void onDocumentModified(bool modified);
    void updateStatusBar(int line, int col);
    void updateZoomLevel(int zoomLevel);
    void showSettingsDialog();
    void updateAllEditors();
    void closeWelcomeTab();
    void compileCode();
    void handleCompilationFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void updateCompilerPath(const QString &newPath);

private:
    void createMenus();
    bool saveToFile(const QString &fileName, int index);  // Метод для сохранения в файл

    QString getCurrentFileName() const;

    Ui::MainWindow *ui;

    QTabWidget *tabWidget; //для вкладок

    QMenu *fileMenu;

    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *newAct;
    QAction *closeAllTab;

    QLabel *statusLabel;  // Лейбл для отображения статуса в правой части статусной строки

    QVector<QString> fileNames;  // для сохранения имен файлов

    QCompleter *completer;

    QProcess *compilerProcess;
    QString currentFilePath;
    QString compilerPath;
    settingsDialog *dialog;
};
#endif // MAINWINDOW_H
