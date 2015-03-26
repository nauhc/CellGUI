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

    if(!multv){
        QTimer* timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(1);
    }

    bufferSize = width() * height() * 4;
    buffer = new unsigned char[bufferSize]();
    for (int i = 0; i < bufferSize; i++)
        buffer[i] = 255;

}

void Coord::clear()
{
    coordScale = 2.;
//    coordScale = 4.;
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
//    pixel = 300;
    pixel = 150;
    micMtr_Pixel = micMeter/pixel;
    maxTimeRatio = 1.0;

}

Coord::~Coord()
{

}

void Coord::drawPoint(QPointF p, QColor c){
    unsigned int px = (int(p.y()) * width() + int(p.x())) * 4;
    if(px > 0 && px+3 < bufferSize){
//        buffer[ px + 3] = c.alpha();
//        buffer[ px + 2] = c.red();
//        buffer[ px + 1] = c.green();
//        buffer[ px + 0] = c.blue();
        int tmp = (255 - c.alpha());
        buffer[px + 2] = (c.red()*c.alpha() + buffer[px + 2]*tmp)/255;
        buffer[px + 1] = (c.green()*c.alpha() + buffer[px + 1]*tmp)/255;
        buffer[px + 0] = (c.blue()*c.alpha() + buffer[px + 0]*tmp)/255;
        buffer[px + 3] = c.alpha() + buffer[px + 3] <  255 ? c.alpha() + buffer[px + 3] : 255;

    }
}

void Coord::drawHLine(QPointF p1, QPointF p2, QColor c, int w) // p1.y = p2.y
{
    //int     line_w = 2;
    float   x1, x2;
    if(p1.x() < p2.x()){
        x1 = p1.x();
        x2 = p2.x();
    }
    else{
        x2 = p1.x();
        x1 = p2.x();
    }

    for (int y = 0; y < w; y++){
        for (int x = x1; x < x2; x++)
            drawPoint(QPointF(x,p1.y()+y), c);
    }
}

void Coord::drawVLine(QPointF p1, QPointF p2, QColor c, int w) // p1.x = p2.x
{
    //int     line_w = 2;
    float   y1, y2;
    if(p1.y() < p2.y()){
        y1 = p1.y();
        y2 = p2.y();
    }
    else{
        y2 = p1.y();
        y1 = p2.y();
    }

    for (int y = y1; y < y2; y++){
        for (int x = 0; x < w; x++)
            drawPoint(QPointF(p1.x()+x,y), c);
    }
}

void Coord::drawRuler()
{
    qreal   indent      = 10;
    qreal   topY        = indent/*-center.y()*/;
    qreal   leftX       = indent/*-center.x()*/;
    qreal   bottomY     = height()-indent/*-center.y()*/;
    qreal   rightX      = width()-indent/*-center.x()*/;
    QPointF topleft     = QPointF(leftX, topY);
    QPointF topright    = QPointF(rightX, topY);
    QPointF botleft     = QPointF(leftX, bottomY);
    QPointF botright    = QPointF(rightX, bottomY);
    QColor  color       = QColor(120, 120, 118, 128);
    int     lineWid     = 2;
    //qDebug() << topleft << topright << botleft << botright;

    // ruler window
    drawHLine(topleft, topright, color, lineWid);
    drawHLine(botleft, botright, color, lineWid);
    drawVLine(topleft, botleft, color, lineWid);
    drawVLine(topright, botright, color, lineWid);

    // ruler units
    drawVLine(QPointF(width()/2, topY-5),    QPointF(width()/2, topY+5), color, lineWid); //top
    drawVLine(QPointF(width()/2, bottomY-5), QPointF(width()/2, bottomY+5), color, lineWid); // bottom
    drawHLine(QPointF(leftX-5, height()/2), QPointF(leftX+5, height()/2), color, lineWid); // left
    drawHLine(QPointF(rightX-5, height()/2), QPointF(rightX+5, height()/2), color, lineWid); // right

    float quarterxx = (rightX - leftX)/4.0;
    float unit_x1 = quarterxx + leftX;
    float unit_x2 = rightX - quarterxx;
    drawVLine(QPointF(unit_x1, topY-5),    QPointF(unit_x1, topY+5), color, lineWid); //topxx1
    drawVLine(QPointF(unit_x2, topY-5),    QPointF(unit_x2, topY+5), color, lineWid); //topxx2
    drawVLine(QPointF(unit_x1, bottomY-5), QPointF(unit_x1, bottomY+5), color, lineWid); // bottomxx1
    drawVLine(QPointF(unit_x2, bottomY-5), QPointF(unit_x2, bottomY+5), color, lineWid); // bottomxx2

    float quarteryy = (bottomY - topY)/4.0;
    float unit_y1 = quarteryy + topY;
    float unit_y2 = bottomY - quarterxx;
    drawHLine(QPointF(leftX-5, unit_y1), QPointF(leftX+5, unit_y1), color, lineWid); // leftyy1
    drawHLine(QPointF(leftX-5, unit_y2), QPointF(leftX+5, unit_y2), color, lineWid); // leftyy2
    drawHLine(QPointF(rightX-5, unit_y1), QPointF(rightX+5, unit_y1), color, lineWid); // rightyy1
    drawHLine(QPointF(rightX-5, unit_y2), QPointF(rightX+5, unit_y2), color, lineWid); // rightyy2
}

