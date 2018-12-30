#ifndef FILETREE_H
#define FILETREE_H

#include "file.h"

#include <QWidget>
#include <QLabel>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <unordered_set>

struct FileSelectEvent{
    FileSelectEvent()=default;
    FileSelectEvent(File* f, bool ctrl = false, bool shift = false, bool override = false, bool selected = false):
        mFile(f), mIsControlDown(ctrl), mIsShiftDown(shift), mIsOverridden(override), mOverrideIsSelected(selected){}
    ~FileSelectEvent(){}

    File* mFile;
    bool mIsControlDown;
    bool mIsShiftDown;
    bool mIsOverridden;
    bool mOverrideIsSelected;
};

class Filetree : public QWidget
{
    Q_OBJECT
public:
    explicit Filetree(QWidget *parent = nullptr);
    virtual ~Filetree();
    void CreateFileTree(QString path);
    FileSelectEvent GetFileUnderMouse(QMouseEvent *event);
    void Select(FileSelectEvent);
    QString getFiletreePath();


signals:

private:
    void AddFile(QString fileName);
    void AddDirectory(QString dirName);
    void ClearChildren();
    void SetFileSelected(FileSelectEvent);
    void SwapSelected(File*);
    void SelectFilesInRange(FileSelectEvent start, FileSelectEvent end);
    QVBoxLayout* mRootLayout;
    QVBoxLayout* mLayout;
    QWidget* mContainer;
    QString mFiletreePath;
    std::unordered_set<File*> mSelectedFiles;
    File* mLastSelectedFile;
};


#endif // FILETREE_H
