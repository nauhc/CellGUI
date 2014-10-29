#include <QResizeEvent>
#include <QPainter>
#include <math.h>
#include "narr.h"

// 123.0/255.0, 57.0/255.0, 144.0/255.0     -> purple
// 28.0/255.0, 120.0/255.0, 159.0/255.0     -> blue
// 97.0/255.0, 149.0/255.0, 5.0/255.0       -> green
// 247.0/255.0, 154.0/255.0, 1.0/255.0      -> yellow
// 208.0/255.0, 81.0/255.0, 38.0/255.0      -> red
// 52.0/255.0, 56.0/255.0, 57.0/255.0       -> dark gray bg
// 193.0/255.0, 194.0/255.0, 196.0/255.0    -> light gray fg

QColor GREEN = QColor(97, 149, 5);

Narr::Narr()
{
}

void Narr::initializeGL()
{

}

void drawRingArc(QPainter   *painter,
                 QPointF    &center,
                 qreal      startAngle,
                 qreal      endAngle,
                 qreal      innerRadius,
                 qreal      thickness,
                 QColor     &color){

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

void Narr::render(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QPointF center(this->size().width()/2,
                   this->size().height()/2);

//    QLinearGradient myGradient;
//    QPainterPath myPath;
//    myPath.moveTo(center.x(), center.y());
//    myPath.arcTo(center.x() - 100, center.x() - 100,
//                 200, 200, 0.0, 30.0);

//    painter->setBrush(myGradient);
//    painter->setPen(myPen);
//    painter->drawPath(myPath);

    drawRingArc(painter, center, 0.0, 360.0, 80.0, 10.0, GREEN);



}

void Narr::resizeGL(int w, int h)
{
}