void Coord::fillRect(QPointF p1, QPointF p2, QColor c) // p1: topleft, p2: bottomright
{
    for (int x = p1.x(); x < p2.x(); x++){
        QPointF up = QPointF(x, p1.y());
        QPointF dn = QPointF(x, p2.y());
        drawVLine(up, dn, c, 1);
    }
}

void Coord::drawColorBar()
{
    // *** draw color map bar indicating time old / new ***
    // ***x->right, y->down***
    int     space       = width()/16;
    int     bar_txt_y   = (height()-space)*6/7;
    int     bar_txt_h   = 20;
    int     bar_txt_w   = width()/8;
    //float   rto         = float(maxIdx)/float(maxFrm) >= 1.0 ? 1.0 : float(maxIdx)/float(maxFrm);
    float   rto         = float(maxIdx)/float(5000) >= 1.0 ? 1.0 : float(maxIdx)/float(5000);
    int     bar_h       = /*bar_txt_h*/10;
    int     bar_len     = width() - 2*(bar_txt_w + space)/**rto*/;

    int     bar1frm_w   = bar_len/(maxIdx - minFrm) < 1 ? 1: bar_len/(maxIdx - minFrm);

    //int   bar_x       = -(bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2);
    //int   bar_y       = -(halfW - bar_txt_w - space);
    int     bar_x       = (width() - bar_len)/2;
    int     bar_y       = bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2;

    QRect rect_empty = QRect(QPoint(bar_x+bar_len*rto, bar_y), QSize( bar_len*(1.0-rto), bar_h+1));
    fillRect(rect_empty.topLeft(), rect_empty.bottomRight(), QColor(220, 220, 220, 64));

    for(int n = 0; n < bar_len*rto; n+= bar1frm_w){
        CubicYFColorMap colorMap;
        QColor c = colorMap.cubicYFmap(Coord_COLOR_START, Coord_COLOR_RANGE, 0, bar_len, n);
        //QRect rect(QPoint(bar_x+n, bar_y), QSize(1, bar_h));
        drawVLine(QPoint(bar_x+n, bar_y), QPoint(bar_x+n, bar_y+bar_h), c, bar1frm_w);
    }

//    float  posRto = float(maxFrm+minFrm)/5000. >= 1.0 ? 1.0 : float(maxFrm+minFrm)/5000.;
    QColor posColor = QColor(128, 128, 128);
//    drawVLine(QPoint(bar_x+bar_len*posRto, bar_y-2), QPoint(bar_x+bar_len*posRto, bar_y+bar_h+2), posColor, 1);
    drawVLine(QPoint(bar_x+bar_len*maxTimeRatio, bar_y-2), QPoint(bar_x+bar_len*maxTimeRatio, bar_y+bar_h+2), posColor, 1);

//    qDebug() << maxTimeRatio;

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

    //    QPointF currCoord = (point-origin)*coordScale+center;
    QPointF currCoord = QPointF((point.x() - origin.x())*coordScale+center.x(), (point.y() - origin.y())*coordScale+center.y());

//    unsigned int range = maxFrm - minFrm;
    unsigned int range = 5000 - minFrm;
    if(currFrm > range) currFrm = range;
    CubicYFColorMap colormap;
    QColor c = colormap.cubicYFmap(Coord_COLOR_START, Coord_COLOR_RANGE, 0, range, currFrm); // 5000 !!!!!

    int r = 2;
    for(int y = -r; y <= r; y++){
        for(int x = -r; x <= r; x++){
            QPointF p = currCoord + QPointF(float(x), float(y));
            if(x*x+y*y <= r*r /*&& x*x+y*y >= (r-1)*(r-1)*/)
                drawPoint(p, c);
        }
    }

    //drawPoint(currCoord, c);

    currFrm++;
    update();

    //qDebug() << point << origin;
}

