#include "connecttoserver.h"

ConnectToServer::ConnectToServer(QWidget *parent) : QDialog(parent)
  , hostCombo(new QComboBox)
  , portLineEdit(new QLineEdit)
  , getFortuneButton(new QPushButton(tr("Connect")))
  , tcpSocket(new QTcpSocket(this))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->hostCombo->setEditable(true);

    QString name = QHostInfo::localHostName();
    if(!name.isEmpty()){
        this->hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if(!domain.isEmpty()){
            this->hostCombo->addItem(name + QChar('.') + domain);
        }
    }
    if(name != QLatin1String("LocalHost")){
        this->hostCombo->addItem(QString("localhost"));
    }

    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    qDebug() << ipAddressList;

    for(int i = 0; i < ipAddressList.size(); i++){
        if(!ipAddressList.at(i).isLoopback()){
            qDebug() << ipAddressList.at(i);
            this->hostCombo->addItem(ipAddressList.at(i).toString());
        }
    }
    for(int i = 0; i < ipAddressList.size(); ++i){
        if(ipAddressList.at(i).isLoopback()){
            this->hostCombo->addItem(ipAddressList.at(i).toString());
        }
    }
    this->portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    auto hostLabel = new QLabel(tr("&Server name: "));
    hostLabel->setBuddy(hostCombo);
    auto portLabel = new QLabel(tr("&Server port: "));
    portLabel->setBuddy(portLineEdit);

    this->statusLabel = new QLabel(tr("This Dialog requires that you run the IOT Server."));

    this->getFortuneButton->setDefault(true);
    this->getFortuneButton->setEnabled(false);

    auto quitButton = new QPushButton(tr("Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(hostCombo, &QComboBox::editTextChanged, this, &ConnectToServer::enabledGetFortuneButton);

    connect(portLineEdit, &QLineEdit::textChanged, this, &ConnectToServer::enabledGetFortuneButton);

    connect(getFortuneButton, &QAbstractButton::clicked, this, &ConnectToServer::requestNewtFortune);

    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

    connect(tcpSocket, &QIODevice::readyRead, this, &ConnectToServer::readFortune);

    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ConnectToServer::displayError);

    QGridLayout *mainLayout = nullptr;

    if(QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()){
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0,0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0,0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    }else{
        mainLayout = new QGridLayout(this);
    }

    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    this->portLineEdit->setFocus();

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
        connect(networkSession, &QNetworkSession::opened, this, &ConnectToServer::sessionOpened);

        this->getFortuneButton->setEnabled(false);
        this->statusLabel->setText(tr("Opening network session."));
        this->networkSession->open();
    }
}

void ConnectToServer::requestNewtFortune(){
    getFortuneButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(), portLineEdit->text().toInt());
}

void ConnectToServer::readFortune(){
    in.startTransaction();
    QString nextFortune;
    in >> nextFortune;

    if(!in.commitTransaction()){
        return ;
    }
    if(nextFortune == currentFortune){
        QTimer::singleShot(0, this, &ConnectToServer::requestNewtFortune);
        return;
    }

    this->currentFortune = nextFortune;
    this->statusLabel->setText(currentFortune);
    this->getFortuneButton->setEnabled(true);
}

void ConnectToServer::displayError(QAbstractSocket::SocketError socketError){
    switch(socketError){
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Fortune Client"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));

    }
    this->getFortuneButton->setEnabled(true);
}

void ConnectToServer::enabledGetFortuneButton(){
    this->getFortuneButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                       !hostCombo->currentText().isEmpty() && !portLineEdit->text().isEmpty());
}

void ConnectToServer::sessionOpened(){
    QNetworkConfiguration config = this->networkSession->configuration();
    QString id;

    if(config.type() == QNetworkConfiguration::UserChoice){
        id = this->networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    }else{
        id = config.identifier();
    }

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    this->statusLabel->setText(tr("This examples requires that you run the "
                                  "Fortune Server example as well."));

    enabledGetFortuneButton();
}


























