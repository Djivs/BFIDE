#include "bfhighlighter.h"

BFHighlighter::BFHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
   HighlightingRule rule;

   bracketsFormat.setForeground(Qt::black);
   rule.pattern = QRegularExpression(QStringLiteral("[\\[\\]]"));
   rule.format = bracketsFormat;
   highlightingRules.append(rule);

   movesFormat.setForeground(QColor(255, 51, 51));
   rule.pattern = QRegularExpression(QStringLiteral("[<>]"));
   rule.format = movesFormat;
   highlightingRules.append(rule);

   mathFormat.setForeground(Qt::blue);
   rule.pattern = QRegularExpression(QStringLiteral("[+-]"));
   rule.format = mathFormat;
   highlightingRules.append(rule);

   consoleFormat.setForeground(Qt::darkGreen);
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
