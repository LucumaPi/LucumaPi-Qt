#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    this->textEdit = new QTextEdit;
    this->textEdit->setText(tr("Text"));
    resize(700,600);
    setCentralWidget(this->textEdit);
    setWindowTitle("Client IoT");
    createActions();
    createToolBars();
    this->connectServer = nullptr;
}


void MainWindow::createActions(){
    this->connecttoServerAction = new QAction(
                tr("&Connect to Server"), this);
    this->connecttoServerAction->setIcon(QIcon(""));
    this->connecttoServerAction->setShortcut(tr("Ctrl+N"));
    this->connecttoServerAction->setStatusTip(tr("Begin connection..."));

    connect(connecttoServerAction, SIGNAL(triggered()), this, SLOT(connectToServer()));

}


void MainWindow::connectToServer(){
    if(!this->connectServer){
        this->connectServer = new ConnectToServer(this);
    }
    this->connectServer->show();
    this->connectServer->raise();
    this->connectServer->activateWindow();
}


void MainWindow::createToolBars(){
    this->fileToolBar = addToolBar(tr("File"));
    this->fileToolBar->addAction(this->connecttoServerAction);
}

MainWindow::~MainWindow()
{

}
