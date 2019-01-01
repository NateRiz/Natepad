#include "filetree.h"
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <string>
#include "mainwindow.h"
#include "editor.h"
#include <QScrollBar>

Filetree::Filetree(QWidget *parent) : QWidget(parent)
{
    mContainer = new QFrame(this);
    mContainer->setStyleSheet("background-color:rgb(60,63,65);");
    mLayout = new QVBoxLayout(mContainer);
    mContainer->setLayout(mLayout);

    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    mLayout->setAlignment(Qt::AlignTop);

    mRootLayout = new QVBoxLayout(this);
    mRootLayout->setMargin(0);
    mRootLayout->setSpacing(0);
    setLayout(mRootLayout);
    mRootLayout->addWidget(mContainer);

    mScroll = new QScrollArea;
    mScroll->setWidget(mContainer);
    mScroll->setWidgetResizable(true);
    mRootLayout->addWidget(mScroll);

    mScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mScroll->verticalScrollBar()->setEnabled(true);
    mScroll->horizontalScrollBar()->setEnabled(false);

    mFiletreePath = "";
    mSelectedFiles = std::unordered_set<File*>();
    mLastSelectedFile = nullptr;
}

void Filetree::CreateFileTree(QString path)
{
    ClearChildren();

    QDirIterator it(path, QStringList() << "*", QDir::Files | QDir::NoDot | QDir::NoDotDot, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        AddFile(QFileInfo(it.next()).absoluteFilePath());
    }
    QDirIterator dirItr(path, QStringList() << "*", QDir::AllDirs | QDir::NoDot | QDir::NoDotDot, QDirIterator::NoIteratorFlags);
    while (dirItr.hasNext())
    {
        AddDirectory(QFileInfo(dirItr.next()).absoluteFilePath());
    }
    mFiletreePath = path;
}

void Filetree::ClearChildren()
{
    for (File* child: findChildren<File*>())
    {
        delete child;
    }
    mSelectedFiles.clear();
}

void Filetree::AddFile(QString fileName)
{
    QWidget* file = new FileLeaf(this, fileName);
    mLayout->addWidget(file);
}

void Filetree::AddDirectory(QString dirName)
{
    QWidget* dir = new FileBranch(this, dirName);
    mLayout->addWidget(dir);
}

QString Filetree::getFiletreePath()
{
    return mFiletreePath;
}

void Filetree::SetFileSelected(FileSelectEvent file)
{
    if(file.mFile==nullptr)
    {
        for (File* file : mSelectedFiles)
        {
            file->setStyleSheet("");
        }
        mSelectedFiles.clear();
        return;
    }

    if(file.mIsOverridden)
    {
        if(file.mOverrideIsSelected && mSelectedFiles.find(file.mFile)==mSelectedFiles.end())
        {
            mSelectedFiles.insert(file.mFile);
            file.mFile->setStyleSheet("background-color:rgb(47,101,202)");
        }
        else if (!file.mOverrideIsSelected && mSelectedFiles.find(file.mFile)!=mSelectedFiles.end())
        {
            mSelectedFiles.erase(file.mFile);
            file.mFile->setStyleSheet("");
        }
        return;
    }

    if(file.mIsControlDown)
    {
        SwapSelected(file.mFile);
    }
    else
    {
        for (File* file : mSelectedFiles)
        {
            file->setStyleSheet("");
        }
        mSelectedFiles.clear();
        mSelectedFiles.insert(file.mFile);
        file.mFile->setStyleSheet("background-color:rgb(47,101,202)");
    }


}

void Filetree::SwapSelected(File* file)
{
    if(mSelectedFiles.find(file)==mSelectedFiles.end())
    {
        mSelectedFiles.insert(file);
        file->setStyleSheet("background-color:rgb(47,101,202)");
    }
    else
    {
        mSelectedFiles.erase(file);
        file->setStyleSheet("");
    }
}

void Filetree::SelectFilesInRange(FileSelectEvent start, FileSelectEvent end)
{
    bool isHighlighting = false;
    if(start.mFile!=nullptr)
    {
        for(File* file : findChildren<File*>())
        {
            if(file==start.mFile){
                isHighlighting = true;
            }

            if(isHighlighting){
                SetFileSelected(FileSelectEvent(file, start.mIsControlDown, true, true, true));

            }

            if (file==end.mFile){
                return;
            }

        }
    }

}

FileSelectEvent Filetree::GetFileUnderMouse(QMouseEvent* event)
{
    bool isControlDown = event->modifiers().testFlag(Qt::ControlModifier);
    bool isShiftDown = event->modifiers().testFlag(Qt::ShiftModifier);


    for(File* file : findChildren<File*>())
    {
        if(file->underMouse())
        {
            return FileSelectEvent(file, isControlDown, isShiftDown, false, false);
        }
    }

    return nullptr;
}

void Filetree::Select(FileSelectEvent selectedFile)
{
    if(selectedFile.mFile == nullptr)
        return;

    bool isAlreadySelected = mSelectedFiles.find(selectedFile.mFile)!=mSelectedFiles.end();
    if (isAlreadySelected && !(selectedFile.mIsShiftDown || selectedFile.mIsControlDown))
    {
        qobject_cast<MainWindow*>(topLevelWidget())->getEditor()->Open(selectedFile.mFile->getFilePath());
        return;
    }

    if(!selectedFile.mIsShiftDown)
    {
        SetFileSelected(selectedFile);
        mLastSelectedFile = selectedFile.mFile;
        return;
    }

    //Shift multi selecting
    FileSelectEvent start = FileSelectEvent();
    FileSelectEvent end = FileSelectEvent();

    SetFileSelected(FileSelectEvent(nullptr)); // Clear

    for(File* file : findChildren<File*>())
    {
        if(start.mFile == nullptr && file == mLastSelectedFile)
        {
            start = FileSelectEvent(file, selectedFile.mIsControlDown);
            end = FileSelectEvent(selectedFile.mFile, selectedFile.mIsControlDown);
            break;
        }
        if(start.mFile == nullptr && file == selectedFile.mFile)
        {
            start = FileSelectEvent(selectedFile.mFile, selectedFile.mIsControlDown);
        }
        if(start.mFile != nullptr && file == mLastSelectedFile)
        {
            end = FileSelectEvent(file, selectedFile.mIsControlDown);
            break;
        }
    }
    SelectFilesInRange(start, end);
}

Filetree::~Filetree()
{
    ClearChildren();
    delete mLayout;
}
