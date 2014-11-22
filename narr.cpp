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

inline int larger(int a, int b){
    return (a > b ? a : b);
}

Narr::Narr(QWidget *parent)
{
    area.clear();
    stage.clear();
    cells.clear();
    stage.push_back(0);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

    //should be getting from controller -- star --
    curr = 0;
    max  = 1;
//    max = 3500;
//    stage.push_back(218);
//    stage.push_back(1001);
//    stage.push_back(1377);
//    stage.push_back(2200);
//    stage.push_back(2883);
//    stage.push_back(max);

    QFile f("/Users/chuanwang/Sourcecode/CellGUI/video/movie_10172013_bv2_control01_original_sampled copy.csv");
    if(!f.open(QIODevice::ReadOnly)){
       qDebug() << "Reading csv file not found.";
    }else{
        QTextStream in(&f);
        while(!in.atEnd()) { // each row
            QString line = in.readLine();
            if(line.isEmpty()){
                continue;
            }
            if(line.isNull()){
                break;
            }
            QVector<float> row;
            foreach (const QString &cell, line.split(",")) {
                //row.append(cell.trimmed());
                row.append(cell.trimmed().toFloat());
            }
            //qDebug() << row;
            //area.push_back(row[2]);
            blebNum.push_back(row[6]);

        }
    }
    f.close();

//    QImage cell0("/Users/chuanwang/Sourcecode/CellGUI/video/cell91.png");
//    QImage cell1("/Users/chuanwang/Sourcecode/CellGUI/video/cell212.png");
//    QImage cell2("/Users/chuanwang/Sourcecode/CellGUI/video/cell318.png");
//    QImage cell3("/Users/chuanwang/Sourcecode/CellGUI/video/cell630.png");
//    QImage cell4("/Users/chuanwang/Sourcecode/CellGUI/video/cell1523.png");
//    qreal cellScale = 0.6;
//    cells.push_back(cell0.convertToFormat(QImage::Format_ARGB32).scaled(cellScale*cell0.width(),cellScale*cell0.height(),Qt::KeepAspectRatio));
//    cells.push_back(cell1.convertToFormat(QImage::Format_ARGB32).scaled(cellScale*cell1.width(),cellScale*cell1.height(),Qt::KeepAspectRatio));
//    cells.push_back(cell2.convertToFormat(QImage::Format_ARGB32).scaled(cellScale*cell2.width(),cellScale*cell2.height(),Qt::KeepAspectRatio));
//    cells.push_back(cell3.convertToFormat(QImage::Format_ARGB32).scaled(cellScale*cell3.width(),cellScale*cell3.height(),Qt::KeepAspectRatio));
//    cells.push_back(cell4.convertToFormat(QImage::Format_ARGB32).scaled(cellScale*cell4.width(),cellScale*cell4.height(),Qt::KeepAspectRatio));
    //should be getting from controller -- end --
}

Narr::~Narr()
{

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

    area.push_back(prop[0]);
    //printAreaData();
}

void Narr::updateStage(unsigned int index)
{
    stage.push_back(index);
}

void Narr::getMax(unsigned int m)
{
    max = m;
    //qDebug() << "NARRATIVE VIS MAX frame number " << max;
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
            float degree = 360./max * n;
            painter->rotate(degree);
            painter->translate(0, innerRadius);
            //draw a bar
            float barheight = float(feature[n] - (*min_it)) / (*max_it - *min_it) * thickness*(1-strtRto) + thickness *strtRto;
            float w = 2.*M_PI/max*number;
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
        QPoint      begin;
        for(int n = 0; n < number; n++)
        {
            //rotate and translate from the center to location on the ring
            float degree = (360./max* n + 90) * M_PI/180;
            float barheight = float(feature[n] - (*min_it)) / (*max_it - *min_it) * thickness*(1-strtRto) + thickness *strtRto;
            float radius    = barheight + innerRadius;
            QPoint point    = QPoint(radius*cos(degree), radius*sin(degree));
            if(n == 0)
                begin = point;
            polyline << point;
        }
        //connect to the beginning
        //polyline << begin;
        //draw polyline
        painter->drawPolyline(polyline);
    }
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
    // set (0,0) to the center of the canvas
    qreal   halfW = this->width()/2; // half width
    qreal   halfH = this->height()/2; // half height
    qreal   halfS = halfW > halfH ? halfH : halfW; // the smaller
    QPointF center(halfW, halfH /*+ 50*/);
    painter->translate(center.x(), center.y());
    // set the start angle to 0 o'clock;
    painter->rotate(-90); //***x->up, y->right***

    painter->setRenderHint(QPainter::Antialiasing);

    // draw ring arcs background
    qreal ringArcInnerRadius    = .20 * halfS;
    qreal ringArcThickness      = .05 * halfS;
    drawRingArc(painter, QPointF(0, 0), 0, 360, ringArcInnerRadius, ringArcThickness, QColor(240, 240, 240));
    // draw real-time changing ring arcs
    float p = float(curr)/max;
    qreal currAngle = p * 360 - 1.0;
    QColor c = gradualColor(GREEN, p);
    drawRingArc(painter, QPointF(0, 0), 0, currAngle, ringArcInnerRadius, ringArcThickness, c);
    /*
    qreal p0y = qreal(ringArcInnerRadius+.5*ringArcThickness) * tan(currAngle*M_PI/180.0);
    qreal p0x = qreal(ringArcInnerRadius+.5*ringArcThickness) / cos(currAngle*M_PI/180.0);
    //painter->drawEllipse(QPoint(p0x, p0y), 1, 1);
    qreal p1y = qreal(ringArcInnerRadius+ringArcThickness+5) * tan(larger(currAngle-10, 0)*M_PI/180.0);
    qreal p1x = qreal(ringArcInnerRadius+ringArcThickness+5) / cos(larger(currAngle-10, 0)*M_PI/180.0);
    qreal p2y = qreal(ringArcInnerRadius+ringArcThickness-5) * tan(larger(currAngle-15, 0)*M_PI/180.0);
    qreal p2x = qreal(ringArcInnerRadius+ringArcThickness-5) / cos(larger(currAngle-15, 0)*M_PI/180.0);
    drawTriangle(painter, p0x, p0y, p1x, p1y, p2x, p2y, gradualColor(GREEN, p)); */



