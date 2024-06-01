#ifndef CPPSYNTAXHIGHLIGHTER_H
#define CPPSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class cppSyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    cppSyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat preprocessorFormat;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
};

#endif // CPPSYNTAXHIGHLIGHTER_H
