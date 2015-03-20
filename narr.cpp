#include <QResizeEvent>
#include <QPainter>
#include <math.h>
#include <QDebug>
#include <vector>
#include <iostream>
#include <QTimer>
#include "narr.h"

// 123.0/255.0, 57.0/255.0, 144.0/255.0     -> purple
// 28.0/255.0, 120.0/255.0, 159.0/255.0     -> blue
// 97.0/255.0, 149.0/255.0, 5.0/255.0       -> green
// 247.0/255.0, 154.0/255.0, 1.0/255.0      -> yellow
// 208.0/255.0, 81.0/255.0, 38.0/255.0      -> red
// 52.0/255.0, 56.0/255.0, 57.0/255.0       -> dark gray bg
// 193.0/255.0, 194.0/255.0, 196.0/255.0    -> light gray fg

QColor GREEN    = QColor(97, 149, 5);
QColor ORANGE   = QColor(247, 154, 1);
QColor BLUE     = QColor(28, 120, 159);
QColor PURPLE   = QColor(123, 57, 144);
QColor WHITE    = QColor(255, 255, 255);
QColor YELLOW   = QColor(247, 154, 1);
QColor RED      = QColor(208, 81, 38);

float BlebSizeMax = 10.0/*1.5*//*12*//*7*/;
float BlebSizeMin = 0.2/*2*/;

inline int larger(int a, int b){
    return (a > b ? a : b);
}

Narr::Narr(QWidget *parent, bool multv)
{
    clear();
     if(!multv){
        QTimer* timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(1);
    }
}

void Narr::clear()
{
    area.clear();
    perimeter.clear();
    blebNum.clear();
    blebAvgSize.clear();
    stage.clear();
    cells.clear();
    stage.push_back(0);
    cellImg.clear();

    begin       = 0;
    curr        = 0;
    max         = 1;
    angle       = 0;
    mouseIndex  = 0;

    //propType    = 1;
}

Narr::~Narr()
{

}

void Narr::setBeginFrm(int beginFrame)
{
    begin = beginFrame;
    //std::cout << "NARRATIVE VIS BEGIN frame number " << begin << std::endl;
}

void Narr::setMaxFrm(unsigned int m)
{
    max = m;
//    max = 5000;
    //std::cout << "NARRATIVE VIS MAX frame number " << max << std::endl;
}

void Narr::printAreaData(){
    for(unsigned int n = 0; n < area.size(); n++){
        std::cout << area[n] << " ";
    }
    std::cout << std::endl;
}

void Narr::updateProperty(floatArray prop, int currFrame)
{
    propSeq.push_back(prop);
    /*
    for(unsigned int i = 0; i < propSeq.size(); i++){
        floatArray p = propSeq[i];
        for(unsigned int j = 0; j < p.size(); j++){
            std::cout << p[j] << " ";
        }
        std:: cout << std::endl;
    }
    std:: cout << std::endl; */
    curr = currFrame;

//    property.push_back(float(frameIdx)); // 0
//    property.push_back(float(area)); // 1
//    property.push_back(float(perimeter)); //2
//    property.push_back(centroid.x); // 3
//    property.push_back(centroid.y); // 4
//    property.push_back(speed.x); // 5
//    property.push_back(speed.y); // 6
//    property.push_back(shape); // 7
//    property.push_back(blebs.size()); // 8
//    property.push_back(avg_blebsize * area_ratio); // 9

//    property.push_back(shape); // 5
//    property.push_back(blebs.size()); // 6
//    property.push_back(avg_blebsize); // 7

    area.push_back(prop[1]);
    perimeter.push_back(prop[2]);
    blebNum.push_back(prop[8]);
    blebAvgSize.push_back(prop[9]);
    //qDebug() << prop[9];
}

