#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QWidget>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QString>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QTextEdit>
#include "colortheme.h"


class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QTextDocument* parent = nullptr, ColorTheme* colorTheme = nullptr);
    virtual ~SyntaxHighlighter();
    void refresh();

protected:
    void highlightBlock(const QString &text) override;

private:
    ColorTheme* mColorTheme;
};

#endif // SYNTAXHIGHLIGHTER_H
