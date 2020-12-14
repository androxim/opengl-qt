#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    openglw = new MyWidget();
    openglw->mww = this;
    openglw->setWindowTitle("OpenGL Qt");

    mwconnected = false;

    curr_att = prev_att = mw_attent = 0;

    strListM1 = new QStringListModel();
    strListM1->setStringList(strList1);
    ui->listView->setModel(strListM1);
    ui->listView->setAutoScroll(true);

    // timer for processing data from MindWave device, attention value is received one per second
    mindwt = new QTimer(this);
    mindwt->connect(mindwt, SIGNAL(timeout()), this, SLOT(mindwtUpdate()));
    mindwt->setInterval(1);

    // timer for filling attention value continuously between previous and new values
    atten_timer = new QTimer(this);
    atten_timer->connect(atten_timer, SIGNAL(timeout()), this, SLOT(atten_timerUpdate()));
    atten_timer->setInterval(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printdata(QString str) // updating execution log
{
    strList1.push_front(str);
    strListM1->setStringList(strList1);
}

void MainWindow::mindwaveconnect() // function to connect to MindWave device
{
    char *comPortName = NULL;
    char *portNumber = (char*)malloc(sizeof(char) * (2 + 1));
    const char *comPortBase = NULL;
    int   errCode = 0;
    int comPortFound = 0;
    const int MAX_PORT = 16;
    size_t length = 0;
    int i = 0;
    int j = 0;

    int dllVersion = TG_GetDriverVersion();
    printdata("ThinkGear DLL version: "+QString::number(dllVersion));
    connectionId = TG_GetNewConnectionId();
    if( connectionId < 0 )
        printdata("ERROR: TG_GetNewConnectionId() returned: "+QString::number(connectionId));
    else
        printdata("ThinkGear Connection ID is: "+QString::number(connectionId));

    printdata("Scanning COM ports 0 to "+QString::number(MAX_PORT));
    comPortBase = "\\\\.\\COM";
    length = strlen(comPortBase);
    comPortName = (char *)realloc (comPortName, (length + 5)*sizeof(char));

    for(i=0; i <= MAX_PORT && comPortFound == 0; i++)
    {

        portNumber = itoa(i, portNumber, 10);
        strcpy(comPortName,comPortBase);

        for(j=0; j<(int)strlen(portNumber); j++)
            comPortName[length+j] = portNumber[j];

        comPortName[length+strlen(portNumber)] = '\0';

        printdata("trying to connect on "+QString::fromUtf8(comPortName));
        errCode = TG_Connect( connectionId,comPortName,TG_BAUD_9600,TG_STREAM_PACKETS );
        if( errCode < 0 )
            printdata("ERROR: TG_Connect() returned");
        else
        {
            // Trying to read one packet to check the connection.
            printdata("Connection available... wait...");
            Sleep(3000); // sometimes we need to wait a little...
            errCode = TG_ReadPackets(connectionId, 1);
            if(errCode >= 0)
            {
                printdata("OK");
                comPortFound = 1;
                break;
            }
        }
    } 	/* end: "Attempt to connect the connection ID handle to serial ports between COM0 and "COM16"" */

    if (comPortFound == 1)
    {
        printdata("Connection with MindWave established! :)");
        mwconnected = true;
        mindwt->start();
        atten_timer->start();
    }
    else
        printdata("Connection with MindWave could not be established! :(");
}

void MainWindow::mindwtUpdate() // processing data from MindWave device
{
    int errCode = TG_ReadPackets( connectionId,1);
    if( errCode == 1 )
    {
        if ( TG_GetValueStatus(connectionId, TG_DATA_ATTENTION) != 0 )
        {
            if (TG_GetValue(connectionId, TG_DATA_ATTENTION)>0)
            {
                mw_attent = TG_GetValue(connectionId, TG_DATA_ATTENTION);
                curr_att = mw_attent;
            }
        }
    }
}

void MainWindow::atten_timerUpdate() // filling attention value continuously between previous and new values
{
    if (abs(curr_att-prev_att)>0)
    {
        if (curr_att<prev_att)
            prev_att-=1;
        else
            prev_att+=1;

        ui->progressBar->setValue(prev_att);

        angle_change = ((double)(100-prev_att)/100)*4;
        if (angle_change==0)
            angle_change = 0.1;

        if ((prev_att<50) && (scale>0.4))
            scale -= 0.05;
        else if ((prev_att>50) && (scale<1.3))
            scale += 0.05;

        openglw->set_angle_scale_incs(angle_change,scale);
    }
}

void MainWindow::on_pushButton_clicked()
{
    openglw->startflow(25);
    openglw->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    mindwaveconnect();
}

void MainWindow::on_pushButton_3_clicked()
{
    openglw->close();
    close();
}

