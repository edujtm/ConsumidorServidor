#ifndef PLOTTERDATA_H
#define PLOTTERDATA_H

#include <QWidget>
#include <QList>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QtGui>

class PlotterData : public QWidget
{
    Q_OBJECT
private:
    QList<int> values;
    QTimer *timer;
    bool started;

public:
    PlotterData(QWidget *parent);
    void addValue(int value);
    void clearValues();
    void clearScreen();
    bool isStarted();
    void startTimer();
     void stopTimer();
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // PLOTTERDATA_H
