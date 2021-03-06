#include "shape.h"
#include <QPainter>
#include <QTimer>
#include <qdebug.h>
#include "cubicYFColormap.h"

const int Shape_COLOR_START = /*30*/0;
const int Shape_COLOR_RANGE = /*90*/120;

Shape::Shape(QObject *parent)
{
    clear();

//    QTimer* timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(1);
    bufferSize = width() * height() * 4;
    buffer = new unsigned char[bufferSize]();
    for (unsigned int i = 0; i < bufferSize; i++)
        buffer[i] = 255;
}

Shape::~Shape()
{
//    delete [] buffer;
}

void Shape::clear()
{
    begin   = 0;
    curr    = 0;
    maxIndex     = 1;
    maxTimeRatio = 1.0;
}

//void Shape::setNeedUpdate()
//{
//    needUpdate = true;
//}

void Shape::setBeginFrm(int beginFrame)
{
    begin = beginFrame;
    //std::cout << "SHAPE VIS BEGIN frame number " << begin << std::endl;
}

void Shape::setMaxFrm(int maxIdx, int maxFrm)
{
    maxIndex = maxIdx;
    maxFrame = maxFrm;
    //std::cout << "SHAPE VIS MAX frame number " << max << std::endl;
}

void Shape::setValue(QVector<float> v)
{
    value = v;
    //drawColorBar();
}

inline QPointF rotate(QPointF p, double theta/*, QPointF org*/){
    return QPointF(p.x()*cos(theta)-p.y()*sin(theta), p.x()*sin(theta)+p.y()*cos(theta))/* + org*/;
}

void Shape::updateContourNBleb(vector<Bleb> &bleb, QVector<QPoint> &smoothContour, QPoint &cent)
{

    qreal scl_c = 0.6;
//    QPolygon contour;
    for(int n = 0; n < smoothContour.size(); n++){
//        contour << QPoint((smoothContour[n].x()-cent.x()), (smoothContour[n].y()-cent.y()));/**0.5*/
        // x and y coordincates exchanged for drawing purpose
        QPointF contP  = QPointF(smoothContour[n].x()-cent.x(), smoothContour[n].y()-cent.y());
        QPointF contPR = rotate(contP, M_PI);
        int x = contPR.x()*scl_c+this->width()/2;
        int y = contPR.y()*scl_c+this->height()/2;
//        contour << QPoint(x, y);/**0.5*/

        unsigned int pixel = (y*width()+x)*4;
        if(pixel< 0 || pixel+3 >= bufferSize)
            continue;
        buffer[ pixel + 3 ] = 64;
        buffer[ pixel + 2 ] = 32;
        buffer[ pixel + 1 ] = 32;
        buffer[ pixel + 0 ] = 32;
    }

//    QVector<QPoint> contour;
//    for(int n = 0; n < smoothContour.size(); n++)
//        contour.push_back(QPoint((smoothContour[n].y()-cent.y()), (smoothContour[n].x()-cent.x())));/**0.5*/
//    //qDebug() << contour;

    qreal scl_b = 1.0;
    range    = MAXFRAMELEN;     //range = 5000 - begin;
    if (curr > range) curr = range;
    CubicYFColorMap colormap;
    QColor c = colormap.cubicYFmap(Shape_COLOR_START, Shape_COLOR_RANGE, 0, range, curr); // MAXFRAMELEN !!!!!

    for(int k = 0; k < bleb.size(); k++){ // one bleb
        vector<polarPoint>  polarPBunch = bleb[k].bunch_polar;
        int num = polarPBunch.size();
//        Point center = bleb[k].center;
        for(int l = 0; l < num; l++){ // one point
            polarPoint polarP = polarPBunch[l];
//            int x = center.x + polarP.r * cos(polarP.theta);
//            int y = center.y + polarP.r * sin(polarP.theta);
//            int x = (center.x + polarP.r * cos(polarP.theta) * scl_b)+this->width()/2;
//            int y = (center.y + polarP.r * sin(polarP.theta) * scl_b)+this->height()/2;
            int x = (polarP.r * cos(polarP.theta) - cent.x()) * scl_b + this->width()/2;
            int y = (polarP.r * sin(polarP.theta) - cent.y()) * scl_b + this->width()/2;
//            qDebug() << x << y;

//            points_in1frm.push_back(QPoint(x,y));
            unsigned int pixel = (y*width()+x)*4;
            if(pixel< 0 || pixel+3 >= bufferSize)
                continue;
            buffer[ pixel + 3] = c.alpha();
            buffer[ pixel + 2] = c.red();
            buffer[ pixel + 1] = c.green();
            buffer[ pixel + 0] = c.blue();
//            drawPoint(QPoint(x, y), c);
        }
    }

    /*
//    qDebug() << "centroid " << cent;
//    qDebug() << "contour_org" << smoothContour;
//    qDebug() << "contour_trans " << contour;

//    std::cout << "blebs and contours sizes load to file " << blebs.size() << std::endl;
//    for(unsigned int n = 0; n < blebs.size(); n++){
//        std::cout << "frame  " << n << std::endl;
//        for(int m = 0; m < contours[n].size(); m++){
//            std::cout << "(x "  << contours[n][m].x() << ", y " << contours[n][m].y() << ") ";
//        }

////        for(unsigned int m = 0; m < blebs[n].size(); m++){
////            std::cout << "bleb  " << m << std::endl;
////            for(unsigned int k = 0; k < blebs[n][m].bunch_polar.size(); k++){
////                std::cout << "(r " << blebs[n][m].bunch_polar[k].r
////                          << " , theta " << blebs[n][m].bunch_polar[k].theta << ") ";
////            }
////            std::cout << std::endl;
////        }
//        std::cout << std::endl;
//    }
*/

    curr++; // local counter

//    int showBarFrm = maxIndex - begin - 50 < range ? maxIndex - begin - 50 : range;
//    if(curr == showBarFrm)
    //        drawColorBar();
}

