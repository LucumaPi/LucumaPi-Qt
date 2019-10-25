#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTextEdit>
#include "PLC/connectplc.h"
#include "server-architecture/server.h"
#include "server-architecture/serverwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,
                        Qt::WindowFlags flags = nullptr);
    ~MainWindow();

signals:

public slots:
    void startServer();
    void connectToPLC();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void copy();
    ConnectPLC *connectPLC;
    ServerWindow *serverWindow;
    Server *serverStart;
    Server *serverStop;

    QMenu *fileMenu;
    QMenu *windowMenu;
    QMenu *editMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *server;
    QAction *plc;
    QAction *openAct;
    QAction *exitAct;
    QAction *copyAct;

    QTextEdit *textEdit;
    ServerWindow *serverProcess;
};
#endif // MAINWINDOW_H
