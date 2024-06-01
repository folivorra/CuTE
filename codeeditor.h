#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QFont>
#include <QCompleter>
#include "cppsyntaxhighlighter.h"

class LineNumberArea;
class cppSyntaxHighlighter;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    bool isModified;
    QPair<int, int> cursorPosition() const;
    void updateSettings();
    void setCompleter(QCompleter *completer);
    QCompleter *completer() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
    void updateCursorPosition();
    void insertCompletion(const QString &completion);

private:
    QWidget *lineNumberArea;
    QString textUnderCursor() const;

    QCompleter *c;
    cppSyntaxHighlighter *highlighter;

    int calculateCurrentIndentation(QTextCursor cursor);

signals:
    void documentModified(bool modified);
    void cursorPositionUpdated(int line, int col);
};

class LineNumberArea : public QWidget {
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