void Shape::updateRto(float minR, float maxR)
{
    maxTimeRatio = maxR;
    minTimeRatio = minR;
    //qDebug() << maxTimeRatio;
//    update();
    drawColorBar();
}

void Shape::setTempType(bool rt)
{
    roomT = rt;
}

void Shape::initializeGL()
{

}

void Shape::drawColorBarText(QPainter *painter){

    int bar_txt_y   = height()*6/7;
    int bar_txt_h   = 20;
    int bar_txt_w   = width()/8;
    QRect txt_old   = QRect(0,                      bar_txt_y, bar_txt_w, bar_txt_h);
    QRect txt_new   = QRect(width()-bar_txt_w-10,   bar_txt_y, bar_txt_w, bar_txt_h);

    QPen myPen(QColor(120, 120, 118));
    painter->setPen(myPen);
    painter->drawText(txt_old, Qt::AlignRight, "0");
    painter->drawText(txt_new, Qt::AlignLeft,  QString::number(maxIndex-begin));
}

void Shape::drawPoint(QPointF p, QColor c) // p1.y = p2.y
{
    unsigned int px = (int(p.y()) * width() + int(p.x())) * 4;
    if(px > 0 && px+3 < bufferSize){
/*
        buffer[ px + 3] = c.alpha();
        buffer[ px + 2] = c.red();
        buffer[ px + 1] = c.green();
        buffer[ px + 0] = c.blue(); */
        int tmp = (255 - c.alpha());
        buffer[px + 2] = (c.red()*c.alpha() + buffer[px + 2]*tmp)/255;
        buffer[px + 1] = (c.green()*c.alpha() + buffer[px + 1]*tmp)/255;
        buffer[px + 0] = (c.blue()*c.alpha() + buffer[px + 0]*tmp)/255;
        buffer[px + 3] = min(c.alpha() + buffer[px + 3], 255);

    }
}

void Shape::drawVLine(QPointF p1, QPointF p2, QColor c, int w)// p1.x = p2.x
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

void Shape::fillRect(QPointF p1, QPointF p2, QColor c)// p1: topleft, p2: bottomright
{
    for (int x = p1.x(); x < p2.x(); x++){
        QPointF up = QPointF(x, p1.y());
        QPointF dn = QPointF(x, p2.y());
        drawVLine(up, dn, c, 1);
    }
}

