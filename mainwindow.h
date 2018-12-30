#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class QWidgetItem;
class Editor;
class Filetree;
class QHBoxLayout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetTitle(QString);
    Filetree* getFiletree();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void closeEvent(QCloseEvent *event);
    Ui::MainWindow *ui;
    QWidget* mContainer;
    Editor* mEditor;
    QHBoxLayout* mLayout;
    Filetree* mFiletree;


    void SetUpMenu();

private slots:
    void CreateClientPrompt();
    void CreateServerPrompt();

};

#endif // MAINWINDOW_H
