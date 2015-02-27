#include <QResizeEvent>
#include <QPainter>
#include <math.h>
#include <QDebug>
#include <vector>
#include <limits>
#include <QTimer>
#include "coord.h"

QColor _BLUE_   = QColor(28, 120, 159);
int COLOR_RANGE = 180;


Coord::Coord(QWidget *parent)
{
    clear();

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

}

void Coord::clear()
{
    centroid.clear();
    currFrm = 0;
    maxFrm  = 1;
//    min = QPoint(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//    max = QPoint(0, 0);
    origin = QPointF(this->width()/2, this->height()/2);
    min = QPoint(0, 0);
    max = QPoint(50, 50);
    micMeter = 20;
    pixel = 300;
    micMtr_Pixel = micMeter/pixel;
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

    if(centroid.size() == 1){
        origin = QPoint(centroid[0].x(), centroid[0].y());
        //qDebug() << "centroid origin " << origin;
    }
}

void Coord::getMaxFrm(unsigned int m)
{
    maxFrm = m;
}

void Coord::setPixel(QString text)
{
    pixel = text.toDouble();
    micMtr_Pixel = micMeter/pixel;
    //cout << "micrometerPerPixel: " << micMtr_Pixel << endl;
}

void Coord::setMicMeter(QString text)
{
    micMeter = text.toDouble();
    micMtr_Pixel = micMeter/pixel;
//    cout << "micrometerPerPixel: " << micMtr_Pixel << endl;
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
QPointF Coord::translateCoord_center(QPointF p){ // center as (0, 0)
    QPointF org = QPointF( p.x() * win.x() / max.x() /*+ this->width()/2*/ /*- this->width()/2*/,
                           p.y() * win.y() / max.y() /*+ this->height()/2*/ /*- this->height()/2*/);

    return (org);
}

QPointF Coord::translateCoord(QPointF p){ // center as (0, 0)

    QPointF org = QPointF( p.x() * win.x() / max.x() + win_off.x() /*- this->width()/2*/,
                           p.y() * win.y() / max.y() + win_off.y() /*- this->height()/2*/);
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

void Coord::drawColorBar(QPainter *painter)
{
    qreal halfW = this->width()/2;
    qreal halfH = this->height()/2;
    QPointF center(halfW, halfH);

    // *** draw color map bar indicating time old / new ***
    painter->translate(center.x(), center.y());
    // ***x->right, y->down***
    int bar_txt_y   = halfH*3/4;
    int bar_txt_h   = 20;
    int bar_txt_w   = /*120*/halfW/4;
    QRect txt_old   = QRect(-halfW,           bar_txt_y, bar_txt_w, bar_txt_h);
    QRect txt_new   = QRect(halfW-bar_txt_w,  bar_txt_y, bar_txt_w, bar_txt_h);

    QPen myPen(QColor(120, 120, 118));
    painter->setPen(myPen);
    painter->drawText(txt_old, Qt::AlignRight, "Old");
    painter->drawText(txt_new, Qt::AlignLeft,  "New");

    painter->rotate(-90); //***x->up, y->right***
    int space   = /*50*/halfW/8;
    int bar_h   = /*bar_txt_h*/10;
    int bar_len = 2*(halfW - bar_txt_w - space);
    int bar_w   = bar_len/bar_len;
    int bar_x   = -(bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2);
    int bar_y   = -(halfW - bar_txt_w - space);

    for(int n = 0; n < bar_len; n++){
        QColor c = mapNumToHue(60, COLOR_RANGE, 0, bar_len, n);
        myPen.setWidth(0);
        painter->setPen(c);
        painter->setBrush(QBrush(c));
        QRect rect(QPoint(bar_x, bar_y+bar_w*n), QSize(bar_h, bar_w));
        painter->drawRect(rect);
    }

}

void Coord::render(QPainter *painter)
{
//    if(!this->needUpdate)
//        return;

    qreal halfW = this->width()/2;
    qreal halfH = this->height()/2;
    // set (0,0) to the center of the canvas

    // set the start angle to 0 o'clock;
    painter->rotate(-90); //***x->up, y->right***

    painter->setRenderHint(QPainter::Antialiasing);


    //draw the x-y coordinate system
    QPen myPen(QColor(120, 120, 118));
    myPen.setWidth(1);
    painter->setPen(myPen);

    painter->rotate(90);//***x->right, y->down***


    //    QPointF topleft  = QPointF(win_off.x() - this->width()/2, win_off.y() - this->height()/2);
    //    QPointF topright = QPointF(topleft.x() + win.x(), topleft.y());
    //    QPointF botleft  = QPointF(topleft.x(), topleft.y() + win.y());
    //    QPointF botright  = QPointF(topleft.x()+win.x(), topleft.y() + win.y());


    // !!! need to move to other place (one-time calculation!) - start
    qreal topY      = 20/*-center.y()*/;
    qreal leftX     = 20/*-center.x()*/;
    qreal bottomY   = this->height()-20/*-center.y()*/;
    qreal rightX    = this->width()-20/*-center.x()*/;
    QPointF topleft  = QPointF(leftX, topY);
    QPointF topright = QPointF(rightX, topY);
    QPointF botleft  = QPointF(leftX, bottomY);
    QPointF botright  = QPointF(rightX, bottomY);
    painter->drawLine(topleft, topright); // ruler window - top
    painter->drawLine(topleft, botleft); // ruler window - left
    painter->drawLine(topright, botright); // ruler window - right
    painter->drawLine(botleft, botright); // ruler window - bottom

    if(centroid.size() > 1){

        //draw units on the window edges
        QPointF origin_tran = translateCoord(origin);
        painter->drawLine(QPointF(origin_tran.x(), topY-5), QPointF(origin_tran.x(), topY+5)); //top
        painter->drawText(origin_tran.x()-10, topY+5+2, 20, 10, Qt::AlignCenter, "0" );
        painter->drawText(origin_tran.x()-10, topY+5+12, 20, 20, Qt::AlignCenter, "μm" );
        painter->drawLine(QPointF(origin_tran.x(), bottomY-5), QPointF(origin_tran.x(), bottomY+5)); // bottom
        painter->drawText(origin_tran.x()-10, bottomY-5-2-10, 20, 10, Qt::AlignCenter, "0" );
        painter->drawLine(QPointF(leftX-5, origin_tran.y()), QPointF(leftX+5, origin_tran.y())); // left
        painter->drawText(leftX+5+2, origin_tran.y()-5, 20, 10, Qt::AlignCenter, "0" );
        painter->drawText(leftX+5+2+20, origin_tran.y()-5, 20, 10, Qt::AlignCenter, "μm" );
        painter->drawLine(QPointF(rightX-5, origin_tran.y()), QPointF(rightX+5, origin_tran.y())); // right
        painter->drawText(rightX-5-15, origin_tran.y()-5, 20, 10, Qt::AlignCenter, "0" );

        /*
    qreal video2visScaleX = max.x()/win.x();
    qreal video2visScaleY = max.y()/win.y();
    qreal unit  = pixel/10.;
    qreal unitX = unit/video2visScaleX;
    qreal unitY = unit/video2visScaleY;

    int rightU = (max.x()-origin.x()-20)/unitX ; // number of units on the right
    int bottomU = (max.y()-origin.y()-20)/unitY; // number of units on the bottom
    for(int i = 1; i < rightU; i++){
        qreal xx = origin_tran.x() + i * unitX;
        if(xx > win_off.x() && xx < win.x()){
            painter->drawLine(QPointF(xx, topY-3), QPointF(xx, topY+3));
            painter->drawLine(QPointF(xx, bottomY-3), QPointF(xx, bottomY+3));
            painter->drawText(xx - 10, topY-3+10, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
            painter->drawText(xx - 10, bottomY-3-15, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
        }
    }
    for(int i = 1; i < bottomU; i++){
        qreal yy = origin_tran.y() + i * unitY;
        if(yy > win_off.y() && yy < win.y()){
            painter->drawLine(QPointF(leftX-3, origin_tran.y()+ i * unitY), QPointF(leftX+3, yy));
            painter->drawLine(QPointF(rightX-3, origin_tran.y()+ i * unitY), QPointF(rightX+3, yy));
            painter->drawText(leftX+3+5, yy-5, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
            painter->drawText(rightX-3-20, yy-10, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
        }
    }
    int leftU = (origin.x()-20)/unitX ; // number of units on the left
    int upU = (origin.y()-20)/unitY; // number of units on the up
    for(int i = 1; i < leftU; i++){
        qreal xx = origin_tran.x() - i * unitX;
        if(xx > win_off.x() && xx < win.x()){
            painter->drawLine(QPointF(xx, topY-3), QPointF(xx, topY+3));
            painter->drawLine(QPointF(xx, bottomY-3), QPointF(xx, bottomY+3));
            painter->drawText(xx - 10, topY-3+10, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
            painter->drawText(xx - 10, bottomY-3-15, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
        }
    }
    for(int i = 1; i < upU; i++){
        qreal yy = origin_tran.y() - i * unitY;
        if(yy > win_off.y() && yy < win.y()){
            painter->drawLine(QPointF(leftX-3, yy), QPointF(leftX+3, yy));
            painter->drawLine(QPointF(rightX-3, yy), QPointF(rightX+3, yy));
            painter->drawText(leftX+3+5, yy-5, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
            painter->drawText(rightX-3-20, yy-3-7, 20, 10, Qt::AlignCenter, QString::number(i*unit*micMtr_Pixel));
        }
    }
*/

    }
    // !!! need to move to other place (one-time calculation!) -end



    // *** draw coordinates of each frame ***
    int size = centroid.size();
    for(int n = 0; n < size; n++/*n+=10*/)
    {
        //QColor c = gradColor(_BLUE_, 0.8 - qreal(n)/size);

        QColor c;
        //c.setHsv(60+qreal(n)/size*180, 255, 200); //color.setHsv(start+range*v/(max-min), 255, 200);
        c = mapNumToHue(60, COLOR_RANGE, 0, size, n);
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

    drawColorBar(painter);

//    this->needUpdate = false;

}

void Coord::resizeGL(int w, int h)
{

}
