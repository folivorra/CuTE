#include "codeeditor.h"
#include "editorsettings.h"
#include "cppsyntaxhighlighter.h"
#include <QPainter>
#include <QTextBlock>
#include <QPlainTextEdit>
#include <QAbstractItemView>
#include <QScrollBar>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent), c(nullptr), highlighter(nullptr) {
    lineNumberArea = new LineNumberArea(this);

    this->setFont(EditorSettings::font());

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // Всегда показывать горизонтальный скроллбар
    setWordWrapMode(QTextOption::NoWrap);  // Отключить перенос слов

    setTabStopDistance(QFontMetricsF(font()).horizontalAdvance(' ') * 4); //tab

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::updateCursorPosition);

    updateLineNumberAreaWidth(0);

    isModified = false;

    connect(this, &QPlainTextEdit::textChanged, [this]() {
        if (!isModified) {
            isModified = true;
            emit documentModified(true);  // Испускается сигнал, когда текст изменен
        }
    });

    highlighter = new cppSyntaxHighlighter(this->document());
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 4; // Максимальное количество цифр
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);  // Заливка фона области номеров строк

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);

            // Установка цвета номера текущей строки
            if (block == textCursor().block()) {
                painter.setPen(Qt::black);  // Цвет номера строки для активной строки
            } else {
                painter.setPen(Qt::darkGray);  // Цвет номера строки для всех остальных строк
            }

            // Рисование номера строки
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    codeEditor->lineNumberAreaPaintEvent(event);
}

QPair<int, int> CodeEditor::cursorPosition() const {
    QTextCursor cursor = textCursor();
    return qMakePair(cursor.blockNumber() + 1, cursor.columnNumber() + 1);
}

void CodeEditor::updateCursorPosition() {
    QTextCursor cursor = textCursor();
    int line = cursor.blockNumber() + 1;  // строки и столбцы обычно считаются с 1 в пользовательских интерфейсах
    int col = cursor.columnNumber() + 1;
    emit cursorPositionUpdated(line, col);
}

void CodeEditor::updateSettings() {
    this->setFont(EditorSettings::font());
}

void CodeEditor::setCompleter(QCompleter *completer) {
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &CodeEditor::insertCompletion);
}

QCompleter *CodeEditor::completer() const {
    return c;
}

void CodeEditor::insertCompletion(const QString &completion) {
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const {
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString word = tc.selectedText();

    // Добавление символов, которые могут быть частью слова
    static const QString additionalSymbols = "_:#<>/.";
    int pos = tc.position() - word.length() - 1;
    while (pos >= 0) {
        QChar c = document()->characterAt(pos);
        if (!c.isLetterOrNumber() && !additionalSymbols.contains(c))
            break;
        word.prepend(c);
        --pos;
    }

    // Проверка на дополнительные символы впереди
    pos = tc.position();
    while (pos < document()->characterCount()) {
        QChar c = document()->characterAt(pos);
        if (!c.isLetterOrNumber() && !additionalSymbols.contains(c))
            break;
        word.append(c);
        ++pos;
    }

    return word;
}

void CodeEditor::keyPressEvent(QKeyEvent *e) {
    if (c && c->popup()->isVisible()) {
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            insertCompletion(c->currentCompletion());
            return;
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }

    QTextCursor cursor = textCursor();
    const int tabSize = 4; // Размер табуляции (может быть настроен)

    if (e->key() == Qt::Key_Tab) {
        if (cursor.hasSelection()) {
            int start = cursor.selectionStart();
            int end = cursor.selectionEnd();

            cursor.beginEditBlock();
            cursor.setPosition(start, QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::StartOfLine);

            while (cursor.position() <= end) {
                cursor.insertText("\t"); // Вставка символа табуляции
                cursor.movePosition(QTextCursor::Down);
                end += 1; // Сдвигаем конец выделения на 1 символ
            }
            cursor.endEditBlock();
        } else {
            cursor.insertText("\t"); // Вставка символа табуляции для одной строки
        }
        return;
    } else if (e->key() == Qt::Key_Backtab) {
        // Обработка Shift+Tab для удаления отступа
        if (cursor.hasSelection()) {
            int start = cursor.selectionStart();
            int end = cursor.selectionEnd();

            cursor.beginEditBlock();
            cursor.setPosition(start, QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::StartOfLine);

            while (cursor.position() <= end) {
                for (int i = 0; i < tabSize; ++i) {
                    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                    if (cursor.selectedText() == " " || cursor.selectedText() == "\t") {
                        cursor.removeSelectedText();
                        end -= 1; // Сдвигаем конец выделения на 1 символ
                        break;
                    }
                }
                cursor.movePosition(QTextCursor::Down);
            }
            cursor.endEditBlock();
        } else {
            cursor.movePosition(QTextCursor::StartOfLine);
            for (int i = 0; i < tabSize; ++i) {
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                if (cursor.selectedText() == " " || cursor.selectedText() == "\t") {
                    cursor.removeSelectedText();
                    break;
                }
            }
        }
        return;
    } else if (e->key() == Qt::Key_BraceLeft) {
        // Обработка ввода открывающей фигурной скобки
        int currentIndentationLevel = calculateCurrentIndentation(cursor);
        QPlainTextEdit::keyPressEvent(e);  // Сначала вставляем скобку
        cursor.insertText("\n" + QString("\t").repeated(currentIndentationLevel + 1)); // Отступ для внутреннего блока
        cursor.insertText("\n" + QString("\t").repeated(currentIndentationLevel) + "}"); // Закрывающая скобка с отступом
        cursor.movePosition(QTextCursor::Up);
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        return;
    } else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        // Автоматический отступ при нажатии Enter
        int currentIndentationLevel = calculateCurrentIndentation(cursor);
        QString trimmedLine = cursor.block().text().trimmed();
        bool increaseIndent = trimmedLine.endsWith("{") || trimmedLine.endsWith(":");
        QPlainTextEdit::keyPressEvent(e);  // Перевод строки
        if (increaseIndent) {
            currentIndentationLevel++;
        }
        cursor.insertText(QString("\t").repeated(currentIndentationLevel));
        setTextCursor(cursor);
        return;
    }

    QPlainTextEdit::keyPressEvent(e); // Обработка других клавиш

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!hasModifier && (e->text().isEmpty() || completionPrefix.length() < 3
                         || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }

    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}


int CodeEditor::calculateCurrentIndentation(QTextCursor cursor) {
    cursor.movePosition(QTextCursor::StartOfLine);
    QString lineText = cursor.block().text();
    int indentationLevel = 0;
    const int tabSize = 4; // Размер табуляции (может быть настроен)

    for (QChar c : lineText) {
        if (c == ' ') {
            indentationLevel++;
        } else if (c == '\t') {
            indentationLevel += tabSize;
        } else {
            break;
        }
    }
    return indentationLevel / tabSize;
}
