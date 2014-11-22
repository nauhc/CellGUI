#include <QResizeEvent>
#include <QPainter>
#include <math.h>
#include <QDebug>
#include <vector>
#include <limits>
#include <QTimer>
#include "coord.h"

QColor _BLUE_   = QColor(28, 120, 159);

Coord::Coord(QWidget *parent)
{
//    QFile f("/Users/chuanwang/Sourcecode/CellGUI/video/movie_10172013_bv2_control01_original_sampled copy.csv");
//    if(!f.open(QIODevice::ReadOnly)){
//       qDebug() << "Reading csv file not found.";
//    }else{
//        QTextStream in(&f);
//        while(!in.atEnd()) { // each row
//            QString line = in.readLine();
//            if(line.isEmpty()){
//                continue;
//            }
//            if(line.isNull()){
//                break;
//            }
//            QVector<float> row;
//            foreach (const QString &cell, line.split(",")) {
//                //row.append(cell.trimmed());
//                row.append(cell.trimmed().toFloat());
//            }
//            QPointF p(row[3], row[4]);
//            //qDebug() << p;
//            centroid.push_back(p);
//        }
//    }
//    f.close();
    centroid.clear();
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

    currFrm = 0;
    maxFrm  = 1;
//    min = QPoint(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//    max = QPoint(0, 0);
    min = QPoint(0, 0);
    max = QPoint(50, 50);

}

Coord::~Coord()
{

}

void Coord::updateCoord(QPointF point, int currFrame)
{
    currCent = QPoint(point.x(), point.y());
    currFrm = currFrame;
    centroid.append(point);
    //qDebug() << "Coord Class" << currFrm << centroid;

//    qreal x_min, y_min, x_max, y_max;
//    if(point.x() < min.x())
//        x_min = point.x();
//    if(point.y() < min.y())
//        y_min = point.y();
//    if(point.x() > max.x())
//        x_max = point.x();
//    if(point.y() > max.y())
//        y_max = point.y();
//    min = QPointF(x_min, y_min);
//    max = QPointF(x_max, y_max);
//    avg = QPointF((x_max - x_min)/2., (y_max - y_min)/2.);
    if(centroid.size() == 1){
        min = QPoint(centroid[0].x() - 30, centroid[0].y() - 30);
        max = QPoint(centroid[0].x() + 30, centroid[0].y() + 30);
    }
}

void Coord::getMaxFrm(unsigned int m)
{
    maxFrm = m;
}

void Coord::getMaxSize(QSize s)
{
    max = QPoint(s.width(), s.height());
    min = QPoint(0, 0);
}

void Coord::initializeGL()
{

}

void drawTextCoord(QPainter *painter, QPointF p, QString t){
    painter->translate(p);
    painter->rotate(90);
    painter->drawText(-20, -20, 40, 40, Qt::AlignCenter, t);
    painter->rotate(-90);
    painter->translate(-p);
}

QPointF Coord::translateCoord(QPointF p){
    qreal halfW = this->width()/2;
    qreal halfH = this->height()/2;
    qreal X_MIN = -(halfW * 0.75);
    qreal X_MAX =   halfW * 0.75;
    qreal Y_MIN = -(halfH * 0.75);
    qreal Y_MAX =   halfH * 0.75;
    return (QPointF( (p.x() - min.x())*(X_MAX - X_MIN)/(max.x() - min.x()) + X_MIN ,
                     (p.y() - min.y())*(Y_MAX - Y_MIN)/(max.y() - min.y()) + Y_MIN ) );
}

QColor Coord::gradColor(QColor color, qreal percent){
    return QColor( color.red() + (255-color.red()) * percent * percent,
                   color.green() + (255-color.green()) * percent * percent,
                   color.blue() + (255-color.blue()) * percent * percent );
}

void Coord::render(QPainter *painter)
{
    qreal halfW = this->width()/2;
    qreal halfH = this->height()/2;
    // set (0,0) to the center of the canvas
    QPointF center(halfW, halfH);
    painter->translate(center.x(), center.y());
    // set the start angle to 0 o'clock;
    painter->rotate(-90); //***x->up, y->right***

    painter->setRenderHint(QPainter::Antialiasing);

    //draw the x-y coordinate system'
    QPen myPen(QColor(120, 120, 118));
    myPen.setWidth(2);
    painter->setPen(myPen);
    qreal rto = 0.8;
    qreal rto1 = 0.75;
    // X axis
    painter->drawLine(QPointF(-(halfW*rto), 0), QPointF(halfW*rto, 0));
    drawTextCoord(painter, QPointF(halfW*rto + 20, 0), "X");
    // max and min on X axis
    drawTextCoord(painter, QPointF(  halfW*rto1 ,  20), QString::number(int(/*max.x()*/25)));
    drawTextCoord(painter, QPointF(-(halfW*rto1), -20), QString::number(int(/*min.x()*/-25)));
    painter->drawLine(QPointF(  halfW*rto1 , -5), QPointF(  halfW*rto1 , 5));
    painter->drawLine(QPointF(-(halfW*rto1), -5), QPointF(-(halfW*rto1), 5));
    // max and min on Y axis
    painter->drawLine(QPointF(0, -(halfW*rto)), QPointF(0, halfW*rto));
    drawTextCoord(painter, QPointF(0, halfW*rto + 20), "Y");
    // max and min on Y axis
    drawTextCoord(painter, QPointF( 20,   halfH*rto1 ), QString::number(int(/*max.y()*/25)));
    drawTextCoord(painter, QPointF(-20, -(halfH*rto1)), QString::number(int(/*min.y()*/-25)));
    painter->drawLine(QPointF( -5,   halfH*rto1 ), QPointF( 5,   halfH*rto1));
    painter->drawLine(QPointF( -5, -(halfH*rto1)), QPointF( 5, -(halfH*rto1)));
    // center text
    if(centroid.size()>0){
        painter->rotate(90);//***x->ringt, y->down***
        // text for the center coordinate
        QString centerText = "Center ( " + QString::number(int(centroid[0].x()))+", "
                             +QString::number(int(centroid[0].y()))+" )";
        painter->drawText(-halfW+20, -halfH+20, 150, 40, Qt::AlignCenter, centerText);
        // text for current centroid
        QString currText = "Centroid ( " + QString::number(int(currCent.x()))+", "
                +QString::number(int(currCent.y()))+" )";
        painter->drawText(halfW-170, -halfH+20, 150, 40, Qt::AlignCenter, currText);

        painter->rotate(-90);
    }

    //draw positions of the centroids
//    for(int n = 30; n < cnt; n++){
//        QColor c = gradColor(_BLUE_, 1. - qreal(n)/cnt);
//        QPen myPen(c);
//        painter->setPen(myPen);
//        painter->setBrush(c);
//        painter->drawEllipse( translateCoord(centroid[n]), 2., 2.);
//    }

    int size = centroid.size();
    for(int n = 0; n < size; n++)
    {
        QColor c = gradColor(_BLUE_, 1. - qreal(n)/size);
        QPen myPen(c);
        painter->setPen(myPen);
        painter->setBrush(c);
        painter->drawEllipse( translateCoord(centroid[n]), 2., 2.);
    }
}

void Coord::resizeGL(int w, int h)
{

}
