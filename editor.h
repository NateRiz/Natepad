#ifndef EDITOR_H
#define EDITOR_H
#include <QTextEdit>
#include<unordered_map>
#include <string>
#include "filetree.h"
#include "colortheme.h"
#include "syntaxhighlighter.h"

class QShortcut;

class Editor : public QTextEdit
{
private:
    Q_OBJECT
    void PromptForSavePath();
    void PromptForOpenPath();
    void CreateShortcuts();
    int SaveBeforeQuitPrompt();

    std::unordered_map<std::string,QShortcut*> mShortcuts;
    QString mSavePath = "";
    QString mExtension;
    bool mIsSaved;
    ColorTheme* mColorTheme;
    SyntaxHighlighter* mSyntaxHighlighter;

public:
    explicit Editor(QWidget *parent = nullptr);
    virtual ~Editor();
    bool CanExit();
    bool filterEvent(QEvent*);
    QString getSavePath();
    ColorTheme *getColorTheme();
    void RefreshSyntaxHighlighter();
    QString GetExtension();

public slots:
    void Save();
    void SaveAs();
    void Open();
    void Open(QString path);
    void SetUnsaved();
    void SetSaved();
    void CopyPath();
    void NewPage();


};

#endif // DEL_H
