#include "plotterdata.h"

PlotterData::PlotterData(QWidget *parent) : QWidget(parent)
{
    //Muda a cor do background para Branco
    this->setBackgroundRole(QPalette::Base);

}

//Metodo que adiciona valores a serem plotados em um vetor de valores
void PlotterData::addValue(int value) {
    //Se houverem menos de 10 valores, apenas adiciona o valor ao vetor
    if(values.size() < 10) {
        values.append(value);
    //Se houverem mais de dez valores, coloca-se todos os valores para uma posição atrás
    //Retira o ultimo valor (que estará repetido na ultima posição e na penultima)
    //Adiciona o novo valor ao final do vetor
    } else {
        for(int i = 1; i<values.size(); i++){
            values.replace(i-1, values.at(i));
        }
        values.pop_back();
        values.append(value);
    }
}

//Limpa os valores do vetor
//Utilizado na função getData para que o metodo addValue não coloque os valores ao final
//do vetor.
void PlotterData::clearValues() {
    values.clear();
}

//Função paintEvent que redesenha a tela com os pontos
void PlotterData::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QPen pointpen, linepen, borderpen;
    QPoint p1, p2;

    //Desenha a borda da tela
    borderpen.setColor(Qt::gray);
    painter.setPen(borderpen);
    painter.drawRect(0,0,this->width()-1, this->height()-1);
    //Fator de normalização dos valores para que fiquem em tamanho proporcional a tela
    float scalefactor = this->height()/100;
    //Se houverem mais de dois valores, começa a desenhar as linhas
    if(values.size() > 2) {
        //Define as cores dos traços
        pointpen.setColor(Qt::black);
        pointpen.setWidth(6);
        linepen.setColor(Qt::red);
        linepen.setWidth(2);

        //Define o primeiro ponto
        p1.setX(0);
        p1.setY(this->height() - (int) (values.at(0)*scalefactor));

        // Desenha os pontos e as linhas de acordo com os valores do vetor values
        for(int i=1; i<values.size(); i++){

            p2.setX(i * (int)(this->width()/9));
            p2.setY(this->height() - (int)(values.at(i)*scalefactor));

            painter.setPen(pointpen);
            painter.drawPoint(p2);

            painter.setPen(linepen);
            painter.drawLine(p1, p2);
            p1 = p2;
        }
    }
}
