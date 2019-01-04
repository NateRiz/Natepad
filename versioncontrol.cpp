#include "versioncontrol.h"
#include "mainwindow.h"
#include "editor.h"
#include <QFileInfo>
#include <QProcess>
#include <QDir>

VersionControl::VersionControl(QWidget *parent) :
    QWidget(parent)
{

}

QStringList VersionControl::GitStatus()
{
    QProcess process;
    process.setWorkingDirectory(GetLocalRepository());
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(QString("git status").arg(GetLocalRepository()));
    qDebug() << process.readAll();
    process.waitForFinished();
    QStringList output(QString(process.readAllStandardOutput()).split("\n"));
    qDebug() << output.join("\n");
    return output;
}

QString VersionControl::GetLocalRepository()
{
    MainWindow* mw = qobject_cast<MainWindow*>(parent());
    QFileInfo path = QFileInfo(mw->getEditor()->getSavePath());
    QDir gitPath(path.absolutePath());
    if(gitPath.cd(".git")){
        gitPath.cdUp();
        return gitPath.absolutePath();
    }
    return QString("");

}

QStringList VersionControl::GetStaged(QStringList status)
{
    return QStringList();
}

QStringList VersionControl::GetNotStage(QStringList status)
{
    return QStringList();
}

QStringList VersionControl::GetUntracked(QStringList status)
{
    return QStringList();
}

bool VersionControl::IsLocalRepository()
{
    return GetLocalRepository()!=QString("");
}
