#ifndef CONNECTTOSERVER_H
#define CONNECTTOSERVER_H

#include <QDialog>
#include <QDataStream>
#include <QTcpSocket>
#include <QtNetwork>
#include <QtWidgets>
#include <QDebug>


class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class ConnectToServer : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectToServer(QWidget *parent = nullptr);

signals:

public slots:
    void requestNewtFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void enabledGetFortuneButton();
    void sessionOpened();

private:
    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *getFortuneButton = nullptr;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString currentFortune;
    QNetworkSession *networkSession = nullptr;
};

#endif // CONNECTTOSERVER_H
