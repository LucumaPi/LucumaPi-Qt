#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "connecttoserver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr,
               Qt::WindowFlags flags = nullptr);
    ~MainWindow();

private slots:
    void connectToServer();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    ConnectToServer *connectServer;

    QMenu *fileMenu;
    QMenu *windowMenu;
    QMenu *editMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *connecttoServerAction;
    QAction *exitAct;

    QTextEdit *textEdit;

};
#endif // MAINWINDOW_H
