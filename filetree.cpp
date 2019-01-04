#include "filetree.h"
#include "mainwindow.h"
#include "editor.h"
#include <QWidget>
#include <QDirIterator>
#include <QTreeWidgetItem>

Filetree::Filetree(QWidget *parent) : QWidget(parent)
{
    mTree = new QTreeWidget(this);
    mTree->setColumnCount(1);
    mTree->setRootIsDecorated(false);

    mLayout = new QVBoxLayout(this);
    mLayout->setSpacing(0);
    mLayout->setMargin(0);
    mLayout->addWidget(mTree);
    setLayout(mLayout);

    connect(mTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)),this, SLOT(OnTreeClick(QTreeWidgetItem*, int)));
}


void Filetree::CreateFileTree(QString path)
{
    mTree->clear();
    qDebug() <<"Creating tree in path: " + path;

    QDirIterator it(path, QStringList() << "*", QDir::Files | QDir::NoDotDot, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        QString absPath = it.next();
        QTreeWidgetItem* leaf = new QTreeWidgetItem(mTree);
        leaf->setText(0, QFileInfo(absPath).fileName());
        leaf->setTextColor(0, QColor("White"));
        leaf->setIcon(0, QIcon(":/Assets/fileSaved.png"));
        leaf->setTextAlignment(0, Qt::AlignLeft);
        leaf->setData(0, Qt::UserRole, absPath);
        mTree->addTopLevelItem(leaf);
    }

    QDirIterator dirItr(path, QDir::Dirs | QDir::NoSymLinks | QDir::NoDot);
    while (dirItr.hasNext())
    {
        QString absPath = dirItr.next();
        QTreeWidgetItem* leaf = new QTreeWidgetItem(mTree);
        leaf->setText(0, QFileInfo(absPath).fileName());
        leaf->setTextColor(0, QColor("White"));
        leaf->setIcon(0, QIcon(":/Assets/directory.png"));
        leaf->setTextAlignment(0, Qt::AlignLeft);
        leaf->setData(0, Qt::UserRole, absPath);
        mTree->addTopLevelItem(leaf);
    }

    mTree->sortColumn();
}

void Filetree::OnTreeClick(QTreeWidgetItem * item, int column)
{
    QFileInfo path(item->data(0, Qt::UserRole).toString());

    if(path.isDir())
    {
        CreateFileTree(path.absoluteFilePath());
    }
    else
    {
        MainWindow* mw = qobject_cast<MainWindow*>(topLevelWidget());
        mw->getEditor()->Open(path.absoluteFilePath());
    }


}
