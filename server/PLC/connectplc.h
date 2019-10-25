#ifndef CONNECTPLC_H
#define CONNECTPLC_H
#include <QtWidgets>
#include "snap7/snap7.h"

class ConnectPLC : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectPLC(QWidget *parent = nullptr);
    ~ConnectPLC();
    word MyDB32[20];
    TS7Client *client;
    void UnitStatus();
    bool Check(int, const char*);

private slots:
    void requestConnection();
    void readData(int number, int beginDB, int endDB);

private:
    QLabel *hostLabel;
    QLabel *rackLabel;
    QLabel *slotLabel;
    QLabel *rangeLabel;
    QLabel *beginLabel;
    QLabel *endLabel;
    QLabel *statusLabel;

    QLineEdit *hostLineEdit;
    QLineEdit *rackLineEdit;
    QLineEdit *slotLineEdit;
    QLineEdit *rangeLineEdit;
    QLineEdit *beginLineEdit;
    QLineEdit *endLineEdit;
    QPushButton *getConnection;
    QPushButton *quitButton;
    QString currentTime;
    QDialogButtonBox *buttonBox;
};

#endif // CONNECTPLC_H
