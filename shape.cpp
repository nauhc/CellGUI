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

void Shape::setValue(float v)
{
    value = v;
}

void Shape::updateContourNBleb(QVector<Bleb> &bleb, QVector<QPoint> &smoothContour, QPoint &cent)
{

    qreal scl = 0.6;
//    QPolygon contour;
    for(int n = 0; n < smoothContour.size(); n++){
//        contour << QPoint((smoothContour[n].x()-cent.x()), (smoothContour[n].y()-cent.y()));/**0.5*/
        // x and y coordincates exchanged for drawing purpose
        int x = (smoothContour[n].y()-cent.y())*scl+this->width()/2;
        int y = (smoothContour[n].x()-cent.x())*scl+this->height()/2;
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

//    contours.push_back(contour);
    range    = maxFrame - begin;
    if (curr > range) curr = range;
    CubicYFColorMap colormap;
    QColor c = colormap.cubicYFmap(Shape_COLOR_START, Shape_COLOR_RANGE, 0, range, curr); // 5000 !!!!!

    for(int k = 0; k < bleb.size(); k++){ // one bleb
        vector<polarPoint>  polarPBunch = bleb[k].bunch_polar;
        int num = polarPBunch.size();
        Point center = bleb[k].center;
        for(int l = 0; l < num; l++){ // one point
            polarPoint polarP = polarPBunch[l];
//            int x = center.x + polarP.r * cos(polarP.theta);
//            int y = center.y + polarP.r * sin(polarP.theta);
            int x = (center.x + polarP.r * cos(polarP.theta))+this->width()/2;
            int y = (center.y + polarP.r * sin(polarP.theta))+this->height()/2;
//            points_in1frm.push_back(QPoint(x,y));
            unsigned int pixel = (y*width()+x)*4;
            if(pixel< 0 || pixel+3 >= bufferSize)
                continue;
            buffer[ pixel + 3] = c.alpha();
            buffer[ pixel + 2] = c.red();
            buffer[ pixel + 1] = c.green();
            buffer[ pixel + 0] = c.blue();
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

    int showBarFrm = maxIndex - begin - 50 < range ? maxIndex - begin - 50 : range;
    if(curr == showBarFrm)
        drawColorBar();
}

void Shape::initializeGL()
{

}

QColor _mapNumToHue_(int start, int range, int min, int max, int v){
    // start:   starting hue (0-360)
    // end:     ending hue (0-360)
    // min:     min value
    // max:     max value
    // v:       value
    QColor color;
    color.setHsv(start+range*v/(max-min), 255, 200);
    return color;
}


void Shape::drawColorBar(QPainter *painter){ //


//    halfW = this->width()/2;
//    halfH = this->height()/2;

//    painter->rotate(90); //***x->right, y->down***

    //int bar_txt_y   = halfH*3/4;
//    int bar_txt_h   = 20;
//    int bar_txt_w   = halfW/4;
//    QRect txt_old   = QRect(-halfW,              bar_txt_y, bar_txt_w, bar_txt_h);
//    QRect txt_new   = QRect(halfW-bar_txt_w-10,  bar_txt_y, bar_txt_w, bar_txt_h);

    int bar_txt_y   = height()*6/7;
    int bar_txt_h   = 20;
    int bar_txt_w   = width()/8;
    QRect txt_old   = QRect(0,                      bar_txt_y, bar_txt_w, bar_txt_h);
    QRect txt_new   = QRect(width()-bar_txt_w-10,   bar_txt_y, bar_txt_w, bar_txt_h);

    QPen myPen(QColor(120, 120, 118));
    painter->setPen(myPen);
    painter->drawText(txt_old, Qt::AlignRight, "0");
    painter->drawText(txt_new, Qt::AlignLeft,  QString::number(maxIndex));


//    painter->rotate(-90); //***x->up, y->right***
//    float rto   = float(maxIndex)/float(maxFrame) > 1.0 ? 1.0 : float(maxIndex)/float(maxFrame);
//    int space   = halfW/8;
//    int bar_h   = 10;
//    int bar_len = 2*(halfW - bar_txt_w - space);
//    int bar_w   = bar_len/bar_len;
//    int bar_x   = -(bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2);
//    int bar_y   = -(halfW - bar_txt_w - space);

//    QRect rect_empty = QRect(QPoint(bar_x, bar_y+bar_len*rto), QSize(bar_h, bar_len*(1.0-rto)));
//    painter->fillRect(rect_empty, QBrush(QColor(220, 220, 220, 128)));

//    for(int n = 0; n < bar_len*rto; n++){
//        CubicYFColorMap colorMap;
//        QColor c = colorMap.cubicYFmap(Shape_COLOR_START, Shape_COLOR_RANGE, 0, bar_len, n);
//        myPen.setWidth(0);
//        painter->setPen(c);
//        painter->setBrush(QBrush(c));
//        QRect rect(QPoint(bar_x, bar_y+bar_w*n), QSize(bar_h, bar_w));
//        painter->drawRect(rect);
//    }

}

void Shape::drawPoint(QPointF p, QColor c) // p1.y = p2.y
{
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
    float rto   = float(maxIndex)/float(maxFrame) >= 1.0 ? 1.0 : float(maxIndex)/float(maxFrame);
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

}


void Shape::render(QPainter *painter)
{
//    if(!this->needUpdate)
//        return;

    // clean canvas
    unsigned char *buffer_tmp = new unsigned char[bufferSize]();
    for (unsigned int i = 0; i < bufferSize; i++)
        buffer_tmp[i] = 255;

    img = QImage(buffer_tmp, width(), height(), QImage::Format_ARGB32);
    painter->drawImage(0, 0, img);

    painter->setRenderHint(QPainter::Antialiasing);


    img = QImage(buffer, width(), height(), QImage::Format_ARGB32);
    painter->drawImage(0, 0, img);

    // draw Value
    painter->setPen(QColor(128, 0, 0));
    painter->drawText(width() - 70, 30, 60, 20, Qt::AlignLeft, QString::number(value, 'f', 2));

//    halfW = this->width()/2;
//    halfH = this->height()/2;

//    // set (0,0) to the center of the canvas
//    QPointF center(halfW, halfH);

//    painter->translate(center.x(), center.y());
//    painter->rotate(-90); //***x->up, y->right***
    drawColorBar(painter);

//    this->needUpdate = false;

}
