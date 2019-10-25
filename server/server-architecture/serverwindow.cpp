#include "serverwindow.h"


ServerWindow::ServerWindow(QWidget *parent) : QDialog(parent), statusLabel(new QLabel)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QNetworkConfigurationManager manager;
    if(manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired){
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));

        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered){
            config = manager.defaultConfiguration();
        }
        this->networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &ServerWindow::sessionOpened);
        this->statusLabel->setText(tr("Opening network session."));
        this->networkSession->open();
    }else{
        this->sessionOpened();
    }

    this->fortunes << tr("you've been connected to Server...");

    auto quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpServer, &QTcpServer::newConnection, this, &ServerWindow::sendData);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = nullptr;
    if(QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()){
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0,0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QVBoxLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    }else{
        mainLayout = new QVBoxLayout(this);
    }
    mainLayout->addWidget(this->statusLabel);
    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QGuiApplication::applicationDisplayName());

    /*
    this->ipClientLabel = new QLabel(tr("IP :"));
    this->dbNumberLabel = new QLabel(tr("DB# :"));
    this->ipPLCLabel = new QLabel(tr("PLC ip :"));
    this->rackLabel = new QLabel(tr("Rack :"));
    this->slotLabel = new QLabel(tr("Slot :"));

    this->ipClientLineEdit = new QLineEdit("192.168.1.x");

    this->dbNumberLineEdit = new QLineEdit("");
    this->dbNumberLineEdit->setValidator(new QIntValidator(1, 200, this));

    this->ipPLCLineEdit = new QLineEdit("192.168.1.x");

    this->rackLineEdit = new QLineEdit("0");
    this->rackLineEdit->setValidator(new QIntValidator(1, 200, this));

    this->slotLineEdit = new QLineEdit("1");
    this->slotLineEdit->setValidator(new QIntValidator(1, 200, this));

    this->ipClientLabel->setBuddy(this->ipClientLineEdit);
    this->dbNumberLabel->setBuddy(this->dbNumberLineEdit);
    this->ipPLCLabel->setBuddy(this->ipPLCLineEdit);
    this->rackLabel->setBuddy(this->rackLineEdit);
    this->slotLabel->setBuddy(this->slotLineEdit);

    this->startServer = new QPushButton(tr("Start server"));
    this->startServer->setDefault(true);

    this->quitServerWindow = new QPushButton(tr("Quit"));

    this->buttonBox = new QDialogButtonBox;
    this->buttonBox->addButton(this->startServer, QDialogButtonBox::ActionRole);
    this->buttonBox->addButton(this->quitServerWindow, QDialogButtonBox::RejectRole);

    this->plcNumber = nullptr;
    this->dbNumber = nullptr;
    this->server = nullptr;
    connect(startServer, SIGNAL(clicked()), this, SLOT(startServerProcess()));
    connect(quitServerWindow, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(this->ipClientLabel, 0, 0);
    mainLayout->addWidget(this->ipClientLineEdit, 0, 1);

    mainLayout->addWidget(this->dbNumberLabel, 1, 0);
    mainLayout->addWidget(this->dbNumberLineEdit, 1, 1);

    mainLayout->addWidget(this->ipPLCLabel, 2, 0);
    mainLayout->addWidget(this->ipPLCLineEdit, 2, 1);

    mainLayout->addWidget(this->rackLabel, 3, 0);
    mainLayout->addWidget(this->rackLineEdit, 3, 1);

    mainLayout->addWidget(this->slotLabel, 4, 0);
    mainLayout->addWidget(this->slotLineEdit, 4, 1);

    mainLayout->addWidget(this->buttonBox, 5, 0);
    setLayout(mainLayout);
    setWindowTitle("Start Server");
    */
}

void ServerWindow::sessionOpened(){
    if(networkSession){
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if(config.type() == QNetworkConfiguration::UserChoice){
            id = config.identifier();
        }else{
            id = config.identifier();
        }
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.value(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }
    this->tcpServer = new QTcpServer(this);
    if(!this->tcpServer->listen()){
        QMessageBox::critical(this, tr("Server"),
                              tr("Unable to start the server : %1.").arg(this->tcpServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); i++) {
        if(ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()){
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    if(ipAddress.isEmpty()){
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    this->statusLabel->setText(tr("The server is running on\n\nIP: %1\nport : %2\n\n"
                                  "Run the fortune CLient Example now").arg(ipAddress).arg(this->tcpServer->serverPort()));


    /*
    QString ipClient = this->ipClientLineEdit->text();
    int dbNumber = this->dbNumberLineEdit->text().toInt();
    QString ipPlc = this->ipPLCLineEdit->text();
    short rack = this->rackLineEdit->text().toShort();
    short slot = this->slotLineEdit->text().toShort();

    QByteArray ipClient1 = ipClient.toLocal8Bit();
    const char *c_str1 = ipClient1.data();

    QByteArray ipPlc1 = ipPlc.toLocal8Bit();
    const char *c_str2 = ipPlc1.data();

    this->plcNumber = new PLCNumber(c_str1, c_str2, dbNumber, rack, slot);
    this->dbNumber = new DBNumber(c_str2, dbNumber, this->plcNumber);
    this->server = new Server(c_str1, dbNumber, this->dbNumber);

    */
}

void ServerWindow::sendData(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << this->fortunes[0];
    QTcpSocket *clientConnection = this->tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}


