#include "editor.h"
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <iostream>
#include <fstream>
#include <QShortcut>
#include <QMessageBox>
#include "mainwindow.h"
#include <QTextStream>
#include <QtGui/QFontMetricsF>
#include <QFontDatabase>
#include <QClipboard>
#include <QApplication>
#include <QFileInfo>
#include <QTextCursor>
#include <QDebug>

Editor::Editor(QWidget* parent):
    QTextEdit(parent)
{
    mColorTheme = new ColorTheme(this);
    mSyntaxHighlighter = new SyntaxHighlighter(this->document(), mColorTheme);
    setTextColor(QColor(QString("White")));
    if(mColorTheme->getDefaultColor()!=QString(""))
        setTextColor(QColor(mColorTheme->getDefaultColor()));
    CreateShortcuts();
    connect(document(), SIGNAL(modificationChanged(bool)), this, SLOT(SetUnsaved()));
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);
    setTabStopDistance(QFontMetricsF(font()).width(' ')*4);
    SetSaved();
}

void Editor::CreateShortcuts()
{
    mShortcuts["Save"]= new QShortcut(QKeySequence("Ctrl+S"), this);
    QObject::connect(mShortcuts["Save"], SIGNAL(activated()), this, SLOT(Save()));
    mShortcuts["Open"]= new QShortcut(QKeySequence("Ctrl+O"), this);
    QObject::connect(mShortcuts["Open"], SIGNAL(activated()), this, SLOT(Open()));
    mShortcuts["New"]= new QShortcut(QKeySequence("Ctrl+N"), this);
    QObject::connect(mShortcuts["New"], SIGNAL(activated()), this, SLOT(NewPage()));
    mShortcuts["SaveAs"]= new QShortcut(QKeySequence("Ctrl+Shift+S"), this);
    QObject::connect(mShortcuts["SaveAs"], SIGNAL(activated()), this, SLOT(SaveAs()));
}

void Editor::Save()
{
    if(mSavePath == "")
    {
        PromptForSavePath();
        if(mSavePath == ""){
            return;
        }
        RefreshSyntaxHighlighter();

    }
    std::ofstream file;
    file.open(mSavePath.toUtf8());
    file << toPlainText().toUtf8().toStdString();
    file.close();
    SetSaved();
}

void Editor::SaveAs()
{
    if(mSavePath=="")
    {
        Save();
        return;
    }

    QString lastSavePath = mSavePath;
    mSavePath = QString("");
    PromptForSavePath();

    if(mSavePath == "")
    {
        mSavePath = lastSavePath;
        return;
    }

    std::ofstream file;
    file.open(mSavePath.toUtf8());
    file << toPlainText().toUtf8().toStdString();
    file.close();
    SetSaved();

    RefreshSyntaxHighlighter();
}

void Editor::Open()
{
    if(!CanExit())
    {
        return;
    }
    PromptForOpenPath();
    if(mSavePath=="")
    {
        return;
    }

    QFile file(mSavePath);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream ReadFile(&file);
    setText(ReadFile.readAll());
    SetSaved();
    qobject_cast<MainWindow*>(topLevelWidget())->getFiletree()->CreateFileTree(QFileInfo(mSavePath).absolutePath());
    RefreshSyntaxHighlighter();

}

void Editor::Open(QString path)
{
    QFile file(path);

    if(!CanExit() || !file.exists())
    {
        return;
    }
    mSavePath = path;
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream ReadFile(&file);
    setText(ReadFile.readAll());
    SetSaved();
    qobject_cast<MainWindow*>(topLevelWidget())->getFiletree()->CreateFileTree(QFileInfo(mSavePath).absolutePath());

}

void Editor::PromptForSavePath()
{
    const QString HOME_DIR = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).join("");
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", HOME_DIR);
    mSavePath = fileName;

}

void Editor::PromptForOpenPath()
{
    const QString HOME_DIR = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).join("");
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", HOME_DIR);
    qobject_cast<MainWindow*>(parent()->parent())->SetTitle(fileName);
    mSavePath = fileName;
}

void Editor::SetUnsaved()
{

    if(mIsSaved){
        qDebug() << QString("Unsaved*");
    }

    mIsSaved = false;
    qobject_cast<MainWindow*>(parent()->parent())->SetTitle(mSavePath+"*");
}

void Editor::SetSaved()
{
    mIsSaved = true;
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent()->parent());
    mainWindow->SetTitle(mSavePath);
    mExtension = QFileInfo(mSavePath).suffix();
    qDebug() <<QString("Saved to: " + mSavePath);
}

bool Editor::CanExit()
{
    if (!mIsSaved)
    {
        int result = SaveBeforeQuitPrompt();
        switch(result){
        case QMessageBox::Save:
            Save();
            return mIsSaved;
        case QMessageBox::Discard:
            return true;
        }
        return false;
    }
    return true;

}

int Editor::SaveBeforeQuitPrompt()
{
    QMessageBox* prompt = new QMessageBox(this);
    prompt->setText("You have unsaved changes.");
    prompt->setStandardButtons(QMessageBox::Cancel | QMessageBox::Save | QMessageBox::Discard);
    prompt->setDefaultButton(QMessageBox::Cancel);
    int result = prompt->exec();
    return result;
}

void Editor::RefreshSyntaxHighlighter()
{
    blockSignals(true);
    mSyntaxHighlighter->refresh();
    blockSignals(false);
}

void Editor::NewPage()
{
    if (CanExit())
    {
        setText("");
        SetSaved();
    }
}

bool Editor::filterEvent(QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if(keyEvent->key() == Qt::Key_Tab)
        {
            insertPlainText(QString().fill(' ', 4));
            return true;
        }
        return false;
    }
    return false;
}

QString Editor::getSavePath()
{
    return mSavePath;
}

void Editor::CopyPath()
{
    QApplication::clipboard()->setText(mSavePath);
}

Editor::~Editor()
{
    delete mSyntaxHighlighter;
    delete mColorTheme;
    for(auto itr = mShortcuts.begin(); itr!=mShortcuts.end(); ++itr){
        delete (*itr).second;
    }
}

/* TODO:
 * Line numbers
 * File tree directory recursion
 * Client/Server
 * Syntax Highlighting
 * Folding
 * right click menu
 */
