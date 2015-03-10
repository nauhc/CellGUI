#include "cubicYFColormap.h"


CubicYFColorMap::CubicYFColorMap()
{

}

QColor CubicYFColorMap::cubicYFmap(int strt, int range, int min, int max, int v)
{
    strt = strt < 0 ? 0 : strt;
    strt = strt > 255 ? 255 : strt;
    range = (strt + range) > 255 ? 255 - strt : range;

    double fract = double(v-min)/double(min-max)*double(range)/255.+double(strt+range)/255.;
    int index = fract * 255;
    int alpha_stt = 30;
    int alpha = (255-alpha_stt)/double(max-min)*double(v-min)+alpha_stt;

    return QColor(cubicYF[index][0]*255, cubicYF[index][1]*255, cubicYF[index][2]*255, alpha);
}
