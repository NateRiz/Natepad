#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editor.h"
#include "filetree.h"
#include "server.h"
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QFontDatabase>
#include <QSizePolicy>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    delete ui->mainToolBar;
    setStyleSheet("background-color:rgb(60,63,65); color:White;");
    setGeometry(0, 0, 800, 600);

    mContainer = new QWidget(this);
    mContainer->setStyleSheet("background-color:rgb(43,43,43);");
    setCentralWidget(mContainer);

    mLayout = new QHBoxLayout;
    mContainer->setLayout(mLayout);

    mFiletree = new Filetree(mContainer);
    QSizePolicy spLeft(QSizePolicy::Ignored, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    mFiletree->setSizePolicy(spLeft);
    mLayout->addWidget(mFiletree);


    mEditor = new Editor(mContainer);
    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(8);
    mEditor->setSizePolicy(spRight);
    mLayout->addWidget(mEditor);
    mEditor->installEventFilter(this);


    SetUpMenu();
}

void MainWindow::SetUpMenu()
{
    menuBar()->setStyleSheet("selection-background-color:rgb(47,101,202)");
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    menuBar()->setFont(fixedFont);


    QMenu* file = menuBar()->addMenu(tr("&File"));
    file->setFont(fixedFont);
    QAction* aNew = file->addAction(QString("New").leftJustified(16,' ')+QString("Ctrl+N"));
    connect(aNew, SIGNAL(triggered()), mEditor, SLOT(NewPage()));
    QAction* aSave = file->addAction(QString("Save").leftJustified(16,' ')+QString("Ctrl+S"));
    connect(aSave, SIGNAL(triggered()), mEditor, SLOT(Save()));
    QAction* aOpen = file->addAction(QString("Open").leftJustified(16,' ')+QString("Ctrl+O"));
    connect(aOpen, SIGNAL(triggered()), mEditor, SLOT(Open()));
    QAction* aSaveAs = file->addAction(QString("Save As").leftJustified(16,' ')+QString("Ctrl+Shift+S"));
    connect(aSaveAs, SIGNAL(triggered()), mEditor, SLOT(SaveAs()));
    
    QMenu* edit = menuBar()->addMenu(tr("&Edit"));
    edit->setFont(fixedFont);
    QAction* aUndo = edit->addAction(QString("Undo").leftJustified(16,' ')+QString("Ctrl+Z"));
    connect(aUndo, SIGNAL(triggered()), mEditor, SLOT(undo()));
    QAction* aRedo = edit->addAction(QString("Redo").leftJustified(16,' ')+QString("Ctrl+Y"));
    connect(aRedo, SIGNAL(triggered()), mEditor, SLOT(redo()));
    QAction* aCopy = edit->addAction(QString("Copy").leftJustified(16,' ')+QString("Ctrl+C"));
    connect(aCopy, SIGNAL(triggered()), mEditor, SLOT(copy()));
    QAction* aPaste = edit->addAction(QString("Paste").leftJustified(16,' ')+QString("Ctrl+V"));
    connect(aPaste, SIGNAL(triggered()), mEditor, SLOT(paste()));
    QAction* aCopyPath = edit->addAction("Copy Path");
    connect(aCopyPath, SIGNAL(triggered()), mEditor, SLOT(CopyPath()));
    QAction* aSelectAll = edit->addAction(QString("Select All").leftJustified(16,' ')+QString("Ctrl+A"));
    connect(aSelectAll, SIGNAL(triggered()), mEditor, SLOT(selectAll()));


    QMenu* online = menuBar()->addMenu(tr("&Connect"));
    QAction* aConnect = online->addAction(QString("Connect"));
    connect(aConnect, SIGNAL(triggered()), this, SLOT(CreateClientPrompt()));
    QAction* aHost = online->addAction(QString("Host"));
    connect(aHost, SIGNAL(triggered()), this, SLOT(CreateServerPrompt()));
    online->setFont(fixedFont);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(mEditor->CanExit())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::SetTitle(QString title)
{
    setWindowTitle(title);
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == mEditor)
    {
        return mEditor->filterEvent(event);
    }

    return false;
}

Filetree* MainWindow::getFiletree()
{
    return mFiletree;
}

Editor *MainWindow::getEditor()
{
    return mEditor;
}

void MainWindow::CreateClientPrompt()
{
    QInputDialog::getText(this, "Join", "IP");
}

void MainWindow::CreateServerPrompt()
{
    mServer = new Server();
}

MainWindow::~MainWindow()
{
    delete mEditor;
    delete mLayout;
    delete mContainer;
    delete ui;
}