void Narr::updateCellImg(QImage &cell, QVector<QPoint> &smoothContour){
    //cell.save("cellImg"+QString::number(int(curr))+".png", "PNG");
    qreal cellScale = 0.6;
    QImage cell_conv = cell.convertToFormat(QImage::Format_ARGB32).scaled(cellScale*cell.width(),cellScale*cell.height(),Qt::KeepAspectRatio);
    cellImg.push_back(cell_conv);
    //qDebug() << cellImg.size();

    /*
    // need this when drawing contour if cell images
    QPolygon contour;
    for(int n = 0; n < smoothContour.size(); n++)
        contour << QPoint(smoothContour[n].x()/2-cell_conv.width()/2, smoothContour[n].y()/2-cell_conv.height()/2);

    contours.push_back(contour);*/
}

void Narr::updateCellImg(QImage &cell)
{
    qreal cellScale = 0.6;
    QImage cell_conv = cell.convertToFormat(QImage::Format_ARGB32).scaled(cellScale*cell.width(),cellScale*cell.height(),Qt::KeepAspectRatio);
    cellImg.push_back(cell_conv);

}

void Narr::updateStage(unsigned int index)
{
    stage.push_back(index);
}


void Narr::initializeGL()
{

}

QColor gradualColor (QColor color, qreal percent){
    return QColor( color.red() + (255-color.red()) * percent * percent,
                   color.green() + (255-color.green()) * percent * percent,
                   color.blue() + (255-color.blue()) * percent * percent );
}

void drawRingArc(QPainter   *painter,
                 QPointF    center,
                 qreal      startAngle,
                 qreal      endAngle,
                 qreal      innerRadius,
                 qreal      thickness,
                 QColor     color){ // clockwise

    qreal outerRadius = innerRadius + thickness;
    //        //debug
    //        painter->setPen(QColor(255, 255, 0));
    //        painter->drawRect(center.x() - outerRadius,
    //                          center.y() - outerRadius,
    //                          2 * outerRadius, 2 * outerRadius);

    QPen myPen(color);
    myPen.setCapStyle(Qt::FlatCap);
    myPen.setWidth(thickness);
    painter->setPen(myPen);

    painter->drawArc(center.x() - outerRadius + .5 * thickness,
                     center.y() - outerRadius + .5 * thickness,
                     2 * outerRadius - thickness, 2 * outerRadius - thickness,
                     -startAngle*16, -endAngle*16); //clockwise
    //QRectF rect = QRectF();
    //painter->drawText();

}

void Narr::drawCircularBarChart(QPainter *painter, std::vector<float> feature,
                                qreal innerRadius, qreal thickness,
                                qreal strtRto, QColor color) //clockwise
{
    if (feature.size() != 0){
        QPen myPen(color);
        myPen.setCapStyle(Qt::FlatCap);
        painter->setPen(myPen);
        painter->setBrush(QBrush(color));
        std::vector<float>::const_iterator max_it, min_it;
        max_it = std::max_element(std::begin(feature), std::end(feature));
        min_it = std::min_element(std::begin(feature), std::end(feature));
        //qDebug() << "MAX" << *max << "MIN" << *min;

        int number = feature.size();
        //qDebug() << number;
        for(int n = 0; n < number; n++){
            //qDebug() << feature[n];
            //rotate and translate from the center to location on the ring
            float degree = 360./(max-begin) * n;
            painter->rotate(degree);
            painter->translate(0, innerRadius);
            //draw a bar
            float barheight = float(feature[n] - (*min_it)) / (*max_it - *min_it) * thickness*(1-strtRto) + thickness *strtRto;
            float w = 2.*M_PI/(max-begin)*number;
            painter->drawRoundedRect(0, 0, w, barheight, 2.0, 2.0);
            // translate and rotate back to the center
            painter->translate(0, -innerRadius);
            painter->rotate(-degree);
        }
    }
}

