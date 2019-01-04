#ifndef VERSIONCONTROL_H
#define VERSIONCONTROL_H

#include <QWidget>

class VersionControl : public QWidget
{
    Q_OBJECT
    QString GetLocalRepository();
    QStringList GetStaged(QStringList);
    QStringList GetNotStage(QStringList);
    QStringList GetUntracked(QStringList);
public:
    explicit VersionControl(QWidget *parent = nullptr);

public slots:
    QStringList GitStatus();
    bool IsLocalRepository();
};

#endif // VERSIONCONTROL_H
