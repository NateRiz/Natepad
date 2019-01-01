#ifndef COLORTHEME_H
#define COLORTHEME_H
#include <fstream>
#include <iostream>
#include <QWidget>
#include <QFile>
#include <QtDebug>
#include <QTextStream>
#include <QHash>
#include <QTextCharFormat>
#include <QRegularExpression>
class ColorTheme : public QWidget
{
private:
    Q_OBJECT
    QHash<QRegularExpression, QTextCharFormat> mColorMap;
    enum eRegexState
    {
        INVALID=0,
        KEYWORDS,
        CONSTANTS,
        LITERALS,
        BASECLASSMETHODS,
        CLASS,
        ARGS,
        COMMENTS,
        DEFAULT
    };
    eRegexState mRegexState;
    QString mDefaultColor;

    void SetState(QString);
    void ParseKeywords(QStringList);


public:
    explicit ColorTheme(QWidget* parent, QString extension);
    virtual ~ColorTheme();

    QHash<QRegularExpression, QTextCharFormat> getColorMap();
    QString getDefaultColor();
};

#endif // COLORTHEME_H