void Narr::drawCircularLineChart(QPainter *painter, std::vector<float> feature,
                                 qreal innerRadius, qreal thickness,
                                 qreal strtRto, QColor color) //clockwise
{
    if (feature.size() != 0){
        QPen myPen(color);
        myPen.setCapStyle(Qt::FlatCap);
        painter->setPen(myPen);
        painter->setBrush(QBrush(color));
        std::vector<float>::const_iterator max_it, min_it;
        max_it = std::max_element(std::begin(feature), std::end(feature));
        min_it = std::min_element(std::begin(feature), std::end(feature));

        int number  = feature.size();
        QPolygon    polyline;
        QPoint      start;
        for(int n = 0; n < number; n++)
        {
            //rotate and translate from the center to location on the ring
            float degree = (360./(max-begin) * n + 90) * M_PI/180;
            float barheight = float(feature[n] - (*min_it)) / (*max_it - *min_it) * thickness*(1-strtRto) + thickness *strtRto;
            float radius    = barheight + innerRadius;
            QPoint point    = QPoint(radius*cos(degree), radius*sin(degree));
            if(n == 0)
                start = point;
            polyline << point;
        }
        //connect to the beginning
        //polyline << begin;
        //draw polyline
        painter->drawPolyline(polyline);
    }
}

void Narr::drawCircularBarChart_fixMax(QPainter *painter,
                                       std::vector<float> feature,
                                       qreal minV, qreal maxV,
                                       qreal innerRadius,
                                       qreal thickness,
                                       QColor color)
{
    if (feature.size() != 0){
        QPen myPen(color);
        myPen.setCapStyle(Qt::FlatCap);
        painter->setPen(myPen);
        painter->setBrush(QBrush(color));

        //int number = feature.size();
        int number = feature.size() < max ?  feature.size() : max;
        for(int n = 0; n < number; n++){
            //qDebug() << feature[n];
            //rotate and translate from the center to location on the ring
            float degree = 360./(max-begin) * n;
            painter->rotate(degree);
            painter->translate(0, innerRadius);
            //draw a bar
            //float barheight = float(feature[n]) * thickness/ maxV; // minV == 0;
            float barheight = float(feature[n] - minV) * thickness/ (maxV - minV);
            float w = /*2.*M_PI/(max-begin)*number*/1;
            painter->drawRect(0, 0, w, barheight);
            // translate and rotate back to the center
            painter->translate(0, -innerRadius);
            painter->rotate(-degree);
        }
    }

}

void Narr::drawCircularBarChart_bleb(QPainter *painter,
                                     std::vector<float> feature, qreal maxV,
                                     qreal innerRadius,
                                     qreal thickness,
                                     QColor color)
{
//    for(int n = 0; n < blebAvgSize.size(); n++)
//        std::cout << blebAvgSize[n];
//    std::cout << endl;
    if (feature.size() != 0){
//        QPen myPen(color);
//        myPen.setCapStyle(Qt::FlatCap);
//        painter->setPen(myPen);
//        painter->setBrush(QBrush(color));

        //int number = feature.size();
        int number = feature.size() < max ?  feature.size() : max;
        for(int n = 0; n < number; n++){
            qreal size = blebAvgSize[n] > BlebSizeMin ? blebAvgSize[n] - BlebSizeMin : 0;
            qreal g =  size > BlebSizeMax ? 1 : size/BlebSizeMax;
            //qreal g = qreal(blebAvgSize[n])/BlebSizeMax;
            QColor cc = gradualColor(color, 1-g);
            painter->setPen(cc);
            painter->setBrush(QBrush(cc));

            //qDebug() << feature[n];
            //rotate and translate from the center to location on the ring
            float degree = 360./(max-begin) * n;
            painter->rotate(degree);
            painter->translate(0, innerRadius);
            //draw a bar
            float barheight = float(feature[n]) * thickness/ maxV;
            //float w = /*2.*M_PI/(max-begin)*number*/1;
            float w = 2.*M_PI/(max-begin)*number;
            painter->drawRect(0, 0, w, barheight);
            // translate and rotate back to the center
            painter->translate(0, -innerRadius);
            painter->rotate(-degree);
        }
    }

}

