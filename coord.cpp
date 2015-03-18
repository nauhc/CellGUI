#include <QResizeEvent>
#include <QPainter>
#include <math.h>
#include <QDebug>
#include <vector>
#include <limits>
#include <QTimer>
#include "coord.h"
//#include "cubehelix.h"
//#include "colormap.h"
#include "cubicYFColormap.h"

const QColor _BLUE_   = QColor(28, 120, 159);
//const int COLOR_RANGE = 180;
const int startIndex = 1;
const qreal shrink = 0.75; // <=1.0 the scale of the canvas that can be used to draw coordinates

const int Coord_COLOR_START = /*30*/0;
const int Coord_COLOR_RANGE = /*90*/120;


Coord::Coord(QWidget *parent, bool multv)
{
    clear();

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

//    bufferSize = width() * height() * 4;
//    buffer = new unsigned char[bufferSize]();
//    for (int i = 0; i < bufferSize; i++)
//        buffer[i] = 255;

}

void Coord::clear()
{
    coordScale = 2.;
    centroid.clear();
    currFrm = 0;
    maxFrm  = 1;
    //range   = 5000;
    origin = QPointF(this->width()/2, this->height()/2);
    min = QPoint(0, 0);
    max = QPoint(50, 50);
    centroid_min = QPoint(100000, 100000);
    centroid_max = QPoint(0, 0);
    micMeter = 20;
    pixel = 300;
    micMtr_Pixel = micMeter/pixel;

//    if(buffer != NULL)
//        delete[] buffer;
//    bufferSize = width() * height() * 4;
//    buffer = new unsigned char[bufferSize]();
//    for (unsigned int i = 0; i < bufferSize; i++)
//        buffer[i] = 255;

}

Coord::~Coord()
{

}

void Coord::updateCoord(QPointF point, int currFrame)
{
    QPointF center(width()/2, height()/2);

    if(centroid.size() == startIndex){
        origin = QPointF(centroid[startIndex-1].x(), centroid[startIndex-1].y());
        //qDebug() << "centroid origin " << origin;
    }

    currCent = QPoint(point.x(), point.y());

    if(centroid.size() > startIndex){
        if(point.x() < centroid_min.x()) centroid_min.setX(point.x());
        if(point.y() < centroid_min.y()) centroid_min.setY(point.y());
        if(point.x() > centroid_max.x()) centroid_max.setX(point.x());
        if(point.y() > centroid_max.y()) centroid_max.setY(point.y());
    }

    centroid.append(point);

////    QPointF currCoord = (point-origin)*coordScale+center;
//    QPointF currCoord = QPointF((point.x() - origin.x())*coordScale, (point.y() - origin.y())*coordScale);
//    qDebug() << currCoord;

//    centroid.append(currCoord);

//    if(currFrm > range) currFrm = range;
//    CubicYFColorMap colormap;
//    QColor c = colormap.cubicYFmap(Coord_COLOR_START, Coord_COLOR_RANGE, 0, range, currFrm); // 5000 !!!!!

//    unsigned int px = (int(currCoord.y()) * width() + int(currCoord.x())) * 4;
//    //unsigned int px = (height()/2 * width() + width()/2)*4;
//    if(px > 0 || px+3 <= bufferSize){
//        buffer[ px + 3] = /*c.alpha()*/255;
//        buffer[ px + 2] = c.red();
//        buffer[ px + 1] = c.green();
//        buffer[ px + 0] = c.blue();
//    }

    currFrm++;
    update();

    //qDebug() << point << origin;
}

void Coord::setMaxFrm(unsigned int im, unsigned int fm)
{
    maxFrm = fm;
    maxIdx = im;
    //qDebug() << fm << im;
}

void Coord::setBeginFrm(unsigned int m)
{
    minFrm = m;
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

void Coord::setScale(qreal scale)
{
    coordScale = scale;
}

void Coord::setMaxSize(QSize s) // video size
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

void Coord::setValue(float v)
{
    value = v;
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
        ratio   = shrink*(this->width())/(centroid_max.x() - centroid_min.x());
        x       = (p.x() - centroid_min.x()) * ratio + shrink*(-this->width()/2);
        y       = (p.y() - centroid_min.y()) * ratio + shrink*(-this->height()/2);
    }
    else{
        ratio   = shrink*(this->width())/(centroid_max.y() - centroid_min.y());
        x       = (p.x() - centroid_min.x()) * ratio + shrink*(-this->width()/2);
        y       = (p.y() - centroid_min.y()) * ratio + shrink*(-this->height()/2);
    }

    return QPointF(x, y);
}

