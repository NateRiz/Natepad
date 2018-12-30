#include "file.h"
#include "filetree.h"
File::File(QWidget *parent): QWidget(parent)
{
    connect(this, SIGNAL(clicked(QMouseEvent*)), this, SLOT(SelectSelf(QMouseEvent*)));
}

void File::SelectSelf(QMouseEvent* event)
{
    Filetree* filetree = qobject_cast<Filetree*>(parent()->parent());
    filetree->Select(filetree->GetFileUnderMouse(event));
}

void File::mousePressEvent(QMouseEvent *event)
{
    emit clicked(event);
}

FileLeaf::FileLeaf(QWidget *parent, QString fileName):
    File(parent)
{
    mFileName = new QLabel(this);
    mIcon = new QLabel(this);
    mFileName->setText(fileName);
    mFileName->setStyleSheet("padding: 4px");//N.E.S.W.
    mFileName->setAlignment(Qt::AlignLeft);

    mPixmap = QPixmap(":/Assets/fileSaved.png");
    mIcon->setPixmap(mPixmap);
    mIcon->setAlignment(Qt::AlignCenter);
    mIcon->setMinimumHeight(int(mIcon->height()));
    mIcon->setStyleSheet("padding: 2px");

    mLayout = new QHBoxLayout(this);
    setLayout(mLayout);
    mLayout->addWidget(mIcon);
    mLayout->addWidget(mFileName);
    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    mLayout->addStretch();

}

FileBranch::FileBranch(QWidget *parent, QString fileName):
    File(parent)
{
    mFileName = new QLabel(this);
    mIcon = new QLabel(this);
    mFileName->setText(fileName);
    mFileName->setStyleSheet("padding: 4px");//N.E.S.W.
    mFileName->setAlignment(Qt::AlignLeft);

    mPixmap = QPixmap(":/Assets/directory.png");
    mIcon->setPixmap(mPixmap);
    mIcon->setAlignment(Qt::AlignCenter);
    mIcon->setMinimumHeight(int(mIcon->height()));
    mIcon->setStyleSheet("padding: 2px");

    mLayout = new QHBoxLayout(this);
    setLayout(mLayout);
    mLayout->addWidget(mIcon);
    mLayout->addWidget(mFileName);
    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    mLayout->addStretch();
}
