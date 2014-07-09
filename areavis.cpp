#include "areavis.h"
#include "qdebug.h"
#include <iostream>

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

void AreaVis::turnTrackOn(){
    track = true;
}

void AreaVis::turnTrackOff(){
    track = false;
}

void AreaVis::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen myPen(QColor(128, 128, 128, 50));
    myPen.setWidth(2);
    painter.setPen(myPen);

    // draw background grid
    int stepX = 20;
    int stepY = 20;
    for(int j = 0; j < this->height(); j+=stepY )
        painter.drawLine(0, j, this->width(), j);
    for(int i = 0; i < this->width(); i+=stepX )
        painter.drawLine(i, 0, i, this->height());

    myPen.setColor(QColor(153, 204, 49)); //green color
    myPen.setWidth(10);
    myPen.setCapStyle(Qt::RoundCap);
    painter.setPen(myPen);
    if(on){
//        QRectF rectangle(30.0, 50.0, 500.0, 300.0);
//        painter.drawRoundedRect(rectangle, 20.0, 15.0);
        painter.drawLine(0, (this->height()/2), 20, (this->height()/2));
        myPen.setWidth(16);
        painter.setPen(myPen);
        painter.drawPoint(20, this->height()/2);
        if (track){

        }
    }



    /*
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
