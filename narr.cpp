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

const qreal area_minV = 100;
const qreal area_maxV = 600;

const qreal peri_minV = 10;
const qreal peri_maxV = 80;

const qreal blebN_minV = 0;
const qreal blebN_maxV = 7;

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

    bufferSize = width() * height() * 4;
    bufferr = new unsigned char[bufferSize]();
    for (unsigned int i = 0; i < bufferSize; i++)
        bufferr[i] = 255;

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
    maxIdx      = 1;
    maxFrm      = 1;
    minFrm      = 0;
    angle       = 0;
    mouseIndex  = 0;
    dataScale   = 1;

    //propType    = 1;
}

Narr::~Narr()
{

}

QColor gradualColor (QColor color, qreal percent){
    return QColor( color.red() + (255-color.red()) * percent * percent,
                   color.green() + (255-color.green()) * percent * percent,
                   color.blue() + (255-color.blue()) * percent * percent );
}

inline QPointF rotate(QPointF p, double theta/*, QPointF org*/){
    return QPointF(p.x()*cos(theta)-p.y()*sin(theta), p.x()*sin(theta)+p.y()*cos(theta))/* + org*/;
}

void Narr::drawPoint(QPointF p, QColor c){
    unsigned int px = (int(p.y()) * width() + int(p.x())) * 4;
    if(px > 0 && px+3 < bufferSize){
        bufferr[ px + 3] = c.alpha();
        bufferr[ px + 2] = c.red();
        bufferr[ px + 1] = c.green();
        bufferr[ px + 0] = c.blue();
    }
}

void Narr::drawRingArc(QPointF center, qreal innerRadius, qreal thickness, QColor c)
{
    qreal r = innerRadius+thickness;
    qreal r_sq = r*r;
    qreal a_sq = innerRadius*innerRadius;

    for(int y = -r; y <= r; y++){
        for(int x = -r; x <= r; x++){
            QPointF p = center + QPointF(float(x), float(y));
            if(x*x+y*y >= a_sq && x*x+y*y <= r_sq){
                drawPoint(p, c);
            }
        }
    }
}

void Narr::initialize()
{
    halfW = this->width()/2; // half width
    halfH = this->height()/2; // half height


    qreal   halfS = halfW > halfH ? halfH : halfW; // the smaller
    center.setX(halfW);
    center.setY(halfH*1.05);

    ringArcInnerRadius    = .30 * halfS;
    ringArcThickness      = .05 * halfS;
    drawRingArc(center, ringArcInnerRadius, ringArcThickness, QColor(240, 240, 240, 255));

    QColor c;
    if(propType == 0) {
        c = gradualColor(ORANGE, 0.95);
    }
    else if (propType == 1) {
        c = gradualColor(PURPLE, 0.95);
    }
    else if (propType == 2) {
        c = gradualColor(BLUE, 0.95);
    }
    propBarInnerRadius    = ringArcInnerRadius + ringArcThickness + .30 * halfS;
    propBarThickness      = .28* halfS;
    drawRingArc(center, propBarInnerRadius, propBarThickness, c);

}

void Narr::setBeginFrm(int beginFrame)
{
    begin = beginFrame;
    //std::cout << "NARRATIVE VIS BEGIN frame number " << begin << std::endl;
}

void Narr::setMaxFrm(unsigned int im, unsigned int fm)
{
    maxIdx = im;
    maxFrm = fm;
    //std::cout << "NARRATIVE VIS MAX frame number " << max << std::endl;

}

void Narr::printAreaData(){
    for(unsigned int n = 0; n < area.size(); n++){
        std::cout << area[n] << " ";
    }
    std::cout << std::endl;
}

