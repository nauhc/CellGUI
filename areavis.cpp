#include "areavis.h"
#include "qdebug.h"
#include <iostream>

int startX  = 20;
int scale   = 200;

DataVis::DataVis(QWidget *parent, QColor clr) : QWidget(parent){
    on      = false;
    track   = false;
    color   = clr;
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
    step        = float(this->width()-startX)/float(fn-f);
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
    for(int i = 0; i < this->width(); i+=gridStepX)
        painter.drawLine(i, 0, i, this->height());

    // pen for drawing data line chart
    myPen.setColor(color);
    myPen.setWidth(10);
    myPen.setCapStyle(Qt::RoundCap);
    painter.setPen(myPen);

    if(on){
        ////initial line
        //painter.drawLine(0, (this->height()/2), startX, (this->height()/2));


        if (track && (currFrm >= startFrm+2)){
            //realtime line chart - area
            painter.drawPolyline(polyline_value);
            myPen.setWidth(20);
            painter.setPen(myPen);
            painter.drawPoint(currPoint_value);

            //draw text showing the cell area
            //myPen.setColor(QColor(128, 128, 128));
            myPen.setWidth(4);
            painter.setPen(myPen);
            QRectF rect_a = QRectF(QPointF(currPoint_value.x()-50, currPoint_value.y()-70),
                                 QPointF(currPoint_value.x()+50, currPoint_value.y()-30));
            QString textArea = /*"cell area: "+ */QString::number(value);
            painter.drawText(rect_a, Qt::AlignCenter, textArea);


//            // pen for drawing perimeter line chart
//            myPen.setColor(QColor(251, 172, 81)); //orange color
//            myPen.setWidth(10);
//            painter.setPen(myPen);
//            //realtime line chart - area
//            painter.drawPolyline(polyline_perimeter);
//            myPen.setWidth(20);
//            painter.setPen(myPen);
//            painter.drawPoint(currPoint_perimeter);

//            //draw text showing the cell perimeter
//            //myPen.setColor(QColor(128, 128, 128));
//            myPen.setWidth(4);
//            painter.setPen(myPen);
//            QRectF rect_p = QRectF(QPointF(currPoint_perimeter.x()-50, currPoint_perimeter.y()+30),
//                                 QPointF(currPoint_perimeter.x()+50, currPoint_perimeter.y()+70));
//            QString textPeri = /*"cell area: "+ */QString::number(perimeter);
//            painter.drawText(rect_p, Qt::AlignCenter, textPeri);
        }
    }

    this->update();
}
