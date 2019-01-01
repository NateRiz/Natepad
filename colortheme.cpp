#include "colortheme.h"
#include "editor.h"

ColorTheme::ColorTheme(QWidget *parent):
    QWidget(parent)
{
    RefreshColorMap();
}

void ColorTheme::SetState(QString line)
{
    mRegexState = INVALID;
    if(line == QString("!KEYWORDS"))
        mRegexState = KEYWORDS;
    if(line == QString("!CONSTANTS"))
        mRegexState = CONSTANTS;
    if(line == QString("!LITERALS"))
        mRegexState = LITERALS;
    if(line == QString("!BASECLASSMETHODS"))
        mRegexState = BASECLASSMETHODS;
    if(line == QString("!CLASS"))
        mRegexState = CLASS;
    if(line == QString("!ARGS"))
        mRegexState = ARGS;
    if(line == QString("!COMMENTS"))
        mRegexState = COMMENTS;
    if(line == QString("!DEFAULT"))
        mRegexState = DEFAULT;
}


void ColorTheme::ParseKeywords(QStringList line)
{
    for (QString keyword : line[0].split(" "))
    {
        keyword = QString("\\b") + keyword + QString("\\b");
        QString hexColor = line[1].replace(" ","");
        QTextCharFormat format;
        format.setForeground(QBrush(QColor(hexColor)));
        mColorMap[QRegularExpression(keyword)]=format;
    }
}

ColorTheme::~ColorTheme(){}

QHash<QRegularExpression, QTextCharFormat> ColorTheme::getColorMap()
{
    return mColorMap;
}

QString ColorTheme::getDefaultColor()
{
    return mDefaultColor;
}

void ColorTheme::RefreshColorMap()
{
    mColorMap= QHash<QRegularExpression, QTextCharFormat>();
    mRegexState = INVALID;

    QString savePath = qobject_cast<Editor*>(parent())->getSavePath();
    QString extension = QFileInfo(savePath).suffix();
    QString path = QString(":/Themes/") + extension + QString(".cfg");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << QString("No theme found for: ." + extension);
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(line == QString("")){
            continue;
        }
        if(line[0]=="!"){
            SetState(line);
            continue;
        }
        QTextCharFormat format;
        switch(mRegexState)
        {
        case INVALID:
            break;
        case KEYWORDS:
            ParseKeywords(line.split(":"));
            break;
        case CONSTANTS:
            format.setForeground(QBrush(QColor(line)));
            mColorMap[QRegularExpression("[+-]?[0-9]{1,9}(?:\\.[0-9]{1,2})?")] = format;
            break;
        case LITERALS:
            format.setForeground(QBrush(QColor(line)));
            mColorMap[QRegularExpression(R"#(("\s*([^"]*)\s*")|('\s*([^']*)\s*'))#", QRegularExpression::DotMatchesEverythingOption)] = format;
            //New lines do not work
            break;
        case CLASS:
            //int() str().. creating an object
            break;
        case BASECLASSMETHODS:
            format.setForeground(QBrush(QColor(line)));
            mColorMap[QRegularExpression(R"#((__)(.*?)(__))#")] = format;
            break;
        case ARGS:
            format.setForeground(QBrush(QColor(line)));
            mColorMap[QRegularExpression(R"#((?<=\().*?(?=\s*\)[^)]*$))#")] = format;
            break;
        case COMMENTS:
            format.setForeground(QBrush(QColor(line)));
            mColorMap[QRegularExpression(R"!(#.*)!")] = format;
            break;
        case DEFAULT:
            mDefaultColor = line;
            break;
        }
    }
}
