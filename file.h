#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class File: public QWidget
{
    Q_OBJECT
public:
    explicit File(QWidget* parent, QString filePath);
    virtual ~File(){}
    QString getFilePath();

public slots:
    void SelectSelf(QMouseEvent *event);
signals:
    void clicked(QMouseEvent* event);


protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QString mFilePath;

};

class FileLeaf: public File
{
    Q_OBJECT
public:
    explicit FileLeaf(QWidget* parent, QString fileName);
    virtual ~FileLeaf(){
        delete mFileName;
        delete mIcon;
        delete mLayout;
    }

private:
    QLabel* mFileName;
    QLabel* mIcon;
    QPixmap mPixmap;
    QHBoxLayout* mLayout;
};

class FileBranch: public File
{
    Q_OBJECT
public:
    explicit FileBranch(QWidget* parent, QString fileName);
    virtual ~FileBranch(){
        delete mFileName;
        delete mIcon;
        delete mLayout;
    }

private:
    QLabel* mFileName;
    QLabel* mIcon;
    QPixmap mPixmap;
    QHBoxLayout* mLayout;
};


#endif // FILE_H
