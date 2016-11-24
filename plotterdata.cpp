#include "plotterdata.h"

PlotterData::PlotterData(QWidget *parent) : QWidget(parent)
{
    started = false;

    this->setBackgroundRole(QPalette::Base);

    timer = new QTimer(this);

    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(repaint()));

}

void PlotterData::addValue(int value) {
    if(values.size() < 10) {
        values.append(value);
    } else {
        values.pop_front();
        for(int i = 1; i<values.size(); i++){
            values.swap(i-1, i);
        }
        values.append(value);
    }
}

void PlotterData::clearValues() {
    values.clear();
}

void PlotterData::clearScreen() {

}

void PlotterData::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QPen pointpen, linepen;
    QPoint p1, p2;
    float scalefactor = this->height()/100;
    if(values.size() > 2) {
        pointpen.setColor(Qt::black);
        pointpen.setWidth(6);
        linepen.setColor(Qt::red);
        linepen.setWidth(2);


        p1.setX(0);
        p1.setY(this->height() - (int) (values.at(0)*scalefactor));

        for(int i=1; i<values.size(); i++){

            p2.setX(i * (int)(this->width()/10));
            p2.setY(this->height() - (int)(values.at(i)*scalefactor));

            painter.setPen(pointpen);
            painter.drawPoint(p2);

            painter.setPen(linepen);
            painter.drawLine(p1, p2);
            p1 = p2;
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
