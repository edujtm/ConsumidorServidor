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
    QList<QPoint> points;
    QTimer *timer;
    bool started;

public:
    PlotterData(QWidget *parent);
    void addPoint(QPoint point);
    void clearPoints();
    void clearScreen();
    bool isStarted();
    void startTimer();
     void stopTimer();
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // PLOTTERDATA_H