void Coord::updateRto(float r)
{
    maxTimeRatio = r;
//    update();
//    qDebug() << maxTimeRatio;
    drawColorBar();

}

void Coord::setMaxFrm(unsigned int im, unsigned int fm)
{
    maxFrm = fm;
    maxIdx = im;
    //qDebug() << fm << im;
//    update();
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
    drawRuler();
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

void Coord::drawColorBarText(QPainter *painter)
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
}

void Coord::render(QPainter *painter)
{
    //    if(!this->needUpdate)
    //        return;
//    static int cnt = 0;
//    qDebug() << "Calling Coord Render()" << cnt++;

    // clear canvas
    painter->eraseRect(0, 0, width(), height());

    img = QImage(buffer, width(), height(), QImage::Format_ARGB32);
    painter->drawImage(0, 0, img);


//    painter->setRenderHint(QPainter::Antialiasing);

    // draw Value
    painter->setPen(QColor(128, 0, 0));
    painter->drawText(width() - 70, 30, 60, 20, Qt::AlignLeft, QString::number(value, 'f', 2));

    qreal halfW = this->width()/2;
    qreal halfH = this->height()/2;
    QPointF center(halfW, halfH);
    // set (0,0) to the center of the canvas

   //draw the x-y coordinate system
    QPen myPen(QColor(120, 120, 118));
    myPen.setWidth(1);
    painter->setPen(myPen);

    painter->translate(halfW, halfH);

    qreal indent    = 10;
    qreal topY      = indent-center.y();
    qreal leftX     = indent-center.x();
    qreal bottomY   = this->height()-indent-center.y();
    qreal rightX    = this->width()-indent-center.x();

    // draw text for ruler
    if(centroid.size() > startIndex){
        //draw units on the window edges
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
        painter->drawText(unit_x1-10, topY+5+2, 20, 10, Qt::AlignCenter, unitx);
        painter->drawText(unit_x2-10, topY+5+2, 20, 10, Qt::AlignCenter, unitx);
        painter->drawText(unit_x1-10, bottomY-5-2-10, 20, 10, Qt::AlignCenter, unitx );
        painter->drawText(unit_x2-10, bottomY-5-2-10, 20, 10, Qt::AlignCenter, unitx );

        float quarteryy = (bottomY - topY)/4.0;
        QString unity = QString::number(quarteryy/coordScale*micMtr_Pixel,'f', 1);
        float unit_y1 = quarteryy + topY;
        float unit_y2 = bottomY - quarterxx;
        painter->drawText(leftX+5+2,    unit_y1-5, 20, 10, Qt::AlignCenter, unity );
        painter->drawText(leftX+5+2,    unit_y2-5, 20, 10, Qt::AlignCenter, unity );
        painter->drawText(rightX-5-20,  unit_y1-5, 20, 10, Qt::AlignCenter, unity );
        painter->drawText(rightX-5-20,  unit_y2-5, 20, 10, Qt::AlignCenter, unity );
    }

    drawColorBarText(painter);

    //    this->needUpdate = false;

}

void Coord::resizeGL(int w, int h)
{

}
