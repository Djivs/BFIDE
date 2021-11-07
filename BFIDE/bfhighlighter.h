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

    QTextCharFormat bracketsFormat;
    QTextCharFormat movesFormat;
    QTextCharFormat mathFormat;
    QTextCharFormat consoleFormat;
};

#endif // BFHIGHLIGHTER_H
