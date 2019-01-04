#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class QWidgetItem;
class Editor;
class Filetree;
class QHBoxLayout;
class Server;
class Settings;
class VersionControl;

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
    Editor* getEditor();
    Settings* getSettings();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void closeEvent(QCloseEvent *event);
    Ui::MainWindow *ui;
    QWidget* mContainer;
    Editor* mEditor;
    QHBoxLayout* mLayout;
    Filetree* mFiletree;
    Server* mServer;
    Settings* mSettings;
    VersionControl* mVersionControl;

    void SetUpMenu();

private slots:
    void CreateClientPrompt();
    void CreateServerPrompt();
    void ChooseThemePrompt();
};

#endif // MAINWINDOW_H