void Narr::updateProperty(floatArray prop, int currFrame, int clustr)
{
    propSeq.push_back(prop);
    curr = currFrame;
    area.push_back(prop[1]); // area
    perimeter.push_back(prop[2]); // perimeter
    blebNum.push_back(prop[8]); // bleb number
    blebAvgSize.push_back(prop[9]); // bleb average size

    unsigned int maxRange = /*maxFrm < 5000 ? 5000 - minFrm :*/ maxFrm - minFrm; //HEREHERE
//    qDebug() << maxRange;
    unsigned int currRange = maxIdx - begin;

    int     num         = area.size();
//    float   degree      = 2.*M_PI/(maxIdx-begin)*num - M_PI_2;
//    float   div         = 2.*M_PI*propBarInnerRadius/(maxIdx-begin);
//    int     num         = area.size() > maxRange ? maxRange : area.size();

    if (num <= maxRange){
    float   degree      = 2.*M_PI/maxRange*num - M_PI_2;
    float   div         = 2.*M_PI*propBarInnerRadius/maxRange;
    int     barWidth    = div > 1 ? int(div) : 1;
    qreal   barheight;
    QColor  color;

    if(propType == 0) { // area
        barheight  = float(prop[1]*dataScale*dataScale - area_minV) * propBarThickness / (area_maxV - area_minV);
        color = gradualColor(ORANGE, 0.3);
    }
    else if (propType == 1) { // perimeter
        barheight  = float(prop[2]*dataScale - peri_minV) * propBarThickness / (peri_maxV - peri_minV);
        color = gradualColor(PURPLE, 0.3);
    }
    else if (propType == 2) { // bleb
        barheight  = float(prop[8]*dataScale - blebN_minV) * propBarThickness / (blebN_maxV - blebN_minV);
        qreal size = prop[9] > BlebSizeMin ? prop[9] - BlebSizeMin : 0;
        qreal g =  size > BlebSizeMax ? 1 : size/BlebSizeMax;
        color = gradualColor(BLUE, 1-g);
    }
    //qDebug() << barheight;

    // draw bars
    for (int h = propBarInnerRadius; h < propBarInnerRadius+barheight; h++){
        for (int v = 0; v < barWidth; v++){
            QPointF rotP = rotate(QPointF(h,v), degree)+center;
            drawPoint(rotP, color);
        }
    }

    // draw fragment
    for (int h = ringArcInnerRadius; h < ringArcInnerRadius+ringArcThickness; h++){
        for (int v = 0; v < ringArcThickness; v++){
            QPointF rotP = rotate(QPointF(h,v), degree)+center;

            drawPoint(rotP, gradualColor(GREEN, 0.2*(clustr*2)));
        }
    }
}

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


void drawRingArc1(QPainter   *painter,
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
            float degree = 360./(maxIdx-begin) * n;
            painter->rotate(degree);
            painter->translate(0, innerRadius);
            //draw a bar
            float barheight = float(feature[n] - (*min_it)) / (*max_it - *min_it) * thickness*(1-strtRto) + thickness *strtRto;
            float w = 2.*M_PI/(maxIdx-begin)*number;
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
            float degree = (360./(maxIdx-begin) * n + 90) * M_PI/180;
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
        int number = feature.size() < maxIdx ?  feature.size() : maxIdx;
        for(int n = 0; n < number; n++){
            //qDebug() << feature[n];
            //rotate and translate from the center to location on the ring
            float degree = 360./(maxIdx-begin) * n;
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
        int number = feature.size() < maxIdx ?  feature.size() : maxIdx;
        for(int n = 0; n < number; n++){
            qreal size = blebAvgSize[n] > BlebSizeMin ? blebAvgSize[n] - BlebSizeMin : 0;
            qreal g =  size > BlebSizeMax ? 1 : size/BlebSizeMax;
            //qreal g = qreal(blebAvgSize[n])/BlebSizeMax;
            QColor cc = gradualColor(color, 1-g);
            painter->setPen(cc);
            painter->setBrush(QBrush(cc));

            //qDebug() << feature[n];
            //rotate and translate from the center to location on the ring
            float degree = 360./(maxIdx-begin) * n;
            painter->rotate(degree);
            painter->translate(0, innerRadius);
            //draw a bar
            float barheight = float(feature[n]) * thickness/ maxV;
            //float w = /*2.*M_PI/(max-begin)*number*/1;
            float w = 2.*M_PI/(maxIdx-begin)*number;
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
            float degree = (360./(maxIdx-begin) * n + 90) * M_PI/180;
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

//void Narr::mouseMoveEvent(QMouseEvent *ev)
//{
//    if(ev->y() <= halfH){
//        float degree = atan((ev->x()-halfW)/(-ev->y()+halfH));
//        if(degree < 0)
//            degree = degree + 2*M_PI;
//        angle = degree*180/M_PI;
//    }else{
//        float degree = atan((ev->x()-halfW)/(ev->y()-halfH));
//        angle = 180-degree*180/M_PI;
//    }
//    //    qDebug() << angle;
//    //    update();
//    mouseIndex = int(angle/360.*(maxIdx-begin)+begin);

//}

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

void Narr::setDataScale(qreal scl)
{
    dataScale = scl;
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
    //    if(!this->needUpdate)
    //        return;

    // draw Value
    painter->eraseRect(0, 0, width(), height());
    //    painter->eraseRect(width() - 70, 30, 60, 20);
    painter->setRenderHint(QPainter::Antialiasing);


    img = QImage(bufferr, width(), height(), QImage::Format_ARGB32);
    painter->drawImage(0, 0, img);

    painter->setPen(QColor(128, 0, 0));
    painter->drawText(width() - 70, 30, 60, 20, Qt::AlignLeft, QString::number(value, 'f', 2));

    // draw current frame
    QPen penContour(QColor(153, 204, 49));
    painter->setPen(penContour);
    painter->drawText(0, 0, 50, 30, Qt::AlignCenter, QString::number(int(mouseIndex)));
    //    painter->drawText(0, 0, 50, 30, Qt::AlignCenter, QString::number(int(angle/360.*(max-begin)+begin)));

//    // set (0,0) to the center of the canvas
    QPointF center(halfW, halfH*1.05);
    painter->translate(center.x(), center.y());

        if(propType == 0){ // area
            painter->setPen(ORANGE);
            painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(area_maxV))+" μm²");
            painter->drawText(-40, -propBarInnerRadius, 80, 20, Qt::AlignCenter, QString::number(int(area_minV))+" μm²");
        }
        else if(propType == 1){ // perimeter
            painter->setPen(PURPLE);
            painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(peri_maxV))+" μm");
            painter->drawText(-40, -propBarInnerRadius, 80, 20, Qt::AlignCenter, QString::number(int(peri_minV))+" μm");
        }
        else if(propType == 2){ // bleb
            painter->setPen(BLUE);
            painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(blebN_maxV))+" blebs");
            painter->drawText(-40, -propBarInnerRadius, 80, 20, Qt::AlignCenter, QString::number(int(blebN_minV))+" blebs");
        }

    //    this->needUpdate = false;

}
































