#include "plotterdata.h"

PlotterData::PlotterData(QWidget *parent) : QWidget(parent)
{
    started = false;

    this->setBackgroundRole(QPalette::Window);

    timer = new QTimer(this);

    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(repaint()));

}

void PlotterData::addPoint(QPoint point) {
    points.append(point);
}

void PlotterData::clearPoints() {
    points.clear();
}

void PlotterData::clearScreen() {

}

void PlotterData::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QPen pointpen, linepen;
    if(points.size() > 2) {
        pointpen.setColor(Qt::black);
        pointpen.setWidth(6);
        linepen.setColor(Qt::red);
        linepen.setWidth(2);
        for(int i=1; i<points.size(); i++){
            painter.setPen(pointpen);
            painter.drawPoint(points.at(i));

            painter.setPen(linepen);
            painter.drawLine(points.at(i-1), points.at(i));
        }
    }
}

void PlotterData::startTimer() {
    if(!isStarted()) {
        timer->start(1000);
        started = true;
    }
}

void PlotterData::stopTimer() {
    if(isStarted()){
        timer->stop();
        started = false;
    }
}

bool PlotterData::isStarted() {
    return started;
}
