#include <QResizeEvent>
#include <QPainter>
#include <math.h>
#include <QDebug>
#include <vector>
#include <limits>
#include <QTimer>
#include "coord.h"
//#include "cubehelix.h"
#include "colormap.h"

const QColor _BLUE_   = QColor(28, 120, 159);
const int COLOR_RANGE = 180;
const int startIndex = 20;
const qreal scale = 0.75; // <=1.0 the scale of the canvas that can be used to draw coordinates

const int Coord_COLOR_START = 30;
const int Coord_COLOR_RANGE = 90;


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
    centroid_min = QPoint(100000, 100000);
    centroid_max = QPoint(0, 0);
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

    if(centroid.size() > startIndex){
        if(point.x() < centroid_min.x()) centroid_min.setX(point.x());
        if(point.y() < centroid_min.y()) centroid_min.setY(point.y());
        if(point.x() > centroid_max.x()) centroid_max.setX(point.x());
        if(point.y() > centroid_max.y()) centroid_max.setY(point.y());
    }

    currFrm = currFrame;
    centroid.append(point);
    //qDebug() << "Coord Class" << currFrm << centroid;

    if(centroid.size() == startIndex){
        origin = QPoint(centroid[startIndex-1].x(), centroid[startIndex-1].y());
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
QPointF Coord::translate_image2canvas_center(QPointF p, bool XGreaterThanY){ // maximize the coordinates
    qreal ratio;
    qreal x, y;

    if(XGreaterThanY) {
        ratio   = scale*(this->width())/(centroid_max.x() - centroid_min.x());
        x       = (p.x() - centroid_min.x()) * ratio + scale*(-this->width()/2);
        y       = (p.y() - centroid_min.y()) * ratio + scale*(-this->height()/2);
    }
    else{
        ratio   = scale*(this->width())/(centroid_max.y() - centroid_min.y());
        x       = (p.x() - centroid_min.x()) * ratio + scale*(-this->width()/2);
        y       = (p.y() - centroid_min.y()) * ratio + scale*(-this->height()/2);
    }

    return QPointF(x, y);
}


QPointF Coord::translateCoord(QPointF p, qreal scale){
    if(scale < 1.0)
        scale = 1.0;
    QPointF org = QPointF( scale*(p.x() * win.x() / max.x() + win_off.x() /*- this->width()/2*/),
                           scale*(p.y() * win.y() / max.y() + win_off.y() /*- this->height()/2*/));
    return org;
}

QPointF Coord::translate_canvas2image(QPointF p)
{
    qreal W = scale*(this->width());
    qreal H = scale*(this->height());
    qreal x = (p.x() - (-W/2)) / W * (centroid_max.x()-centroid_min.x()) + centroid_min.x();
    qreal y = (p.y() - (-H/2)) / H * (centroid_max.y()-centroid_min.y()) + centroid_min.y();

    return QPointF(x, y);
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

    // *** draw color map bar indicating time old / new ***
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
        //QColor c = mapNumToHue(60, COLOR_RANGE, 0, bar_len, n);
        ColorMap colorMap;
        QColor c = colorMap.cubehelix(Coord_COLOR_START, Coord_COLOR_RANGE, 0, bar_len, n);
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
    QPointF center(halfW, halfH);
    // set (0,0) to the center of the canvas

    // set the start angle to 0 o'clock;
    painter->rotate(-90); //***x->up, y->right***

    painter->setRenderHint(QPainter::Antialiasing);


    //draw the x-y coordinate system
    QPen myPen(QColor(120, 120, 118));
    myPen.setWidth(1);
    painter->setPen(myPen);

    painter->rotate(90);//***x->right, y->down***
    painter->translate(halfW, halfH);

    //    QPointF topleft  = QPointF(win_off.x() - this->width()/2, win_off.y() - this->height()/2);
    //    QPointF topright = QPointF(topleft.x() + win.x(), topleft.y());
    //    QPointF botleft  = QPointF(topleft.x(), topleft.y() + win.y());
    //    QPointF botright  = QPointF(topleft.x()+win.x(), topleft.y() + win.y());


    // !!! need to move to other place (one-time calculation!) - start
    qreal indent    = 10;
    qreal topY      = indent-center.y();
    qreal leftX     = indent-center.x();
    qreal bottomY   = this->height()-indent-center.y();
    qreal rightX    = this->width()-indent-center.x();
    QPointF topleft  = QPointF(leftX, topY);
    QPointF topright = QPointF(rightX, topY);
    QPointF botleft  = QPointF(leftX, bottomY);
    QPointF botright  = QPointF(rightX, bottomY);
    painter->drawLine(topleft, topright); // ruler window - top
    painter->drawLine(topleft, botleft); // ruler window - left
    painter->drawLine(topright, botright); // ruler window - right
    painter->drawLine(botleft, botright); // ruler window - bottom


    bool Xgreater = (centroid_max.x() - centroid_min.x()) > (centroid_max.y() - centroid_min.y());
//    qDebug() << Xgreater;
    if(centroid.size() > startIndex){

        //draw units on the window edges
//        QPointF origin_tran = translateCoord(origin, 1.0);
        QPointF origin_tran = translate_image2canvas_center(origin, Xgreater);
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

    }
    // !!! need to move to other place (one-time calculation!) -end

    //QPointF ruler = translate_canvas2image(QPointF(this->width()/6, indent));
    //myPen.setWidth(5);
    //painter->setPen(myPen);
    //painter->drawPoint(ruler);


    // *** draw coordinates of each frame ***
    int size = centroid.size();
    for(int n = startIndex; n < size; n++/*n+=10*/)
    {
        //QColor c = gradColor(_BLUE_, 0.8 - qreal(n)/size);

        QColor c;
        //c = mapNumToHue(60, COLOR_RANGE, 0, size, n);
        ColorMap colorMap;
        c = colorMap.cubehelix(Coord_COLOR_START, Coord_COLOR_RANGE, 0, size, n);
        QPen penDot(c);
        painter->setPen(penDot);
        painter->setBrush(c);
//        QPointF visPoint = translateCoord(centroid[n], 5.0);
        QPointF visPoint = translate_image2canvas_center(centroid[n], Xgreater);
        //qDebug() << visPoint;
        painter->drawEllipse( visPoint, 2.0, 2.0);
    }

    drawColorBar(painter);

//    this->needUpdate = false;

}

void Coord::resizeGL(int w, int h)
{

}
