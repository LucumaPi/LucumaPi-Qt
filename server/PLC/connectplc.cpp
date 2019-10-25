#include <iostream>
#include "connectplc.h"
#include<sstream>


ConnectPLC::ConnectPLC(QWidget *parent) : QDialog(parent)
{
    this->client = new TS7Client();
    this->hostLabel = new QLabel(tr("IP :"));
    this->rackLabel = new QLabel(tr("Rack"));
    this->slotLabel = new QLabel(tr("Slot"));
    this->rangeLabel = new QLabel(tr("DB#"));
    this->beginLabel = new QLabel(tr("Begin DB#"));
    this->endLabel = new QLabel(tr("End DB#"));

    this->hostLineEdit = new QLineEdit("192.168.1.20");
    this->rackLineEdit = new QLineEdit("0");
    this->rackLineEdit->setValidator(new QIntValidator(1, 20, this));

    this->slotLineEdit = new QLineEdit("1");
    this->slotLineEdit->setValidator(new QIntValidator(1, 20, this));

    this->rangeLineEdit = new QLineEdit("");
    this->rangeLineEdit->setValidator(new QIntValidator(1, 200, this));

    this->beginLineEdit = new QLineEdit("");
    this->beginLineEdit->setValidator(new QIntValidator(0, 200, this));

    this->endLineEdit = new QLineEdit("");
    this->endLineEdit->setValidator(new QIntValidator(0,200, this));

    this->hostLabel->setBuddy(this->hostLineEdit);
    this->rackLabel->setBuddy(this->rackLineEdit);
    this->slotLabel->setBuddy(this->slotLabel);
    this->rangeLabel->setBuddy(this->rangeLineEdit);
    this->beginLabel->setBuddy(this->beginLineEdit);
    this->endLabel->setBuddy(this->endLineEdit);

    this->statusLabel = new QLabel(tr("waiting for connection..."));

    this->getConnection = new QPushButton(tr("Connect to PLC"));
    this->getConnection->setDefault(true);

    this->quitButton = new QPushButton(tr("Quit"));

    this->buttonBox = new QDialogButtonBox;
    this->buttonBox->addButton(this->getConnection, QDialogButtonBox::ActionRole);
    this->buttonBox->addButton(this->quitButton, QDialogButtonBox::RejectRole);

    connect(getConnection, SIGNAL(clicked()), this, SLOT(requestConnection()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(hostLabel,0,0);
    mainLayout->addWidget(hostLineEdit,0,1);
    mainLayout->addWidget(rackLabel,1,0);
    mainLayout->addWidget(rackLineEdit,1,1);
    mainLayout->addWidget(slotLabel,2,0);
    mainLayout->addWidget(slotLineEdit,2,1);
    mainLayout->addWidget(rangeLabel,3,0);
    mainLayout->addWidget(rangeLineEdit,3,1);
    mainLayout->addWidget(beginLabel, 4, 0);
    mainLayout->addWidget(beginLineEdit, 4, 1);
    mainLayout->addWidget(endLabel, 5, 0);
    mainLayout->addWidget(endLineEdit, 5, 1);
    mainLayout->addWidget(statusLabel,6,0,1,2);
    mainLayout->addWidget(buttonBox,7,0, 1, 2);
    setLayout(mainLayout);
    setWindowTitle(tr("PLC view"));
}

void ConnectPLC::requestConnection(){
    QString x = hostLineEdit->text();
    QByteArray h = x.toLocal8Bit();
    const char *c_str2 = h.data();
    printf("%s \n", c_str2);

    this->client->ConnectTo(c_str2, rackLineEdit->text().toInt(), slotLineEdit->text().toInt());
    UnitStatus();
    readData(rangeLineEdit->text().toInt(), beginLineEdit->text().toInt(),endLineEdit->text().toInt());
}

void ConnectPLC::readData(int numberDB, int beginDB, int endDB){
    byte hibyte;
    byte lobyte;
    word value;
    std::stringstream ss;
    this->client->DBRead(numberDB, beginDB, endDB, &MyDB32);

    for (int x= 0; x< endDB;x++) {
        value = MyDB32[x];
        hibyte = (value & 0xff00) >> 8;
        lobyte = (value & 0xff);
        value = lobyte << 8 | hibyte;
        ss << value << "\n";
    }
    //printf("Buffer : %s\n", value);

    std::string s = ss.str();
    std::cout << s << std::endl;
    const char *tt = s.c_str();
    printf("%s \n", tt);
}

bool ConnectPLC::Check(int Result, const char* function){
    printf("\n");
    printf("+-----------------------------------------------------\n");
    printf("| %s\n",function);
    printf("+-----------------------------------------------------\n");
   if (Result==0) {
       printf("| Result         : OK\n");
       printf("| Execution time : %d ms\n", this->client->ExecTime());
       printf("+-----------------------------------------------------\n");
   }else {
       printf("| ERROR !!! \n");
       if (Result<0){
        printf("| Library Error (-1)\n");
       }else{
         printf("| %s\n",CliErrorText(Result).c_str());
         printf("+-----------------------------------------------------\n");
       }
   }
   return Result==0;
}

void ConnectPLC::UnitStatus(){
    int res =0;
    int Status = this->client->PlcStatus();
    if(this->Check(res, "CPU Status")){
        switch(Status){
            case S7CpuStatusRun:
                printf("RUN  \n");
                break;
            case S7CpuStatusStop:
                printf("STOP  \n");
                break;
            default:
                printf("UNKOWN\n");
                break;
        }
    }
}
ConnectPLC::~ConnectPLC(){

}
