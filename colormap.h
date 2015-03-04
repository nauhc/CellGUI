#ifndef COLORMAP_H
#define COLORMAP_H
#include <math.h>
#include <QColor>

class ColorMap
{
public:
    ColorMap();
    QColor cubehelix(int strt, int range, int min, int max, int v);
    //QColor cubehelix(double fract);
};

#endif // COLORMAP_H
