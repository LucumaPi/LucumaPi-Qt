#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QtWidgets>
#include "server.h"
#include <QVector>
#include <QString>
#include <QtCore>
#include <QtGui>
#include <QtNetwork>

class QLabel;
class QTcpServer;
class QNetworkSession;

class ServerWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ServerWindow(QWidget *parent = nullptr);

signals:

public slots:
    void sessionOpened();
    void sendData();
private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;

    /*
    QLabel *ipClientLabel;
    QLabel *dbNumberLabel;
    QLabel *ipPLCLabel;
    QLabel *rackLabel;
    QLabel *slotLabel;
    PLCNumber *plcNumber;
    DBNumber *dbNumber;
    Server *server;


    QLineEdit *ipClientLineEdit;
    QLineEdit *dbNumberLineEdit;
    QLineEdit *ipPLCLineEdit;
    QLineEdit *rackLineEdit;
    QLineEdit *slotLineEdit;

    QPushButton *startServer;
    QPushButton *quitServerWindow;
    QDialogButtonBox *buttonBox;
    */

};

#endif // SERVERWINDOW_H
