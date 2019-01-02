#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QFile>

class Settings : public QWidget
{
    Q_OBJECT
    void LoadSettings();
    QFile mSettings;

    enum eSettingsState{
        INVALID=0,
        THEMES
    };
    eSettingsState mState;
    QHash<QString, QString> mThemes;
    QString mThemeDirectory;

    void SetLoadState(QString line);

public:
    explicit Settings(QWidget *parent = nullptr);
    QString GetTheme(QString);
    void WriteTheme(QString extension, QString themeName);
    QString GetThemeDirectory();

};

#endif // SETTINGS_H
