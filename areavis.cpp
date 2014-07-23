#include "areavis.h"
#include "qdebug.h"
#include <iostream>

int startX  = 20;
int scale   = 100;

DataVis::DataVis(QWidget *parent) : QWidget(parent){
    on      = false;
    track   = false;
}

DataVis::~DataVis(){

}

void DataVis::turnVisOn(){
    on = true;
    std::cout << "areaVis is ON " << std::endl;
}

void DataVis::turnVisOff(){
    on = false;
    std::cout << "areaVis is OFF " << std::endl;
}

void DataVis::turnTrackOn(int fn, int f){
    track       = true;
    startFrm    = f;
    step        = float(this->width()-startX)/float(fn-f);
}

void DataVis::turnTrackOff(){
    track = false;
}

void DataVis::updateData(int a, int perimtr, int currFrame){
    if(currFrm == startFrm+2){
        startArea = a;
        startPerimeter = perimtr;
    }
    area = a;
    perimeter = perimtr;
    currFrm = currFrame;

    if(currFrm - startFrm - 2 > 0){
        int x = startX + step*(currFrm-startFrm-2);
        int y_a = this->height()/2 + (area-startArea)*gridStepY/scale;
        int y_p = this->height()/2 + (perimeter-startPerimeter)*gridStepY/scale;
        currPoint_area = QPoint(x, y_a);
        currPoint_perimeter = QPoint(x, y_p);
        polyline_area << currPoint_area;
        polyline_perimeter << currPoint_perimeter;
    }
}

void DataVis::releaseAreaVis(){
    std::cout << "Area visualization is off." << std::endl;
    track   = false;
    on      = false;
    area = 0;
    perimeter = 0;
    polyline_area.clear(); // clear polyline
    currPoint_area = QPoint(); // construct a null point
    polyline_perimeter.clear(); // clear polyline
    currPoint_perimeter = QPoint(); // construct a null point
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

    // pen for drawing area line chart
    myPen.setColor(QColor(153, 204, 49)); //green color
    myPen.setWidth(10);
    myPen.setCapStyle(Qt::RoundCap);
    painter.setPen(myPen);

    if(on){
        ////initial line
        //painter.drawLine(0, (this->height()/2), startX, (this->height()/2));


        if (track && (currFrm >= startFrm+2)){
            //realtime line chart - area
            painter.drawPolyline(polyline_area);
            myPen.setWidth(20);
            painter.setPen(myPen);
            painter.drawPoint(currPoint_area);

            //draw text showing the cell area
            //myPen.setColor(QColor(128, 128, 128));
            myPen.setWidth(4);
            painter.setPen(myPen);
            QRectF rect_a = QRectF(QPointF(currPoint_area.x()-50, currPoint_area.y()-70),
                                 QPointF(currPoint_area.x()+50, currPoint_area.y()-30));
            QString textArea = /*"cell area: "+ */QString::number(area);
            painter.drawText(rect_a, Qt::AlignCenter, textArea);


            // pen for drawing perimeter line chart
            myPen.setColor(QColor(251, 172, 81)); //orange color
            myPen.setWidth(10);
            painter.setPen(myPen);
            //realtime line chart - area
            painter.drawPolyline(polyline_perimeter);
            myPen.setWidth(20);
            painter.setPen(myPen);
            painter.drawPoint(currPoint_perimeter);

            //draw text showing the cell perimeter
            //myPen.setColor(QColor(128, 128, 128));
            myPen.setWidth(4);
            painter.setPen(myPen);
            QRectF rect_p = QRectF(QPointF(currPoint_perimeter.x()-50, currPoint_perimeter.y()+30),
                                 QPointF(currPoint_perimeter.x()+50, currPoint_perimeter.y()+70));
            QString textPeri = /*"cell area: "+ */QString::number(perimeter);
            painter.drawText(rect_p, Qt::AlignCenter, textPeri);


        }
    }

    this->update();
}
