#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  tcpConnect();


  connect(ui->pushButtonConnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpConnect()));

  connect(ui->pushButtonDisconnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpDisconnect()));

  connect(ui->pushButtonDisconnect,
          SIGNAL(clicked(bool)),
          ui->listWidgetIP,
          SLOT(clear()));

  connect(ui->pushButtonStart,
          SIGNAL(clicked(bool)),
          this,
          SLOT(getData()));
}

void MainWindow::tcpConnect(){
  QString string;

  socket->connectToHost(ui->lineEditIP->text(),1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
    socket->write("list");
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    qDebug() << socket->bytesAvailable();
    while(socket->bytesAvailable()) {
        string = socket->readLine().replace("\r","").replace("\n","");
        listIP = string.split(" ");
        for(int i = 0; i < listIP.size(); ++i){
            ui->listWidgetIP->addItem(listIP.at(i));
            qDebug() << listIP.at(i);
        }
    }
  }
  else{
    qDebug() << "Disconnected";
  }
}

void MainWindow::tcpDisconnect() {
    if(socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        if(socket->state() == QAbstractSocket::UnconnectedState) {
            qDebug() << "Disconnected";
        } else {
            qDebug() << "Connected";
        }
    }
}

//      Este método pega os dados do servidor e manda para a plotter.

void MainWindow::getData(){

  QString str, strConnect;
  QByteArray array;
  QStringList list;
  QDateTime datetime;

  qDebug() << "to get data...";
  if(socket->state() == QAbstractSocket::ConnectedState){
    if(!selectedIP.isEmpty()) {
        if(socket->isOpen()){

            //Espaço entre os dados desenhados na plotter
            //int dx = ui->plotterData->width()/10;
            //int dy;

            //float scalefactor = ui->plotterData->height()/100;          //  Valor que redimensiona os valores
                                                                        //  Para que o valor 100 fique no topo
                                                                        //  e 0 embaixo na plotter.

            //limpa os pontos da lista de pontos que foram desenhados na Plotter
            ui->plotterData->clearValues();

            //Ponto que será utilizados no desenhos das linhas com base nos valores retornados no get
            //QPoint pf;
            //int i = dx;               //Variavel que determinará o eixo x


            //Escrevendo no metodo get no servidor e esperando resposta.
            qDebug() << "reading...";
            strConnect = "get "+ selectedIP +"\r\n";
            socket->write(strConnect.toStdString().c_str());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();

            //se o servidor responder, cria-se o primeiro ponto que irá ficar na posição (0, 0)
            //dy = ui->plotterData->height();
            //pf = QPoint(0, dy);
            ui->plotterData->addValue(0);

            while(socket->bytesAvailable()){

                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    datetime.fromString(list.at(0),Qt::ISODate);
                    str = list.at(1);
                    qDebug() << datetime << ": " << str;
                }

                //dy = (int) (ui->plotterData->height() - std::stoi(str.toStdString().c_str()) * scalefactor);
                //pf  = QPoint(i, dy);
                ui->plotterData->addValue(std::stoi(str.toStdString().c_str()));
                //i += dx;
            }
            ui->plotterData->startTimer();
        }
    } else {
        qDebug() << "Select IP and update first.";
    }
  }
}


MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}

void MainWindow::on_horizontalSliderTiming_valueChanged(int value)
{
    ui->labelTiming->setNum((double)value/10);
}

void MainWindow::on_pushButtonUpdate_clicked()
{
     if(ui->listWidgetIP->currentRow() != -1){
        selectedIP = ui->listWidgetIP->currentItem()->text();
        ui->plotterData->clearScreen();
     }
}


void MainWindow::on_pushButtonStop_clicked()
{
    ui->plotterData->stopTimer();
}
