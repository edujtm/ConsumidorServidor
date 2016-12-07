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

  //Desabilita os botões de Start e Stop
  ui->pushButtonStart->setEnabled(false);
  ui->pushButtonStop->setEnabled(false);

  //Conecta o botão de conectar ao slot tcpConnect que irá fazer a conexão com o servidor
  connect(ui->pushButtonConnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpConnect()));

  //Conecta o botão de desconectar ao slot tcpDisconnect que irá desconectar do servidor
  connect(ui->pushButtonDisconnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpDisconnect()));

  //Conecta o botão disconecta ao slot clear que limpa os ips do ListWidget
  connect(ui->pushButtonDisconnect,
          SIGNAL(clicked(bool)),
          ui->listWidgetIP,
          SLOT(clear()));

  //Conecta o botão start ao start timer que irá começar a receber os dados
  connect(ui->pushButtonStart,
          SIGNAL(clicked(bool)),
          this,
          SLOT(startTimer()));

  //Conecta o botão stop ao slot stopTimer() que irá parar o timer
  connect(ui->pushButtonStop,
          SIGNAL(clicked(bool)),
          this,
          SLOT(stopTimer()));

  //Conecta o timer com o slot getData() para que se consiga pegar as informações e as alterações do servidor
  connect(timer,
          SIGNAL(timeout()),
          this,
          SLOT(getData()));
}


void MainWindow::tcpConnect(){
  QString string;
  //Conecta ao servidor cujo IP foi digitado no primeiro LineEdit
  socket->connectToHost(ui->lineEditIP->text(),1234);
  if(socket->waitForConnected(3000)){
    //Caso ele consiga conectar, espera a resposta do list no servidor para que depois possa ler
    qDebug() << "Connected";
    socket->write("list");
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    qDebug() << socket->bytesAvailable();
    //Lê os IPs que serão escritos como resposta do servidor e armazena-os em uma lista de strings
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
    //Se estiver desconectado, não precisa desconectar.
    if(socket->state() == QAbstractSocket::ConnectedState) {
        //Disconeta do servidor e imprime uma resposta no debug caso tenha sucesso ou não
        socket->disconnectFromHost();
        if(socket->state() == QAbstractSocket::UnconnectedState) {
            ui->pushButtonStart->setEnabled(false);
            ui->pushButtonStop->setEnabled(false);
            if(isStarted()){
                timer->stop();
                started = false;
            }
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

            //após o servidor responder, adiciona o primeiro valor  na lista de valores da classe PlotterData
            ui->plotterData->addValue(0);

            //Lê cada um dos valores retornados pelo servidor e manda-os para a lista de valores da classe PlotterData
            while(socket->bytesAvailable()){

                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    datetime.fromString(list.at(0),Qt::ISODate);
                    str = list.at(1);
                    qDebug() << datetime << ": " << str;
                }
                ui->plotterData->addValue(stoi(str.toStdString().c_str(), str.length()));
            }
            //Chama o metodo repaint da plotter que irá atualizar o desenho de acordo com os valores passados
            ui->plotterData->repaint();
        }
  }
}

//Destrutor da MainWindow
MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}

//Slot que altera o tempo entre os gets no servidor
void MainWindow::on_horizontalSliderTiming_valueChanged(int value)
{
    ui->labelTiming->setNum((double)value/10);
    if(isStarted()){
        timer->setInterval(value*100);
    }
}
//Slot que é ativado quando o botão update é clicado
void MainWindow::on_pushButtonUpdate_clicked()
{
    //Se o usuário tiver escolhido um IP, atualiza a variável que guarda o ip selecionado
     if(ui->listWidgetIP->currentRow() != -1){
        selectedIP = ui->listWidgetIP->currentItem()->text();
        //Habilita os botões de start e stop, já que um IP foi selecionado
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
        qDebug() << "updated";
     }
}
//Slot que é ativado quando o botão Start é clicado.
void MainWindow::startTimer(){
    //Inicia o timer que irá chamar o método getData.
    if(socket->state() == QAbstractSocket::ConnectedState && !isStarted()) {
        timer->start(1000);
        started = true;
    }
}

//Slot que é ativado quando o botão Stop é clicado.
void MainWindow::stopTimer() {
    if(isStarted()){
        timer->stop();
        started = false;
    }
}

//Retorna se o timer está iniciado ou não.
bool MainWindow::isStarted() {
    return started;
}

//Função que converte um numero em string para inteiro
int MainWindow::stoi(std::string numero, int tam)
{
    int valor = 0;
    for(int i=0; i<tam; i++)
    {
        valor += (numero[i] - '0') * pow(10, tam - (1+i));
    }
    return valor;
}
