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
    clear();

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

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

//    if(centroid.size() == 1){
//        min = QPoint(centroid[0].x() - 30, centroid[0].y() - 30);
//        max = QPoint(centroid[0].x() + 30, centroid[0].y() + 30);
//    }
}

void Coord::getMaxFrm(unsigned int m)
{
    maxFrm = m;
}

void Coord::getMaxSize(QSize s) // video size
{
    max = QPoint(s.width(), s.height());
    min = QPoint(0, 0);
    //qDebug() << this->width() << " " << this->height();

    qreal w = 0;
    qreal h = 0;
    qreal X_MIN = 0;
    qreal Y_MIN = 0;

    double rto_video = max.x()/max.y();

    if(rto_video >= 1){
        w = this->width() - 40;
        h = w * max.y()/max.x();
        X_MIN = 20;
        Y_MIN = (this->height() - h) / 2;
    }else{
        h = this->height() - 40;
        w = h * max.x()/max.y();
        X_MIN = (this->width() - w) / 2;
        Y_MIN = 20;
    }

    win = QPointF(w, h);
    win_off = QPointF(X_MIN, Y_MIN);
}

void Coord::clear()
{
    centroid.clear();
    currFrm = 0;
    maxFrm  = 1;
//    min = QPoint(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//    max = QPoint(0, 0);
    min = QPoint(0, 0);
    max = QPoint(50, 50);

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

// Coordinate
//QPointF Coord::translateCoord(QPointF p){
//    qreal halfW = this->width()/2;
//    qreal halfH = this->height()/2;
//    qreal X_MIN = -(halfW * 0.75);
//    qreal X_MAX =   halfW * 0.75;
//    qreal Y_MIN = -(halfH * 0.75);
//    qreal Y_MAX =   halfH * 0.75;
//    QPointF org = QPointF( (p.x() - min.x())*(X_MAX - X_MIN)/(max.x() - min.x()) + X_MIN ,
//                           (p.y() - min.y())*(Y_MAX - Y_MIN)/(max.y() - min.y()) + Y_MIN );

//    // ***x-> right, y-> down***
//    return ( /*QPointF(-org.y(), org.x())*/ org);
//}

// video size
QPointF Coord::translateCoord(QPointF p){ // center as (0, 0)

    QPointF org = QPointF( p.x() * win.x() / max.x() + win_off.x() - this->width()/2,
                           p.y() * win.y() / max.y() + win_off.y() - this->height()/2);

    return org;

}

QColor Coord::gradColor(QColor color, qreal percent){
    return QColor( color.red() + (255-color.red()) * percent * percent,
                   color.green() + (255-color.green()) * percent * percent,
                   color.blue() + (255-color.blue()) * percent * percent );
}

QColor mapNumToHue(int start, int range, int min, int max, int v){
    // start:   starting hue (0-360)
    // end:     ending hue (0-360)
    // min:     min value
    // max:     max value
    // v:       value
    QColor color;
    color.setHsv(start+range*v/(max-min), 255, 200);
    return color;
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


//    //draw the x-y coordinate system'
    QPen myPen(QColor(120, 120, 118));
    myPen.setWidth(2);
    painter->setPen(myPen);

//    qreal rto = 0.8;
    qreal rto1 = 0.75;
//    // Y axis
//    painter->drawLine(QPointF(-(halfW*rto), 0), QPointF(halfW*rto, 0));
//    drawTextCoord(painter, QPointF(-(halfW*rto + 20), 0), "Y");
//    // max and min on X axis
//    drawTextCoord(painter, QPointF(  halfW*rto1 ,  20), QString::number(int(/*max.x()*/-25)));
//    drawTextCoord(painter, QPointF(-(halfW*rto1), -20), QString::number(int(/*min.x()*/25)));
//    painter->drawLine(QPointF(  halfW*rto1 , -5), QPointF(  halfW*rto1 , 5));
//    painter->drawLine(QPointF(-(halfW*rto1), -5), QPointF(-(halfW*rto1), 5));
//    // X axis
//    painter->drawLine(QPointF(0, -(halfW*rto)), QPointF(0, halfW*rto));
//    drawTextCoord(painter, QPointF(0, halfW*rto + 20), "X");
//    // max and min on Y axis
//    drawTextCoord(painter, QPointF( 20,   halfH*rto1 ), QString::number(int(/*max.y()*/25)));
//    drawTextCoord(painter, QPointF(-20, -(halfH*rto1)), QString::number(int(/*min.y()*/-25)));
//    painter->drawLine(QPointF( -5,   halfH*rto1 ), QPointF( 5,   halfH*rto1));
//    painter->drawLine(QPointF( -5, -(halfH*rto1)), QPointF( 5, -(halfH*rto1)));
//    // Centroid text
//    painter->rotate(90);//***x->right, y->down***
//    if(centroid.size()>0){
//        // text for the center coordinate
//        QString centerText = "Center ( " + QString::number(int(centroid[0].x()))+", "
//                             +QString::number(int(centroid[0].y()))+" )";
//        painter->drawText(-halfW+20, -halfH+20, 150, 40, Qt::AlignCenter, centerText);
//        // text for current centroid
//        QString currText = "Centroid ( " + QString::number(int(currCent.x()))+", "
//                +QString::number(int(currCent.y()))+" )";
//        painter->drawText(halfW-170, -halfH+20, 150, 40, Qt::AlignCenter, currText);
//    }

    painter->rotate(90);//***x->right, y->down***

    // ruler window - top
    QPointF topleft  = QPointF(win_off.x() - this->width()/2, win_off.y() - this->height()/2);
    QPointF topright = QPointF(topleft.x() + win.x(), topleft.y());
    QPointF botleft  = QPointF(topleft.x(), topleft.y() + win.y());
    QPointF botright  = QPointF(topleft.x()+win.x(), topleft.y() + win.y());
    painter->drawLine(topleft, topright);
    // ruler window - left
    painter->drawLine(topleft, botleft);
    // ruler window - right
    painter->drawLine(topright, botright);
    // ruler window - bottom
    painter->drawLine(botleft, botright);

    int size = centroid.size();
    for(int n = 0; n < size; n++/*n+=10*/)
    {
        //QColor c = gradColor(_BLUE_, 0.8 - qreal(n)/size);

        QColor c;
        c.setHsv(60+qreal(n)/size*180, 255, 200);
        QPen penDot(c);
        painter->setPen(penDot);
        painter->setBrush(c);
        painter->drawEllipse( translateCoord(centroid[n]), 2.0, 2.0);
    }

    //painter->drawEllipse(QPointF(165, 279), 5, 5);

//    if(centroid.size() > 0){
//        myPen.setWidth(3);
//        myPen.setColor(QColor(247, 154, 1));
//        painter->setPen(myPen);
//        painter->drawEllipse( translateCoord(centroid[size-1]), 2.5, 2.5);
//    }

    // draw color map bar indicating time old / new
    // ***x->right, y->down***
    painter->drawText(-halfW, halfH-40, 150, 40, Qt::AlignCenter, "Old");
    painter->drawText(halfW-150, halfH-40, 160, 40, Qt::AlignCenter, "New");

    painter->rotate(-90); //***x->up, y->right***
    int xx = 5;
    int yy = halfH*2/180;
    for(int n = 0; n < 180; n++){
        QColor c = mapNumToHue(60, 180, 0, 180, n);
        //c.setHsv(60+n, 255, 200);
        myPen.setWidth(0);
        painter->setPen(c);
        painter->setBrush(QBrush(c));
        QRect rect(QPoint(-(halfW*rto1 + 60), (-halfH+30+yy*n)*rto1), QSize(xx, yy));
        painter->drawRect(rect);
    }


}

void Coord::resizeGL(int w, int h)
{

}
