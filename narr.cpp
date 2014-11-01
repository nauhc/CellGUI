#include <QResizeEvent>
#include <QPainter>
#include <math.h>
#include <QDebug>
#include <vector>
#include "narr.h"

// 123.0/255.0, 57.0/255.0, 144.0/255.0     -> purple
// 28.0/255.0, 120.0/255.0, 159.0/255.0     -> blue
// 97.0/255.0, 149.0/255.0, 5.0/255.0       -> green
// 247.0/255.0, 154.0/255.0, 1.0/255.0      -> yellow
// 208.0/255.0, 81.0/255.0, 38.0/255.0      -> red
// 52.0/255.0, 56.0/255.0, 57.0/255.0       -> dark gray bg
// 193.0/255.0, 194.0/255.0, 196.0/255.0    -> light gray fg

QColor GREEN = QColor(97, 149, 5);
QColor ORANGE = QColor(247, 154, 1);
QColor BLUE = QColor(28, 120, 159);

Narr::Narr()
{
    area.clear();
    stage.clear();
    stage.push_back(0);

    //should be getting from controller
    max = 3500;
    stage.push_back(218);
    stage.push_back(1001);
    stage.push_back(1377);
    stage.push_back(2200);
    stage.push_back(2883);
    stage.push_back(max);

    QFile f("/Users/chuanwang/Sourcecode/CellGUI/video/movie_10172013_bv2_control01_original_sampled.csv");
    if(!f.open(QIODevice::ReadOnly)){
       qDebug() << "Reading csv file not found.";
    }else{
        QTextStream in(&f);
        int cnt = 0;
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
            area.push_back(row[1]);
            blebNum.push_back(row[6]);

        }
    }
    f.close();


}

void Narr::buildFeature(float f)
{
    area.push_back(f);
}

void Narr::buildStage(unsigned int index)
{
    stage.push_back(index);
}

void Narr::getMax(unsigned int m)
{
    max = m;
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
                 QColor     color){

    qreal outerRadius = innerRadius + thickness;

 /*
    QPainterPath myPath;
    myPath.moveTo(center.x() + innerRadius * cos(startAngle),
                  center.y() + innerRadius * sin(startAngle));
    myPath.lineTo(center.x() + outerRadius * cos(startAngle),
                  center.y() + outerRadius * sin(startAngle));
    myPath.arcTo(center.x() - outerRadius,
                 center.y() - outerRadius,
                 2 * outerRadius, 2 * outerRadius,
                 startAngle, endAngle);
    myPath.moveTo(center.x() + outerRadius * cos(endAngle),
                  center.y() + outerRadius * sin(endAngle));
    myPath.moveTo(center.x() + innerRadius * cos(endAngle),
                  center.y() + innerRadius * sin(endAngle));

    myPath.arcTo(center.x() - innerRadius,
                 center.y() - innerRadius,
                 2 * innerRadius, 2 * innerRadius,
                 endAngle, startAngle);
    myPath.closeSubpath();

    QLinearGradient myGradient;
    QPen myPen(color);
    myPen.setWidth(2);

    painter->setBrush(myGradient);
    painter->setPen(myPen);
    painter->drawPath(myPath); */

        QPen myPen(color);
        myPen.setCapStyle(Qt::FlatCap);
        myPen.setWidth(thickness);
        painter->setPen(myPen);
        painter->drawArc(center.x() - outerRadius,
                         center.y() - outerRadius,
                         2 * outerRadius, 2 * outerRadius,
                         startAngle*16, endAngle*16);

}

void drawBar(QPainter *painter, std::vector<float> feature,
             qreal innerRadius, qreal thickness,
             qreal strtRto, QColor color)
{
    QPen myPen(color);
    myPen.setCapStyle(Qt::FlatCap);
    painter->setPen(myPen);
    painter->setBrush(QBrush(color));
    std::vector<float>::const_iterator max, min;
    max = std::max_element(std::begin(feature), std::end(feature));
    min = std::min_element(std::begin(feature), std::end(feature));

    int number = feature.size();
    for(int n = 0; n < number; n++){
        //rotate and translate from the center to location on the ring
        painter->rotate(360./number/2*n);
        painter->translate(0, innerRadius);
        //draw a bar
        float barheight = float(feature[n] - (*min)) / (*max - *min) * thickness*(1-strtRto) + thickness *strtRto;
        painter->drawRoundedRect(0, 0, 2.*M_PI/number - 2, barheight, 5.0, 3.0);
        // translate and rotate back to the center
        painter->translate(0, -innerRadius);
        painter->rotate(-360./number*n);
    }

}

void Narr::render(QPainter *painter)
{
    // move painter to the center of the canvas
    QPointF center(this->size().width()/2,
                   this->size().height()/2);
    painter->translate(center.x(), center.y());
    painter->rotate(-90);

    painter->setRenderHint(QPainter::Antialiasing);

    qreal ringArcInnerRadius    = 70.0;
    qreal ringArcThickness      = 20.0;
    for(int n = 0; n < stage.size() - 1; n++)
    {
        float p = 1.0 - float(n+1) / float(stage.size());
        drawRingArc(painter, QPointF(0,0),
                    float(stage[n]) / float(max) * 360.0 + 1.0,
                    float(stage[n+1] - stage[n]) / float(max) * 360.0 - 1.0,
                    ringArcInnerRadius, ringArcThickness, gradualColor(GREEN, p));
    }

    qreal areaBarInnerRadius    = ringArcInnerRadius + ringArcThickness + 150.0;
    qreal areaBarThinkness      = 100.0;
    drawBar(painter, area, areaBarInnerRadius, areaBarThinkness, 0.2, ORANGE);

//    qreal periBarInnerRadius    = areaBarInnerRadius + areaBarThinkness + 30.0;
//    qreal periBarThinkness      = 50.0;
//    drawBar(painter, blebNum, periBarInnerRadius, periBarThinkness, 0.1, BLUE);

}

void Narr::resizeGL(int w, int h)
{

}
































