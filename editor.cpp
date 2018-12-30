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
    mColorTheme = new ColorTheme(this, QString("py"));
    setStyleSheet("Color: White"); //Text color
    CreateShortcuts();
    connect(this, SIGNAL(textChanged()), this, SLOT(SetUnsaved()));
    SetSaved();
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);
    setTabStopDistance(QFontMetricsF(font()).width(' ')*4);
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
        if(mSavePath == "")
            return;
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
    mIsSaved = false;
    qobject_cast<MainWindow*>(parent()->parent())->SetTitle(mSavePath+"*");

}

void Editor::SetSaved()
{
    mIsSaved = true;
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent()->parent());
    mainWindow->SetTitle(mSavePath);
    mExtension = QFileInfo(mSavePath).suffix();

    QString path = QFileInfo(mSavePath).absolutePath();
    if(mainWindow->getFiletree()->getFiletreePath() != path)
    {
        mainWindow->getFiletree()->CreateFileTree(path);
    }


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

        /* //Very Broken
        QTextCursor tc = textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString copy = tc.selectedText();
        std::string hexColor = mColorTheme->ParseWord(tc.selectedText());
        qDebug() << tc.selectedText();
        qDebug() << QString::fromStdString(hexColor);
        tc.removeSelectedText();
        setTextColor(QColor(QString::fromStdString(hexColor)));
        tc.insertText(copy);
        */
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

void Editor::CopyPath()
{
    QApplication::clipboard()->setText(mSavePath);
}

Editor::~Editor()
{
}

/* TODO:
 * Line numbers
 * File tree fix background color
 * File Tree selection and directory recursion
 * Client/Server
 * Syntax Highlighting
 * Folding
 */
