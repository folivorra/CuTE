#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>
#include <QFileDialog>

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    connect(ui->ButtonApply, &QPushButton::clicked, this, &settingsDialog::applySettings);
    connect(ui->ButtonCancel, &QPushButton::clicked, this, &settingsDialog::cancelSettings);
    connect(ui->buttonBrowse, &QPushButton::clicked, this, &settingsDialog::browse);

    setWindowTitle("Settings");

    loadSettings();
}

settingsDialog::~settingsDialog() {
    delete ui;
}

void settingsDialog::applySettings() {
    emit fontChanged(ui->fontBox->currentFont());
    emit fontSizeChanged(ui->sizeFBox->value());
    emit pathChanged(ui->linePath->text()); // Испускаем сигнал с новым путем

    saveSettings();
    accept();
}

void settingsDialog::cancelSettings() {
    reject();
}

QFont settingsDialog::currentFont() const {
    return ui->fontBox->currentFont();
}

int settingsDialog::currentFontSize() const {
    return ui->sizeFBox->value();
}

QString settingsDialog::currentCompilerPath() const {
    return ui->linePath->text();
}

void settingsDialog::saveSettings() {
    QSettings settings("YourCompany", "YourApp");
    settings.setValue("font", ui->fontBox->currentFont().family());
    settings.setValue("fontSize", ui->sizeFBox->value());
    settings.setValue("pathToCompiler", ui->linePath->text());
}

void settingsDialog::loadSettings() {
    QSettings settings("YourCompany", "YourApp");
    QString fontFamily = settings.value("font", "Cascadia Code").toString();
    int fontSize = settings.value("fontSize", 14).toInt();
    QString compilerPath = settings.value("pathToCompiler", "").toString();

    ui->fontBox->setCurrentFont(QFont(fontFamily));
    ui->sizeFBox->setValue(fontSize);
    ui->linePath->setText(compilerPath);
}

void settingsDialog::clearSettings() {
    QSettings settings("YourCompany", "YourApp");
    settings.clear();
}

void settingsDialog::browse() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Compiler g++"), "",
                                                    tr("Executables (*.exe);;All Files (*)"));

    if (!filePath.isEmpty()) {
        ui->linePath->setText(filePath);
    }
}

