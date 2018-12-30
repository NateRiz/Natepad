#ifndef COLORTHEME_H
#define COLORTHEME_H
#include <fstream>
#include<iostream>
#include <QWidget>
#include <QFile>
#include <QtDebug>
#include <QTextStream>
#include <unordered_map>
class ColorTheme : public QWidget
{
private:
    Q_OBJECT
    std::unordered_map<std::string, std::string> mColorMap;

public:
    explicit ColorTheme(QWidget* parent, QString extension):
        QWidget(parent)
    {
        QString path = ":/Themes/" + extension + QString(".cfg");
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug("No theme found for: ." + extension.toLatin1());
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QStringList line = in.readLine().replace(" ","").split(":");

            if(line.size()!=2)
            {
                qDebug("Could not parse line from theme file:\n" + line.join("").toLatin1());
            }
            else
            {
                mColorMap[line[0].toStdString()]=line[1].toStdString();
            }
        }

    }

    std::string ParseWord(QString word)
    {
        return mColorMap["Text"];
    }

    virtual ~ColorTheme(){}
};

#endif // COLORTHEME_H
