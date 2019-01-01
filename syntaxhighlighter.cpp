#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, ColorTheme* colorTheme):
    QSyntaxHighlighter(parent),
    mColorTheme(colorTheme)
{


}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    for (QRegularExpression keyword : mColorTheme->getColorMap().keys())
    {
        QTextCharFormat format = mColorTheme->getColorMap()[keyword];
        QRegularExpressionMatchIterator matchIterator = keyword.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), format);
        }
    }

}

SyntaxHighlighter::~SyntaxHighlighter()
{
}

