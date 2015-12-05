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
QColor BLUE     = /*QColor(102, 177, 50)*/QColor(28, 120, 159);
QColor PURPLE   = QColor(123, 57, 144);
QColor WHITE    = QColor(255, 255, 255);
QColor YELLOW   = QColor(247, 154, 1);
QColor RED      = QColor(208, 81, 38);

float blebColorMap[7][3] = {{255,255,204},
                            {199,233,180},
                            {127,205,187},
                            {65,182,196},
                            {29,145,192},
                            {34,94,168},
                            {12,44,132}};

float BlebSizeMax = 7.0; /*10.0*//*1.5*//*12*/
float BlebSizeMin = 0.5;

const qreal area_minV = 100;
//const qreal area_maxV = 600;
qreal area_maxV = 500;

const qreal peri_minV = 30;
qreal peri_maxV = 120;

const qreal blebN_minV = 0;
qreal blebN_maxV = 15;

const qreal blebS_minV = 0;
qreal blebS_maxV = 200;


inline int larger(int a, int b){
    return (a > b ? a : b);
}


Narr::Narr(QWidget *parent, bool multv, int sideLen)
{
    clear();
    if(!multv){
        QTimer* timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(1);
    }

    resize(sideLen, sideLen);

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
    //    cluster.clear();

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

    //    ringArcInnerRadius    = .30 * halfS;
    ringArcInnerRadius    = .15 * halfS;
    ringArcThickness      = .01 * halfS;
    //drawRingArc(center, ringArcInnerRadius, ringArcThickness, QColor(240, 240, 240, 255));

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
    //    propBarInnerRadius    = ringArcInnerRadius + ringArcThickness + .30 * halfS;
    //    propBarThickness      = .28* halfS;
    propBarInnerRadius    = ringArcInnerRadius + ringArcThickness + .10 * halfS;
    propBarThickness      = .65* halfS;
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

void Narr::updateProperty_single(floatArray prop, int currFrame, int clustr)
{
    propSeq.push_back(prop);
    curr = currFrame;
    area.push_back(prop[1]); // area
    perimeter.push_back(prop[2]); // perimeter
    blebNum.push_back(prop[8]); // bleb number
    blebAvgSize.push_back(prop[9]); // bleb average size
    //    cluster.push_back(clustr);

    unsigned int maxRange = /*maxFrm < 5000 ? 5000 - minFrm :*/ maxFrm - minFrm; //HEREHERE
    //    qDebug() << maxRange;
    unsigned int currRange = maxIdx - begin;
    //    float   degree      = 2.*M_PI/maxRange*num - M_PI_2;
    int     num         = area.size();
    //    float   degree      = 2.*M_PI/(maxIdx-begin)*num - M_PI_2;
    //    float   div         = 2.*M_PI*propBarInnerRadius/(maxIdx-begin);
    //    int     num         = area.size() > maxRange ? maxRange : area.size();

    if (num <= maxRange){

        float   div         = 2.*M_PI*propBarInnerRadius/maxRange;
        int     barWidth    = div > 1 ? int(div) : 1;
        //    qreal   barheight;
        //    QVector<qreal>   barheight;
        //    QColor  color;

        if(propType == 0) { // area
            //        barheight  = float(prop[1]*dataScale*dataScale - area_minV) * propBarThickness / (area_maxV - area_minV);
            QColor color = gradualColor(ORANGE, 0.3);
            for(unsigned int i = 0; i < area.size(); i++){
                float   degree      = 2.*M_PI/maxRange*i - M_PI_2;
                qreal barheight = float(area[i]*dataScale*dataScale - area_minV) * propBarThickness / (area_maxV - area_minV);
                for (int h = propBarInnerRadius; h < propBarInnerRadius+barheight; h++){
                    for (int v = 0; v < barWidth; v++){
                        QPointF rotP = rotate(QPointF(h,v), degree)+center;
                        drawPoint(rotP, color);
                    }
                }
            }
        }
        else if (propType == 1) { // perimeter
            //        barheight  = float(prop[2]*dataScale - peri_minV) * propBarThickness / (peri_maxV - peri_minV);
            QColor color = gradualColor(PURPLE, 0.3);
            for(unsigned int i = 0; i < perimeter.size(); i++){
                float   degree      = 2.*M_PI/maxRange*i - M_PI_2;
                qreal barheight = float(perimeter[i]*dataScale - peri_minV) * propBarThickness / (peri_maxV - peri_minV);
                for (int h = propBarInnerRadius; h < propBarInnerRadius+barheight; h++){
                    for (int v = 0; v < barWidth; v++){
                        QPointF rotP = rotate(QPointF(h,v), degree)+center;
                        drawPoint(rotP, color);
                    }
                }
            }
        }
        else if (propType == 2) { // bleb
            //        barheight  = float(prop[8]*dataScale - blebN_minV) * propBarThickness / (blebN_maxV - blebN_minV);
            for(unsigned int i = 0; i < blebNum.size(); i++){
                qreal barheight = float(blebNum[i]*dataScale - blebN_minV) * propBarThickness / (blebN_maxV - blebN_minV);
                qreal size = blebAvgSize[i] > BlebSizeMin ? blebAvgSize[i] - BlebSizeMin : 0;
                qreal g =  size > BlebSizeMax ? 1 : size/BlebSizeMax;
                QColor color = gradualColor(BLUE, 1-g);
                float    degree = 2.*M_PI/maxRange*i - M_PI_2;
                for (int h = propBarInnerRadius; h < propBarInnerRadius+barheight; h++){
                    for (int v = 0; v < barWidth; v++){
                        QPointF rotP = rotate(QPointF(h,v), degree)+center;
                        drawPoint(rotP, color);
                    }
                }
            }

        }
        //qDebug() << barheight;

        //    // draw bars
        //    for (int h = propBarInnerRadius; h < propBarInnerRadius+barheight; h++){
        //        for (int v = 0; v < barWidth; v++){
        //            QPointF rotP = rotate(QPointF(h,v), degree)+center;
        //            drawPoint(rotP, color);
        //        }
        //    }

        //    // draw fragment
        //    for(unsigned int i = 0; i < area.size(); i++){
        //        float degree = 2.*M_PI/maxRange*i - M_PI_2;
        //        for (int h = ringArcInnerRadius; h < ringArcInnerRadius+ringArcThickness; h++){
        //            for (int v = 0; v < /*ringArcThickness*/barWidth; v++){
        //                QPointF rotP = rotate(QPointF(h,v), degree)+center;
        //                drawPoint(rotP, gradualColor(GREEN, 0.2*(clustr*2)));
        //            }
        //        }
        //    }
    }

}

void Narr::updateProperty_multi(floatArray prop, int currFrame/*, int clustr*/)
{
    propSeq.push_back(prop);
    curr = currFrame;
    area.push_back(prop[1]); // area
    perimeter.push_back(prop[2]); // perimeter
    //    blebNum.push_back(prop[8]); // bleb number
    //    blebAvgSize.push_back(prop[9]); // bleb average size
    int bN = prop[5];
    blebNum.push_back(bN); // bleb number

    float bS = 0;
    floatArray blebsSize;
    for (int i=6; i<6+bN; i++){
        bS += prop[i];
        blebsSize.push_back(prop[i]);
    }
    bS = float(bS)/float(bN);
    blebAvgSize.push_back(bS); // bleb average size

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
            if (prop[1] > area_maxV) barheight = propBarThickness;
            else
                barheight  = float(prop[1]*dataScale*dataScale - area_minV) * propBarThickness / (area_maxV - area_minV);
            color = gradualColor(ORANGE, 0.3);
            // draw bars
            for (int h = propBarInnerRadius; h < propBarInnerRadius+barheight; h++){
                for (int v = 0; v < barWidth; v++){
                    QPointF rotP = rotate(QPointF(h,v), degree)+center;
                    drawPoint(rotP, color);
                }
            }
        }
        else if (propType == 1) { // perimeter
            if (prop[2] > peri_maxV) barheight = propBarThickness;
            else
                barheight  = float(prop[2]*dataScale - peri_minV) * propBarThickness / (peri_maxV - peri_minV);
            color = gradualColor(PURPLE, 0.3);
            // draw bars
            for (int h = propBarInnerRadius; h < propBarInnerRadius+barheight; h++){
                for (int v = 0; v < barWidth; v++){
                    QPointF rotP = rotate(QPointF(h,v), degree)+center;
                    drawPoint(rotP, color);
                }
            }
        }
        else if (propType == 2) { // bleb
            if (bN > blebN_maxV )
                barheight = propBarThickness;
            else
                barheight  = float(bN*dataScale - blebN_minV) * propBarThickness / (blebN_maxV - blebN_minV);

            int blebHeight = barheight/float(blebsSize.size());
            //qDebug() << "blebHeight" << blebHeight;

//            qreal size = bS > BlebSizeMin ? bS - BlebSizeMin : 0;
//            size = bS < BlebSizeMax ? bS - BlebSizeMin: BlebSizeMax - BlebSizeMin;
//            qreal g =  size > BlebSizeMax ? 1 : size/BlebSizeMax;
//            color = gradualColor(BLUE, 1-g);

            // draw bars
            //for (int i = blebsSize.size(); i > 0; i--){
            for (int i = 0; i < blebsSize.size();i++){
                qreal size = blebsSize[i] > BlebSizeMin ? blebsSize[i]: BlebSizeMin;
                size = blebsSize[i] < BlebSizeMax ? blebsSize[i]: BlebSizeMax;
//                int colorlevel = int(size/BlebSizeMax * 7) >= 7 ? 6 : int(size/BlebSizeMax * 7);
                int colorlevel = 0;
                if(size < BlebSizeMin || size < 0.1* (BlebSizeMax-BlebSizeMin))
                    colorlevel = 0;
                else if (size < 0.2 * (BlebSizeMax-BlebSizeMin))
                    colorlevel = 1;
                else if (size < 0.4 * (BlebSizeMax-BlebSizeMin))
                    colorlevel = 2;
                else if (size < 0.6 * (BlebSizeMax-BlebSizeMin))
                    colorlevel = 3;
                else if (size < 0.8 * (BlebSizeMax-BlebSizeMin))
                    colorlevel = 4;
                else if (size < 0.9 * (BlebSizeMax-BlebSizeMin))
                    colorlevel = 5;
                else{
                    colorlevel = 6;
                    //qDebug() << i << size << BlebSizeMax-BlebSizeMin << colorlevel;
                }
                color = QColor(blebColorMap[colorlevel][0], blebColorMap[colorlevel][1], blebColorMap[colorlevel][2]);

                //qDebug() << i << size << propBarInnerRadius+blebHeight*i << propBarInnerRadius+blebHeight*(i+1) << colorlevel << prop[1];

                for (int h = propBarInnerRadius+blebHeight*i; h < propBarInnerRadius+blebHeight*(i+1); h++){
                    for (int v = 0; v < barWidth; v++){
                        QPointF rotP = rotate(QPointF(h,v), degree)+center;
                        drawPoint(rotP, color);
                    }
                }
            }

//            // draw bars (single color - average bleb size - on one bar )
//            //for (int i = blebsSize.size(); i > 0; i--){
//            for (int i = 0; i < blebsSize.size();i++){
//                qreal size = blebsSize[i] > BlebSizeMin ? blebsSize[i] - BlebSizeMin : 0;
//                size = blebsSize[i] < BlebSizeMax ? blebsSize[i] - BlebSizeMin: BlebSizeMax - BlebSizeMin;
//                int colorlevel = int(size/BlebSizeMax * 7) >= 7 ? 6 : int(size/BlebSizeMax * 7);
//                color = QColor(blebColorMap[colorlevel][0], blebColorMap[colorlevel][1], blebColorMap[colorlevel][2]);

//                //qDebug() << i << propBarInnerRadius+blebHeight*i << propBarInnerRadius+blebHeight*(i+1) << colorlevel;

//                for (int h = propBarInnerRadius+blebHeight*i; h < propBarInnerRadius+blebHeight*(i+1); h++){
//                    for (int v = 0; v < barWidth; v++){
//                        QPointF rotP = rotate(QPointF(h,v), degree)+center;
//                        drawPoint(rotP, color);
//                    }
//                }
//            }

        }
        //qDebug() << barheight;



//        // draw fragment
//        for (int h = ringArcInnerRadius; h < ringArcInnerRadius+ringArcThickness; h++){
//            for (int v = 0; v < ringArcThickness; v++){
//                QPointF rotP = rotate(QPointF(h,v), degree)+center;
//                drawPoint(rotP, gradualColor(GREEN, 0.2*(clustr*2)));
//            }
//        }
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

void Narr::setPropType(int propTp)
{
    propType = propTp;
    switch(propType){
    case 0:
        std::cout << "Property Type 'Area' selected." << std::endl;
        break;
    case 1:
        std::cout << "Property Type: 'Perimeter' selected." << std::endl;
        break;
    case 2:
        std::cout << "Property Type: 'Bleb' selected." << std::endl;
        break;
    default:
        std::cout << "Property Type: 'Area' selected." << std::endl;
        break;
    }
    initialize();
    //    update();
    //    qDebug() << propType;
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

void Narr::setMaxValue(float maxArea, float maxPeri, float maxBlebN, float maxBlebS)
{
    area_maxV = 580/*maxArea*/;
    peri_maxV = 300/*maxPeri*/;
    //    blebN_maxV = maxBlebN;
    //    blebS_maxV = maxBlebS;
}

void Narr::setValue(QVector<float> v)
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
    //    painter->drawText(width() - 70, 30, 60, 20, Qt::AlignLeft, QString::number(value, 'f', 2));
    for (unsigned int k = 0; k < value.size(); k++)
        painter->drawText(width() - 60, 20+k*15, 60, 20, Qt::AlignLeft, QString::number(value[k], 'f', 2));


    //    // draw current frame
    //    QPen penContour(QColor(153, 204, 49));
    //    painter->setPen(penContour);
    //    painter->drawText(0, 0, 50, 30, Qt::AlignCenter, QString::number(int(mouseIndex)));
    //    //    painter->drawText(0, 0, 50, 30, Qt::AlignCenter, QString::number(int(angle/360.*(max-begin)+begin)));

    //    // set (0,0) to the center of the canvas
    QPointF center(halfW, halfH*1.05);
    painter->translate(center.x(), center.y());

    if(propType == 0){ // area
        painter->setPen(ORANGE);
        painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(area_maxV))+" μm²");
        painter->drawText(-40, -propBarInnerRadius+10, 80, 30, Qt::AlignCenter, QString::number(int(area_minV))+" μm²");
    }
    else if(propType == 1){ // perimeter
        painter->setPen(PURPLE);
        painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(peri_maxV))+" μm");
        painter->drawText(-40, -propBarInnerRadius+10, 80, 30, Qt::AlignCenter, QString::number(int(peri_minV))+" μm");
    }
    else if(propType == 2){ // bleb
        painter->setPen(BLUE);
        painter->drawText(-40, -propBarInnerRadius-propBarThickness-20, 80, 20, Qt::AlignCenter, QString::number(int(blebN_maxV))+" blebs");
        painter->drawText(-40, -propBarInnerRadius+10, 80, 30, Qt::AlignCenter, QString::number(int(blebN_minV))+" blebs");
    }

    //    this->needUpdate = false;

}

void Narr::resizeEvent(QResizeEvent *e)
{

}



























