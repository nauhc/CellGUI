#include "areavis.h"
#include "qdebug.h"
#include <iostream>

int startX  = 20;
int scale   = 200;

DataVis::DataVis(QWidget *parent, QColor clr, QString str) : QWidget(parent){
    on      = false;
    track   = false;
    color   = clr;
    string  = str;
}

DataVis::~DataVis(){

}

void DataVis::turnVisOn(){
    on = true;
    std::cout << "dataVis is ON " << std::endl;
}

void DataVis::turnVisOff(){
    on = false;
    std::cout << "dataVis is OFF " << std::endl;
}

void DataVis::turnTrackOn(int fn, int f){
    track       = true;
    startFrm    = f;
    step        = float(this->width()-2*startX)/float(fn-f);
    gridStepX   = int(step)*3;
}

void DataVis::turnTrackOff(){
    track = false;
}

void DataVis::updateData(int v, int currFrame){
    if(currFrm == startFrm+2){
        startValue = v;
    }
    value = v;
    currFrm = currFrame;

    if(currFrm - startFrm - 2 > 0){
        int x = startX + step*(currFrm-startFrm-2);
        int y_a = this->height()/2 + (value-startValue)*gridStepY/scale;
        currPoint_value = QPoint(x, y_a);
        polyline_value << currPoint_value;
    }
}

void DataVis::releaseDataVis(){
    std::cout << "Area visualization is off." << std::endl;
    track   = false;
    on      = false;
    value = 0;
    polyline_value.clear(); // clear polyline
    currPoint_value = QPoint(); // construct a null point
}

void DataVis::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen myPen(QColor(128, 128, 128, 50));
    myPen.setWidth(2);
    painter.setPen(myPen);

    // draw background grid
    for(int j = 0; j < this->height(); j+=gridStepY)
        painter.drawLine(0, j, this->width(), j);
    for(int i = gridStepX; i < this->width(); i+=gridStepX)
        painter.drawLine(i, 0, i, this->height());

    //draw y-axis with graduation
    myPen.setColor(QColor(128, 128, 128, 250));
    myPen.setWidth(3);
    painter.setPen(myPen);
    painter.drawLine(2*gridStepX, 0, 2*gridStepX,
                     this->height()/*-this->height()%gridStepX*/);
    for(int j = gridStepY; j < this->height(); j+=gridStepY)
        painter.drawLine(2*gridStepX-10, j, 2*gridStepX+10, j);

    QRectF rectY = QRectF(QPointF(60, 5), QPointF(250, 50));
    QString textY = string+" (pixel)";
    myPen.setColor(color);
    painter.setPen(myPen);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(rectY, Qt::AlignLeft, textY);

//    //draw x-axis
//    painter.drawLine(0, this->height()-this->height()%gridStepX,
//                     this->width(), this->height()-this->height()%gridStepX);

    // pen for drawing data line chart
    myPen.setColor(color);
    myPen.setWidth(3);
    myPen.setCapStyle(Qt::RoundCap);
    painter.setPen(myPen);

    if(on){
        ////initial line
        //painter.drawLine(0, (this->height()/2), startX, (this->height()/2));

        if (track && (currFrm >= startFrm+2)){
            //realtime line chart - area
            painter.drawPolyline(polyline_value);
            myPen.setWidth(10);
            painter.setPen(myPen);
            painter.drawPoint(currPoint_value);

            //draw text showing the cell area
            //myPen.setColor(QColor(128, 128, 128));
            myPen.setWidth(4);
            painter.setPen(myPen);
            QRectF rect_a = QRectF(QPointF(currPoint_value.x()-50, currPoint_value.y()-60),
                                 QPointF(currPoint_value.x()+50, currPoint_value.y()-20));
            QString textArea = QString::number(value);
            painter.setFont(QFont("Arial", 16));
            painter.drawText(rect_a, Qt::AlignCenter, textArea);
        }
    }

    this->update();
}
