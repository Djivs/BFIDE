#include "bfhighlighter.h"

BFHighlighter::BFHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    setUpColorScheme(Qt::black, QColor(255, 51, 51), Qt::blue, Qt::darkGreen);
    HighlightingRule rule;

    bracketsFormat.setForeground(*bracketsColor);
    rule.pattern = QRegularExpression(QStringLiteral("[\\[\\]]"));
    rule.format = bracketsFormat;
    highlightingRules.append(rule);

    movesFormat.setForeground(*movesColor);
    rule.pattern = QRegularExpression(QStringLiteral("[<>]"));
    rule.format = movesFormat;
    highlightingRules.append(rule);

    mathFormat.setForeground(*mathColor);
    rule.pattern = QRegularExpression(QStringLiteral("[+-]"));
    rule.format = mathFormat;
    highlightingRules.append(rule);

    consoleFormat.setForeground(*consoleColor);
    rule.pattern = QRegularExpression(QStringLiteral("[.,]"));
    rule.format = consoleFormat;
    highlightingRules.append(rule);
}

void BFHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
    }
}

void BFHighlighter::setUpColorScheme(QColor brackets, QColor moves, QColor math, QColor console) {
    bracketsColor = new QColor(brackets);
    movesColor = new QColor(moves);
    mathColor = new QColor(math);
    consoleColor = new QColor(console);
}
