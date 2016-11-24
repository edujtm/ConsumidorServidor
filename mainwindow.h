#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <plotterdata.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void getData();
  void tcpConnect();
  void tcpDisconnect();

private slots:
  void on_horizontalSliderTiming_valueChanged(int value);

  void on_pushButtonUpdate_clicked();

  void on_pushButtonStop_clicked();

private:
  bool started;
  QPoint pf;
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QString selectedIP;
  QStringList listIP;
};

#endif // MAINWINDOW_H
