#include "plotterdata.h"

PlotterData::PlotterData(QWidget *parent) : QWidget(parent)
{

    this->setBackgroundRole(QPalette::Base);

}

void PlotterData::addValue(int value) {
    if(values.size() < 10) {
        values.append(value);
    } else {
        values.pop_front();
        for(int i = 1; i<values.size(); i++){
            values.replace(i-1, values.at(i));
        }
        values.append(value);
    }
}

void PlotterData::clearValues() {
    values.clear();
}

void PlotterData::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QPen pointpen, linepen, borderpen;
    QPoint p1, p2;

    borderpen.setColor(Qt::gray);
    painter.setPen(borderpen);
    painter.drawRect(0,0,this->width()-1, this->height()-1);

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