void Shape::drawColorBar()
{
    // *** draw color map bar indicating time old / new ***
    // ***x->right, y->down***
    int space       = width()/16;
    int bar_txt_y   = (height()-space)*7/8;
    int bar_txt_h   = 20;
    int bar_txt_w   = width()/8;
    //float rto   = float(maxIndex)/float(maxFrame) >= 1.0 ? 1.0 : float(maxIndex)/float(maxFrame);
    //float rto   = float(maxIndex)/5000. >= 1.0 ? 1.0 : float(maxIndex)/5000.;
    float rto   = float(maxIndex - begin)/MAXFRAMELEN >= 1.0 ? 1.0 : float(maxIndex - begin)/MAXFRAMELEN;
    int bar_h   = /*bar_txt_h*/10;
    int bar_len = width() - 2*(bar_txt_w + space)/**rto*/;

    int bar1frm_w   = bar_len/(maxIndex - begin) < 1 ? 1: bar_len/(maxIndex - begin);

    //int bar_x   = -(bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2);
    //int bar_y   = -(halfW - bar_txt_w - space);
    int bar_x   = (width() - bar_len)/2;
    int bar_y   = bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2;

//    QRect rect_empty = QRect(QPoint(bar_x+bar_len*rto, bar_y), QSize( bar_len*(1.0-rto), bar_h+1));
    QPointF empty_topleft  = QPoint(bar_x+bar_len*rto, bar_y);
    QPointF empty_botright = QPoint(bar_x+bar_len*rto + bar_len*(1.0-rto), bar_y+bar_h+1);
    fillRect(empty_topleft, empty_botright, QColor(220, 220, 220, 64));

    for(int n = 0; n < bar_len*rto; n+= bar1frm_w){
        CubicYFColorMap colorMap;
        QColor c = colorMap.cubicYFmap(Shape_COLOR_START, Shape_COLOR_RANGE, 0, bar_len, n);
        //QRect rect(QPoint(bar_x+n, bar_y), QSize(1, bar_h));
        drawVLine(QPoint(bar_x+n, bar_y), QPoint(bar_x+n, bar_y+bar_h), c, bar1frm_w);
    }

//    float   posRto = float(maxFrame)/5000. >= 1.0 ? 1.0 : float(maxFrame)/5000.;
    QColor posColor = QColor(128, 128, 128);
//    drawVLine(QPoint(bar_x+bar_len*posRto, bar_y-2), QPoint(bar_x+bar_len*posRto, bar_y+bar_h+2), posColor, 1);
    drawVLine(QPoint(bar_x+bar_len*maxTimeRatio, bar_y-2), QPoint(bar_x+bar_len*maxTimeRatio, bar_y+bar_h+2), posColor, 1);
    drawVLine(QPoint(bar_x+bar_len*minTimeRatio, bar_y-2), QPoint(bar_x+bar_len*minTimeRatio, bar_y+bar_h+2), posColor, 1);

}


void Shape::render(QPainter *painter)
{
//    if(!this->needUpdate)
//        return;

    // clean canvas
//    unsigned char *buffer_tmp = new unsigned char[bufferSize]();
//    for (unsigned int i = 0; i < bufferSize; i++)
//        buffer_tmp[i] = 255;

//    img = QImage(buffer_tmp, width(), height(), QImage::Format_ARGB32);
//    painter->drawImage(0, 0, img);

    painter->eraseRect(QRect(0, 0, width(), height()));

    img = QImage(buffer, width(), height(), QImage::Format_ARGB32);
    painter->drawImage(0, 0, img);

    //painter->setRenderHint(QPainter::Antialiasing);
    // draw Value
    if(roomT) // 25 - blue
        painter->setPen(QColor(0, 0, 128));
    else // 37 - red
        painter->setPen(QColor(128, 0, 0));
    for (unsigned int k = 0; k < value.size(); k++)
        painter->drawText(width() - 70, 30+k*15, 60, 20, Qt::AlignLeft, QString::number(value[k], 'f', 2));

    drawColorBarText(painter);

//    this->needUpdate = false;

}
