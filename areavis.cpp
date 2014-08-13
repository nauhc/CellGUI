#include "areavis.h"
#include "qdebug.h"
#include <iostream>
#include <QTimer>

int scaleX = 5;

DataVis::DataVis(QWidget *parent, QColor clr) : QWidget(parent){
    on      = false;
    track   = false;
    color   = clr;
//    string  = str;

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);
}

void DataVis::setMinMax(int v_min, int v_max){
    value_min = v_min;
    value_max = v_max;
}

DataVis::~DataVis(){

}

void DataVis::turnVisOn(){
    on = true;
    //std::cout << "dataVis is ON " << std::endl;
}

void DataVis::turnVisOff(){
    on = false;
    //std::cout << "dataVis is OFF " << std::endl;
}

void DataVis::turnTrackOn(int fn, int f){
    track       = true;
    startFrm    = f;
    step        = float(this->width()-75)/float(fn-f);
    scaleX      = int(25/step)+1;
    gridStepX   = int(step*scaleX);
    //std::cout << "step " << step << " scaleX " << scaleX <<  " gridStep " << gridStepX << std::endl;
    gridStepY   = int(this->height()/16);
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

    if(currFrm - startFrm > 2){
        int x = gridStepX*2 + step*(currFrm-startFrm-2);
        int y = (1.0+double(value_max-value)/double(value_max-value_min)*15.0)*double(gridStepY);
        currPoint_value = QPoint(x, y);
        polyline_value << currPoint_value;
    }
}

void DataVis::releaseDataVis(){
    //std::cout << "Area visualization is off." << std::endl;
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

    /*
    //text showing what is y-axis for
    QRectF rectY = QRectF(QPointF(60, 5), QPointF(250, 50));
    QString textY = string+" (pixels)";
    myPen.setColor(color);
    painter.setPen(myPen);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(rectY, Qt::AlignLeft, textY);*/

    if(on){
        ////initial line
        //painter.drawLine(0, (this->height()/2), gridStepX, (this->height()/2));

        if (track && (currFrm >= startFrm+2)){
            //draw y-axis with graduation
            myPen.setColor(QColor(160, 160, 160, 250));
            myPen.setWidth(3);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(myPen);
            painter.drawLine(2*gridStepX, 0, 2*gridStepX, this->height()/*-this->height()%gridStepX*/);
            for(int j = gridStepY; j < this->height(); j+=gridStepY)
                painter.drawLine(2*gridStepX-5, j, 2*gridStepX+5, j);

            //draw x-axis
            int verticalPos = gridStepY*15;
            painter.drawLine(2*gridStepX-5, verticalPos, this->width(), verticalPos);
            painter.drawLine(currPoint_value.x(), verticalPos-2, currPoint_value.x(), verticalPos+1);
            QRectF rect_f = QRectF(QPointF(currPoint_value.x()-15, verticalPos+5),
                                   QPointF(currPoint_value.x()+15, verticalPos+15));
            QString textFrm = QString::number(currFrm);
            painter.setFont(QFont("Arial", 14));
            painter.drawText(rect_f, Qt::AlignCenter, textFrm);
            painter.setFont(QFont("Arial", 17));
            QRectF rect_frmLbl = QRectF(QPointF(this->width()-55, verticalPos-20),
                                   QPointF(this->width()-5, verticalPos-5));
            QString textFrmLbl = "Frame";
            painter.drawText(rect_frmLbl, Qt::AlignLeft, textFrmLbl);

            // pen for drawing data line chart
            myPen.setColor(color);
            myPen.setWidth(3);
            myPen.setCapStyle(Qt::RoundCap);
            painter.setPen(myPen);

            // text for y-axis graduation
            for(int n = 0; n < 15; n+=2){
                QRectF rect = QRectF(QPointF(0, gridStepY*n+9), QPointF(gridStepX*2-5, gridStepY*(n+2)));
                QString textGrad = QString::number(value_max-n*(value_max-value_min)/14);
                if(n%4 == 0)
                    painter.setFont(QFont("Arial", 16));
                else
                    painter.setFont(QFont("Arial", 12));
                painter.drawText(rect, Qt::AlignRight, textGrad);
            }

            //realtime line chart - area
            painter.drawPolyline(polyline_value);
            myPen.setWidth(10);
            painter.setPen(myPen);
            painter.drawPoint(currPoint_value);

            //draw text showing the cell area
            myPen.setWidth(4);
            painter.setPen(myPen);
            QRectF rect_a = QRectF(QPointF(currPoint_value.x()-50, currPoint_value.y()-60),
                                   QPointF(currPoint_value.x()+50, currPoint_value.y()-20));
            QString textArea = QString::number(value);
            painter.setFont(QFont("Arial", 16));
            painter.drawText(rect_a, Qt::AlignCenter, textArea);
        }
    }

//    this->update(); //This consumes too much resources
}