void Narr::drawCircularLineChart_fixMax(QPainter *painter,
                                 std::vector<float> feature, qreal maxV,
                                 qreal innerRadius, qreal thickness,
                                 QColor color) //clockwise
{
    if (feature.size() != 0){
        QPen myPen(color);
        myPen.setCapStyle(Qt::FlatCap);
        painter->setPen(myPen);
        painter->setBrush(QBrush(color));

        int number  = feature.size();
        QPolygon    polyline;
        QPoint      start;
        for(int n = 0; n < number; n++)
        {
            //rotate and translate from the center to location on the ring
            float degree = (360./(max-begin) * n + 90) * M_PI/180;
            float barheight = float(feature[n]) * thickness/ maxV;
            float radius    = barheight + innerRadius;
            QPoint point    = QPoint(radius*cos(degree), radius*sin(degree));
            if(n == 0)
                start = point;
            polyline << point;
        }
        //connect to the beginning
        //polyline << begin;
        //draw polyline
        painter->drawPolyline(polyline);
    }
}

void Narr::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->y() <= halfH){
        float degree = atan((ev->x()-halfW)/(-ev->y()+halfH));
        if(degree < 0)
            degree = degree + 2*M_PI;
        angle = degree*180/M_PI;
    }else{
        float degree = atan((ev->x()-halfW)/(ev->y()-halfH));
        angle = 180-degree*180/M_PI;
    }
//    qDebug() << angle;
//    update();
    mouseIndex = int(angle/360.*(max-begin)+begin);

}

void Narr::setPropType(int propTp)
{
    propType = propTp;
    switch(propType){
    case 0:
        std::cout << "Property Type 'Area' selected. \n" << std::endl;
        break;
    case 1:
        std::cout << "Property Type: 'Perimeter' selected. \n" << std::endl;
        break;
    case 2:
        std::cout << "Property Type: 'Bleb' selected. \n" << std::endl;
        break;
    default:
        std::cout << "Property Type: 'Area' selected. \n" << std::endl;
        break;
    }
    update();
}

void Narr::setPropertyType(int propTp)
{
    propType = propTp;
    switch(propType){
    case 0:
        //std::cout << "Property Type 'Area' selected. \n" << std::endl;
        break;
    case 1:
        //std::cout << "Property Type: 'Perimeter' selected. \n" << std::endl;
        break;
    case 2:
        //std::cout << "Property Type: 'Bleb' selected. \n" << std::endl;
        break;
    default:
        //std::cout << "Property Type: 'Area' selected. \n" << std::endl;
        break;
    }
}

void Narr::setValue(float v)
{
    value = v;
}

void drawTriangle(QPainter *painter,
                  qreal p0x, qreal p0y,
                  qreal p1x, qreal p1y,
                  qreal p2x, qreal p2y,
                  QColor color){
    QPainterPath path;
    path.moveTo(p0x, p0y);
    path.lineTo(p1x, p1y);
    path.lineTo(p2x, p2y);
    path.lineTo(p0x, p0y);
    painter->fillPath(path, QBrush(color));
}

void drawXY(){
    QPainter painter_xy;
    //    QPen myPen(PURPLE);
    //    painter_xy.setPen(myPen);
    //    painter_xy.drawLine(QPoint(0, 0), QPoint(0, 50));
    //    myPen.setColor(YELLOW);
    //    painter_xy.setPen(myPen);
    //    painter_xy.drawLine(QPoint(0, 0), QPoint(50, 0));

    painter_xy.drawLine(QPoint(0, 0), QPoint(50, 0)); // x
    painter_xy.drawLine(QPoint(0, 0), QPoint(0, 50)); // y
}



