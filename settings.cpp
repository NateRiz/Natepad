#include "settings.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>


Settings::Settings(QWidget *parent) :
    QWidget(parent)
{
    QDir cwd(QCoreApplication::applicationDirPath());
    cwd.cd("../Natepad");
    mSettings.setFileName(cwd.path() + "/settings.cfg");
    cwd.cd("Themes");
    mThemeDirectory = cwd.path();
    LoadSettings();
}

QString Settings::GetTheme(QString extension)
{
    return mThemes[extension];
}

QString Settings::GetThemeDirectory()
{
    return mThemeDirectory;
}

void Settings::LoadSettings()
{
    mState = INVALID;
    mSettings.open(QFile::ReadWrite | QFile::Text);
    QTextStream in(&mSettings);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line[0] == QString("["))
        {
            SetLoadState(line);
            continue;
        }
        switch(mState)
        {
        case INVALID:
            break;

        case THEMES:
            line = line.replace(" ","");
            QStringList theme = line.split(":");
            if(theme.size()!=2){
                continue;
            }
            mThemes[theme[0]]=theme[1];
        }
    }
    mSettings.close();
}

void Settings::WriteTheme(QString extension, QString themeName)
{
    //TODO
    //Check if file exists - if not create.
    mSettings.open(QFile::ReadWrite | QFile::Text);
    QTextStream in(&mSettings);
    QStringList settings = in.readAll().split("\n");
    settings.removeAll(QString(""));

    bool inThemes = false;
    for(int s = 0; s < settings.size(); ++s)
    {
        if(settings[s][0]=="["){
            inThemes =  false;
        }
        //Falthrough if it is a theme - otherwise stay false
        if(settings[s].toLower() == "[themes]")
        {
            inThemes = true;
            ++s;
            settings.insert(s, extension+" : "+themeName);
        }

        else if(inThemes)
        {
            QString currentExtension = settings[s];
            currentExtension.replace(" ","");
            if(currentExtension.split(":")[0] == extension){
                settings.removeAt(s);
            }
        }
    }

    mSettings.seek(0);
    mSettings.write(settings.join("\n").toUtf8());
    mSettings.close();

    mThemes[extension]=themeName;
}

void Settings::SetLoadState(QString line)
{
    mState = INVALID;
    if(line.toLower()==QString("[themes]")){
        mState = THEMES;
    }
}
