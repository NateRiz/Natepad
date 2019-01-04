#ifndef FILETREE_H
#define FILETREE_H

#include <QTreeWidget>
#include <QVBoxLayout>

class Filetree : public QWidget
{
    Q_OBJECT
public:
    explicit Filetree(QWidget *parent = nullptr);
    void CreateFileTree(QString path);

private:
    QVBoxLayout* mLayout;
    QTreeWidget* mTree;


public slots:
    void OnTreeClick(QTreeWidgetItem*, int);

};

#endif // FILETREE_H
