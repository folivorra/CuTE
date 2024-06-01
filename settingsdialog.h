#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFontComboBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QDir>

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = nullptr);
    ~settingsDialog();

    QFont currentFont() const;
    int currentFontSize() const;
    QString currentCompilerPath() const;

    void saveSettings();
    void loadSettings();
    static void clearSettings();

signals:
    void fontChanged(const QFont &newFont);
    void fontSizeChanged(int newSize);
    void pathChanged(const QString &newPath);

private slots:
    void applySettings();
    void cancelSettings();
    void browse();

private:
    Ui::settingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