//    for(int n = 0; n < stage.size() - 1; n++)
//    {
//        //float p = rand() % 100 / 100.;
//        float p = /*1.0 - */float(n+1) / float(stage.size());
//        drawRingArc(painter, QPointF(0,0),
//                     float(stage[n]) / float(max) * 360.0 + 1.0, // start angle
//                    float(stage[n+1] - stage[n]) / float(max) * 360.0 - 1.0, // end angle
//                    ringArcInnerRadius, ringArcThickness, gradualColor(GREEN, p));
//        //draw triangle on the end of the last ring arc
//        if ( n+1 == stage.size()-1 ){
//            painter->rotate(90); //***x->right, y->down***

//            qreal th1 = 20., th2 = 5.;
//            qreal p0x = 0, p0y = - (ringArcThickness + ringArcInnerRadius);
//            qreal p1x = 0 - th1, p1y = -(1.5* ringArcThickness + ringArcInnerRadius + th2);
//            qreal p2x = 0 - th1*.7, p2y = -(.3* ringArcThickness + ringArcInnerRadius - th2);
//            drawTriangle(painter, p0x, p0y, p1x, p1y, p2x, p2y, gradualColor(GREEN, p)); //gradualColor(GREEN, p)
//            qreal p3x = p0x, p3y = p1y;
//            drawTriangle(painter, p0x, p0y, p1x, p1y, p3x, p3y, WHITE);
//            qreal p4x = p0x, p4y = p2y;
//            drawTriangle(painter, p0x, p0y, p2x, p2y, p4x, p4y, WHITE);
//        }
//    }

    painter->rotate(-90); //***x->left, y->up***

    //draw circular bar
    qreal areaBarInnerRadius    = ringArcInnerRadius + ringArcThickness + .40 * halfS;
    qreal areaBarThinkness      = .30* halfS;
    //printAreaData();
//    painter->setPen(QColor(250, 250, 250));
//    painter->drawArc(0, 0, -areaBarInnerRadius, areaBarInnerRadius, 0, 360);
//    painter->drawArc(0, 0, -areaBarThinkness, areaBarThinkness, 0, 360);

//    painter->translate(0, areaBarInnerRadius);

//    painter->translate(0, -areaBarInnerRadius);
    drawRingArc(painter, QPointF(0,0), 0, 360, areaBarInnerRadius, areaBarThinkness, gradualColor(ORANGE, 0.95));

    drawCircularBarChart(painter, area, areaBarInnerRadius, areaBarThinkness, 0.1, gradualColor(ORANGE, 0.7));
    drawCircularLineChart(painter, area, areaBarInnerRadius, areaBarThinkness, 0.1, gradualColor(ORANGE, 0.3));


//    painter->rotate(90); //***x->up, y->right***
//    painter->drawLine(QPoint(0, 0), QPoint(50, 0)); // x
//    painter->drawLine(QPoint(0, 0), QPoint(0, 50)); // y

//    // draw cells
//    qreal cellRadius = ringArcInnerRadius + ringArcThickness + 70;
//    for(int n = 0; n < cells.size(); n++)
//    {
//        float degree = n * 360./cells.size();
//        int x_center = cellRadius * cos(degree*M_PI/180);
//        int y_center = cellRadius * sin(degree*M_PI/180);
//        painter->translate(x_center, y_center);
//        painter->rotate(90);
//        painter->drawImage(-cells[n].width()/2, -cells[n].height()/2, cells[n]);
//        painter->rotate(-90);
//        painter->translate(-x_center, -y_center);
//    }

}

//void Narr::resizeGL(int w, int h)
//{

//}
































