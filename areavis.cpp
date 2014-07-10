#include "areavis.h"
#include "qdebug.h"
#include <iostream>

int startX  = 20;
int scale   = 200;

AreaVis::AreaVis(QWidget *parent) : QWidget(parent){
    on      = false;
    track   = false;
}

void AreaVis::turnVisOn(){
    on = true;
    std::cout << "areaVis is ON " << std::endl;
}

void AreaVis::turnVisOff(){
    on = false;
    std::cout << "areaVis is OFF " << std::endl;
}

void AreaVis::turnTrackOn(int fn, int f){
    track       = true;
    startFrm    = f;
    step        = float(this->width()-startX)/float(fn-f);
}

void AreaVis::turnTrackOff(){
    track = false;
}

void AreaVis::updateArea(int a, int currFrame){
    if(currFrm == startFrm+2)
        startArea = a;
    area = a;
    currFrm = currFrame;

    if(currFrm - startFrm - 2 > 0){
        int x = startX + step*(currFrm-startFrm-2);
        int y = this->height()/2 + (area-startArea)*gridStepY/scale;
        currPoint = QPoint(x, y);
        polyline <<  currPoint;
    }

}

void AreaVis::paintEvent(QPaintEvent *event)
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

    // draw area
    myPen.setColor(QColor(153, 204, 49)); //green color
    myPen.setWidth(10);
    myPen.setCapStyle(Qt::RoundCap);
    painter.setPen(myPen);

    if(on){
        //initial line
        painter.drawLine(0, (this->height()/2), startX, (this->height()/2));

        if (track && currFrm>= startFrm+2){
            //realtime line chart
            painter.drawPolyline(polyline);
            myPen.setWidth(20);
            painter.setPen(myPen);
            painter.drawPoint(currPoint);

            myPen.setColor(QColor(128, 128, 128));
            myPen.setWidth(3);
            painter.setPen(myPen);
            //draw text showing the cell area
            QRectF rect = QRectF(QPointF(currPoint.x()-30, currPoint.y()-70),
                                 QPointF(currPoint.x()+30, currPoint.y()-30));
            QString textArea = QString::number(area);
            painter.drawText(rect, Qt::AlignCenter, textArea);
        }
    }
    /*
//        QRectF rectangle(30.0, 50.0, 500.0, 300.0);
//        painter.drawRoundedRect(rectangle, 20.0, 15.0);

//    //draw an ellipse
//    //The setRenderHint() call enables antialiasing, telling QPainter to use different
//    //color intensities on the edges to reduce the visual distortion that normally
//    //occurs when the edges of a shape are converted into pixels
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap));
//    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
//    painter.drawEllipse(200, 80, 400, 240); */
    this->update();
}
