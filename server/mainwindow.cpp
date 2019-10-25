#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    this->textEdit = new QTextEdit;
    this->textEdit->setText(tr("Text"));
    resize(700,600);
    setCentralWidget(this->textEdit);
    setWindowTitle("Server IoT");
    createActions();
    createToolBars();
    this->connectPLC = nullptr;
    this->serverStart = nullptr;
    this->serverWindow = nullptr;
    this->serverStop = nullptr;
    this->serverProcess = nullptr;
}

void MainWindow::createActions(){
    this->server = new QAction(
                  tr("&Start Server"), this
                );
    this->server->setIcon(QIcon(":images/img/start-server-01.png"));
    this->server->setShortcut(tr("Ctrl+N"));
    this->server->setStatusTip(tr("begin a server"));

    this->plc = new QAction(
                    QIcon(""), tr("&Connect to PLC"), this
                );
    this->plc->setShortcut(tr("Ctrl+M"));
    this->plc->setStatusTip(tr("begin a plc connnection"));

    connect(server, SIGNAL(triggered()), this, SLOT(startServer()));
    connect(plc, SIGNAL(triggered()), this, SLOT(connectToPLC()));

}
void MainWindow::startServer(){
    if(!this->serverProcess){
        this->serverProcess = new ServerWindow(this);
    }
    this->serverProcess->show();
    this->serverProcess->raise();
    this->serverProcess->activateWindow();
    /*
    if(!this->serverWindow){
        this->serverWindow = new ServerWindow(this);
    }
    this->serverWindow->show();
    this->serverWindow->raise();
    this->serverWindow->activateWindow();
    */
}

void MainWindow::connectToPLC(){
    if(!this->connectPLC){
        this->connectPLC = new ConnectPLC(this);
    }
    this->connectPLC->show();
    this->connectPLC->raise();
    this->connectPLC->activateWindow();
}

void MainWindow::createMenus(){

}

void MainWindow::createToolBars(){
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(this->server);
    fileToolBar->addAction(this->plc);
}

void MainWindow::createStatusBar(){

}

void MainWindow::copy(){

}


MainWindow::~MainWindow()
{

}

