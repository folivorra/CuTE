#include "cppsyntaxhighlighter.h"
#include <QTextCharFormat>
#include <QBrush>

cppSyntaxHighlighter::cppSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {

    HighlightingRule rule;

    // Ключевые слова
    keywordFormat.setForeground(QColor(0x000080)); // Dark blue
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns = {
        "\\bchar\\b", "\\bclass\\b", "\\bconst\\b", "\\bdouble\\b", "\\benum\\b",
        "\\bexplicit\\b", "\\bfriend\\b", "\\binline\\b", "\\bint\\b", "\\blong\\b",
        "\\bnamespace\\b", "\\boperator\\b", "\\bprivate\\b", "\\bprotected\\b", "\\bpublic\\b",
        "\\bshort\\b", "\\bsignals\\b", "\\bsigned\\b", "\\bslots\\b", "\\bstatic\\b",
        "\\bstruct\\b", "\\btemplate\\b", "\\btypedef\\b", "\\btypename\\b", "\\bunion\\b",
        "\\bunsigned\\b", "\\bvirtual\\b", "\\bvoid\\b", "\\bvolatile\\b", "\\bbool\\b",
        "\\btrue\\b", "\\bfalse\\b", "\\bnullptr\\b", "\\busing\\b", "\\btry\\b", "\\bcatch\\b",
        "\\bthrow\\b"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Классы
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(QColor(0x800080)); // Purple
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Однострочные комментарии
    singleLineCommentFormat.setForeground(QColor(0x008000)); // Green
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Многострочные комментарии
    multiLineCommentFormat.setForeground(QColor(0x008000)); // Green

    // Строковые литералы
    quotationFormat.setForeground(QColor(0xA52A2A)); // Brown
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Символьные литералы
    QTextCharFormat charLiteralFormat;
    charLiteralFormat.setForeground(QColor(0xA52A2A)); // Brown
    rule.pattern = QRegularExpression("'.'");
    rule.format = charLiteralFormat;
    highlightingRules.append(rule);

    // Функции
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(0x0000FF)); // Blue
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Препроцессорные директивы
    preprocessorFormat.setForeground(QColor(0x800000)); // Maroon
    preprocessorFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("#[^\n]*");
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);

    // Числовые литералы
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor(0x2E8B57)); // SeaGreen
    rule.pattern = QRegularExpression("\\b[0-9]+(\\.[0-9]+)?\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Регулярные выражения для многострочных комментариев
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void cppSyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1) {
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
