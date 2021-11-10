#ifndef BFHIGHLIGHTER_H
#define BFHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

#include <QObject>

class BFHighlighter : public QSyntaxHighlighter
{
public:
    BFHighlighter(QTextDocument *parent = NULL);
protected:
    void highlightBlock(const QString &text) override;
private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QColor *bracketsColor, *movesColor, *mathColor, *consoleColor;

    QTextCharFormat bracketsFormat;
    QTextCharFormat movesFormat;
    QTextCharFormat mathFormat;
    QTextCharFormat consoleFormat;

    void setUpColorScheme(QColor brackets, QColor moves, QColor math, QColor console);
};

#endif // BFHIGHLIGHTER_H
