#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  timer = new QTimer(this);
  started = false;
  aux = 0;
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
          SLOT(startTimer()));

  connect(ui->pushButtonStop,
          SIGNAL(clicked(bool)),
          this,
          SLOT(stopTimer()));

  connect(timer,
          SIGNAL(timeout()),
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



            //limpa os pontos da lista de pontos que foram desenhados na Plotter
            ui->plotterData->clearValues();

            //Escrevendo no metodo get no servidor e esperando resposta.
            qDebug() << "reading...";
            strConnect = "get "+ selectedIP +"\r\n";
            socket->write(strConnect.toStdString().c_str());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();

            //se o servidor responder, cria-se o primeiro ponto que irá ficar na posição (0, 0)
            ui->plotterData->addValue(0);

            while(socket->bytesAvailable()){

                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    datetime.fromString(list.at(0),Qt::ISODate);
                    str = list.at(1);
                    qDebug() << datetime << ": " << str;
                }

                ui->plotterData->addValue(std::stoi(str.toStdString().c_str()));
            }
            ui->plotterData->repaint();
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
    if(isStarted()){
        timer->setInterval(value*100);
    }
}

void MainWindow::on_pushButtonUpdate_clicked()
{
     if(ui->listWidgetIP->currentRow() != -1){
        selectedIP = ui->listWidgetIP->currentItem()->text();
     }
}

void MainWindow::startTimer(){
    if(socket->state() == QAbstractSocket::ConnectedState && !isStarted()) {
        timer->start(1000);
        started = true;
    }
}

void MainWindow::stopTimer() {
    if(isStarted()){
        timer->stop();
        started = false;
    }
}

bool MainWindow::isStarted() {
    return started;
}