void Narr::render(QPainter *painter)
{
////    if(!this->needUpdate)
////        return;

//    // draw Value
//    painter->eraseRect(0, 0, width(), height());
////    painter->eraseRect(width() - 70, 30, 60, 20);
//    painter->setPen(QColor(128, 0, 0));
//    painter->drawText(width() - 70, 30, 60, 20, Qt::AlignLeft, QString::number(value, 'f', 2));

//    halfW = this->width()/2; // half width
//    halfH = this->height()/2; // half height

//    // draw current frame
//    painter->setRenderHint(QPainter::Antialiasing);
//    QPen penContour(QColor(153, 204, 49));
//    painter->setPen(penContour);
//    painter->drawText(0, 0, 50, 30, Qt::AlignCenter, QString::number(int(mouseIndex)));
////    painter->drawText(0, 0, 50, 30, Qt::AlignCenter, QString::number(int(angle/360.*(max-begin)+begin)));

//    // set (0,0) to the center of the canvas
//    qreal   halfS = halfW > halfH ? halfH : halfW; // the smaller
//    QPointF center(halfW, halfH*1.05);
//    painter->translate(center.x(), center.y());

//    // set the start angle to 0 o'clock;
//    painter->rotate(-90); //***x->up, y->right***

//    // draw ring arcs background
//    qreal ringArcInnerRadius    = .30 * halfS;
//    qreal ringArcThickness      = .05 * halfS;
//    drawRingArc(painter, QPointF(0, 0), 0, 360, ringArcInnerRadius, ringArcThickness, QColor(240, 240, 240));

//    // draw real-time changing ring arcs
//    float p;
//        if(propType == 0) // area
//            p = float(area.size())/(max-begin);
//        else if (propType == 1)
//            p = float(perimeter.size())/(max-begin);
//        else if (propType == 2)
//            p = float(blebNum.size())/(max-begin);
//    qreal currAngle = p * 360 - 1.0;
//    drawRingArc(painter, QPointF(0, 0), 0, currAngle, ringArcInnerRadius, ringArcThickness, gradualColor(GREEN, /*p*/0.2));


////    float p = float(curr-begin)/(max-begin);
////    qreal currAngle = p * 360 - 1.0;
////    QColor c = gradualColor(GREEN, /*p*/0.2);
////    drawRingArc(painter, QPointF(0, 0), 0, currAngle, ringArcInnerRadius, ringArcThickness, c);
//    //    painter->setPen(QColor(200, 200, 200));
//    //    float xx = (ringArcInnerRadius - 60)*sin(currAngle*M_PI/180);
//    //    float yy = (ringArcInnerRadius - 60)*cos(currAngle*M_PI/180);
//    //    painter->rotate(90);
//    //    painter->drawText( xx-30*1.7, yy- 30, 60*1.7, 60 , Qt::AlignCenter, QString::number(int(curr)));
//    //    painter->rotate(-90);

//    painter->rotate(-90); //***x->left, y->up***
//    //draw circular bar
//    qreal propBarInnerRadius    = ringArcInnerRadius + ringArcThickness + .30 * halfS;
//    qreal propBarThickness      = .28* halfS;
//    //printAreaData();

////    drawRingArc(painter, QPointF(0,0), 0, 360, propBarInnerRadius, propBarThinkness+4, gradualColor(ORANGE, 0.95));
////    drawCircularBarChart(painter, area, areaBarInnerRadius, areaBarThinkness, 0.1, gradualColor(ORANGE, 0.7));
////    drawCircularLineChart(painter, area, propBarInnerRadius, propBarThinkness, 0.1, gradualColor(ORANGE, 0.3));

//    if(propType == 0){ // area
//        // draw real-time changing ring arcs

//        qreal maxArea = 1300;
//        qreal minArea = 200;
//        drawRingArc(painter, QPointF(0,0), 0, 360, propBarInnerRadius, propBarThickness+4, gradualColor(ORANGE, 0.95));
//        drawCircularBarChart_fixMax(painter, area, minArea, maxArea, propBarInnerRadius, propBarThickness, gradualColor(ORANGE, 0.3));
//        painter->rotate(180);
//        painter->setPen(ORANGE);
//        painter->drawLine(QPointF(0, -propBarInnerRadius), QPointF(0, -propBarInnerRadius-propBarThickness-2));
//        painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(maxArea))+" μm²");
//        painter->drawText(-40, -propBarInnerRadius, 80, 20, Qt::AlignCenter, QString::number(int(minArea))+" μm²");
//        painter->rotate(180);
//    }
//    else if(propType == 1){ // perimeter
//        qreal maxPerimeter = 300;
//        qreal minPerimeter = 50;
//        drawRingArc(painter, QPointF(0,0), 0, 360, propBarInnerRadius, propBarThickness+4, gradualColor(PURPLE, 0.95));
//        drawCircularBarChart_fixMax(painter, perimeter, minPerimeter, maxPerimeter, propBarInnerRadius, propBarThickness, gradualColor(PURPLE, 0.3));
//        painter->rotate(180);
//        painter->setPen(PURPLE);
//        painter->drawLine(QPointF(0, -propBarInnerRadius), QPointF(0, -propBarInnerRadius-propBarThickness-2));
//        painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(maxPerimeter))+" μm");
//        painter->drawText(-40, -propBarInnerRadius, 80, 20, Qt::AlignCenter, QString::number(int(minPerimeter))+" μm");
//        painter->rotate(180);
//    }
//    else if(propType == 2){ // bleb


//        drawRingArc(painter, QPointF(0,0), 0, 360, propBarInnerRadius, propBarThickness+4, gradualColor(BLUE, 0.95));
//        qreal maxBlebNum = 7;
//        drawCircularBarChart_bleb(painter, blebNum, maxBlebNum, propBarInnerRadius, propBarThickness, BLUE/*gradualColor(BLUE, 0.3)*/);
//        painter->rotate(180);
//        painter->setPen(BLUE);
//        painter->drawLine(QPointF(0, -propBarInnerRadius), QPointF(0, -propBarInnerRadius-propBarThickness-2));
//        painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(maxBlebNum))+" blebs");
//        painter->drawText(-40, -propBarInnerRadius, 80, 20, Qt::AlignCenter, "0 bleb");
//        painter->rotate(180);


//    }


//    painter->rotate(90); //***x->up, y->right***
//    //painter->drawLine(QPoint(0, 0), QPoint(50, 0)); // x
//    //painter->drawLine(QPoint(0, 0), QPoint(0, 50)); // y

//    //draw frame index indicator
//    painter->setPen(QPen(GREEN));
//    penContour.setWidth(1);
//    painter->rotate(angle);
//    painter->drawLine(QPoint(0,0), QPoint(propBarInnerRadius + propBarThickness+2, 0));
//    painter->setPen(QPen(QColor(255,255,255)));
//    painter->drawLine(QPoint(0,0), QPoint(ringArcInnerRadius, 0));
//    painter->drawLine(QPoint(ringArcInnerRadius + ringArcThickness,0), QPoint(propBarInnerRadius, 0));
//    painter->rotate(-angle);

////    // draw cell according to mouse Index
////    //qDebug() << mouseIndex << curr << begin;
////    if(begin > 0 && mouseIndex > 0 && curr > 0){
////            int idx = curr > mouseIndex ? (mouseIndex - begin) : (curr - begin);
////            if(idx > 0 && idx < cellImg.size()){
////                //qDebug() << "Idx " << idx;
////                painter->translate(0,0);
////                painter->rotate(90); //***x->up, y->right***
////                //int side = 2*ringArcInnerRadius/1.3;
////                float scale = 1.2;
////                QImage cell = cellImg[idx];
////                QRect centerRect = QRect(-cell.width()/2*scale, -cell.height()/2*scale, cell.width()*scale, cell.height()*scale);
////                painter->drawImage(centerRect, cell);
////                //painter->drawImage(-cellImg[idx-1].width()/2, -cellImg[idx-1].height()/2, cellImg[idx-1]);
////                painter->rotate(-90);
////            }
////    }

////    this->needUpdate = false;

}
