QPointF Coord::translate_image2canvas_center(QPointF p, qreal ratio)
{
//    qreal x = (p.x() - centroid_min.x()) * ratio /*+ ratio*(-this->width()/2)*/;
//    qreal y = (p.y() - centroid_min.y()) * ratio /*+ ratio*(-this->height()/2)*/;
    qreal x = (p.x() - origin.x()) * ratio /*+ ratio*(-this->width()/2)*/;
    qreal y = (p.y() - origin.y()) * ratio /*+ ratio*(-this->height()/2)*/;

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
    qreal W = shrink*(this->width());
    qreal H = shrink*(this->height());
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
    int bar_txt_y   = halfH*3/4-10;
    int bar_txt_h   = 20;
    int bar_txt_w   = /*120*/halfW/4;
    QRect txt_old   = QRect(-halfW,             bar_txt_y, bar_txt_w, bar_txt_h);
    QRect txt_new   = QRect(halfW-bar_txt_w-10, bar_txt_y, bar_txt_w, bar_txt_h);

    QPen myPen(QColor(120, 120, 118));
    painter->setPen(myPen);
    painter->drawText(txt_old, Qt::AlignRight, "0");
    painter->drawText(txt_new, Qt::AlignLeft,  QString::number(maxIdx));

    painter->rotate(-90); //***x->up, y->right***
    float rto   = float(maxIdx)/float(maxFrm) >= 1.0 ? 1.0 : float(maxIdx)/float(maxFrm);
    int space   = /*50*/halfW/8;
    int bar_h   = /*bar_txt_h*/10;
    int bar_len = 2*(halfW - bar_txt_w - space)/**rto*/;
    int bar_w   = bar_len/bar_len;
    int bar_x   = -(bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2);
    int bar_y   = -(halfW - bar_txt_w - space);

    QRect rect_empty = QRect(QPoint(bar_x, bar_y+bar_len*rto), QSize(bar_h, bar_len*(1.0-rto)));
    painter->fillRect(rect_empty, QBrush(QColor(220, 220, 220, 128)));
    for(int n = 0; n < bar_len*rto; n++){
        //QColor c = mapNumToHue(60, COLOR_RANGE, 0, bar_len, n);
        CubicYFColorMap colorMap;
        QColor c = colorMap.cubicYFmap(Coord_COLOR_START, Coord_COLOR_RANGE, 0, bar_len, n);
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

    // clear canvas
    painter->eraseRect(0, 0, width(), height());
    painter->setRenderHint(QPainter::Antialiasing);


    // draw Value
    painter->setPen(QColor(128, 0, 0));
    painter->drawText(width() - 70, 30, 60, 20, Qt::AlignLeft, QString::number(value, 'e', 1));

    qreal halfW = this->width()/2;
    qreal halfH = this->height()/2;
    QPointF center(halfW, halfH);
    // set (0,0) to the center of the canvas

    // set the start angle to 0 o'clock;
    painter->rotate(-90); //***x->up, y->right***



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


//    bool Xgreater = (centroid_max.x() - centroid_min.x()) > (centroid_max.y() - centroid_min.y());
    //    qDebug() << Xgreater;
    if(centroid.size() > startIndex){
        //draw units on the window edges
        painter->drawLine(QPointF(0, topY-5),    QPointF(0, topY+5)); //top
        painter->drawLine(QPointF(0, bottomY-5), QPointF(0, bottomY+5)); // bottom
        painter->drawLine(QPointF(leftX-5, 0), QPointF(leftX+5, 0)); // left
        painter->drawLine(QPointF(rightX-5, 0), QPointF(rightX+5, 0)); // right
        painter->drawText(0-10, topY+5+2, 20, 10, Qt::AlignCenter, "0" );
        painter->drawText(0-10, topY+5+12, 20, 20, Qt::AlignCenter, "μm" );
        painter->drawText(0-10, bottomY-5-2-10, 20, 10, Qt::AlignCenter, "0" );
        painter->drawText(leftX+5+2, 0-5, 20, 10, Qt::AlignCenter, "0" );
        painter->drawText(leftX+5+2+20, 0-5, 20, 10, Qt::AlignCenter, "μm" );
        painter->drawText(rightX-5-15, 0-5, 20, 10, Qt::AlignCenter, "0" );

        float quarterxx = (rightX - leftX)/4.0;
        QString unitx = QString::number(quarterxx/coordScale*micMtr_Pixel,'f', 1);
        float unit_x1 = quarterxx + leftX;
        float unit_x2 = rightX - quarterxx;
        painter->drawLine(QPointF(unit_x1, topY-5),    QPointF(unit_x1, topY+5)); //topxx1
        painter->drawLine(QPointF(unit_x2, topY-5),    QPointF(unit_x2, topY+5)); //topxx2
        painter->drawText(unit_x1-10, topY+5+2, 20, 10, Qt::AlignCenter, unitx);
        painter->drawText(unit_x1-10, topY+5+12, 20, 20, Qt::AlignCenter, "μm" );
        painter->drawText(unit_x2-10, topY+5+2, 20, 10, Qt::AlignCenter, unitx);
        painter->drawText(unit_x2-10, topY+5+12, 20, 20, Qt::AlignCenter, "μm" );
        painter->drawLine(QPointF(unit_x1, bottomY-5), QPointF(unit_x1, bottomY+5)); // bottomxx1
        painter->drawLine(QPointF(unit_x2, bottomY-5), QPointF(unit_x2, bottomY+5)); // bottomxx2
        painter->drawText(unit_x1-10, bottomY-5-2-10, 20, 10, Qt::AlignCenter, unitx );
        painter->drawText(unit_x2-10, bottomY-5-2-10, 20, 10, Qt::AlignCenter, unitx );

        float quarteryy = (bottomY - topY)/4.0;
        QString unity = QString::number(quarteryy/coordScale*micMtr_Pixel,'f', 1);
        float unit_y1 = quarteryy + topY;
        float unit_y2 = bottomY - quarterxx;
        painter->drawLine(QPointF(leftX-5, unit_y1), QPointF(leftX+5, unit_y1)); // leftyy1
        painter->drawLine(QPointF(leftX-5, unit_y2), QPointF(leftX+5, unit_y2)); // leftyy2
        painter->drawText(leftX+5+2,    unit_y1-5, 20, 10, Qt::AlignCenter, unity );
        painter->drawText(leftX+5+2+20, unit_y1-5, 20, 10, Qt::AlignCenter, "μm" );
        painter->drawText(leftX+5+2,    unit_y2-5, 20, 10, Qt::AlignCenter, unity );
        painter->drawText(leftX+5+2+20, unit_y2-5, 20, 10, Qt::AlignCenter, "μm" );
        painter->drawLine(QPointF(rightX-5, unit_y1), QPointF(rightX+5, unit_y1)); // rightyy1
        painter->drawLine(QPointF(rightX-5, unit_y2), QPointF(rightX+5, unit_y2)); // rightyy2
        painter->drawText(rightX-5-20,  unit_y1-5, 20, 10, Qt::AlignCenter, unity );
        painter->drawText(rightX-5-20,  unit_y2-5, 20, 10, Qt::AlignCenter, unity );
    }
    // !!! need to move to other place (one-time calculation!) -end

    //QPointF ruler = translate_canvas2image(QPointF(this->width()/6, indent));
    //myPen.setWidth(5);
    //painter->setPen(myPen);
    //painter->drawPoint(ruler);


    // *** draw coordinates of each frame ***
    //int size = centroid.size();
    int size = centroid.size() > maxFrm ? maxFrm : centroid.size();
    int range = maxFrm - minFrm;
    for(int n = startIndex; n < size; n++/*n+=10*/)
    {
        CubicYFColorMap colorMap;
        QColor c = colorMap.cubicYFmap(Coord_COLOR_START, Coord_COLOR_RANGE, 0, range, n<=range?n:range);
        QPen penDot(c);
        painter->setPen(penDot);
        painter->setBrush(c);
        QPointF visPoint = translate_image2canvas_center(centroid[n], coordScale);
        //qDebug() << visPoint;
        painter->drawEllipse( visPoint, 2.0, 2.0);
    }

//    painter->translate(-halfW, -halfH);
//    img = QImage(buffer, width(), height(), QImage::Format_ARGB32);
//    painter->drawImage(0, 0, img);
//    painter->translate(halfW, halfH);

    drawColorBar(painter);

    //    this->needUpdate = false;

}

void Coord::resizeGL(int w, int h)
{

}
