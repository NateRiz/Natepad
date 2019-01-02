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
    QString mPath;

    void SetState(QString);
    void ParseKeywords(QStringList);

public:
    explicit ColorTheme(QWidget* parent);
    virtual ~ColorTheme();

    QHash<QRegularExpression, QTextCharFormat> getColorMap();
    QString getDefaultColor();
    void RefreshColorMap();
    void SetPath(QString path);
    QHash<QString, QString> GetAvailableThemes();
};

#endif // COLORTHEME_H
