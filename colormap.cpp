#include "colormap.h"

ColorMap::ColorMap()
{
}

QColor ColorMap::cubehelix(int strt, int range, int min, int max, int v)
{
    // start:   starting value (0 - 255)
    // range:   from start to ending value (0 - 255-start)
    // min:     min value
    // max:     max value
    // v:       value

    strt = strt < 0 ? 0 : strt;
    strt = strt > 255 ? 255 : strt;
    range = (strt + range) > 255 ? 255 - strt : range;

    //    double fract = double(v-min)/double(max-min)*double(range)/255.+double(strt)/255.; regular mapping
    double fract = double(v-min)/double(min-max)*double(range)/255.+double(strt+range)/255.;

    double start = 0.5;
    double rots = /*-1.5*/0;
    double hue = /*1.5*/1.0;
    double gamma = 1.0;

    /* variables */
    //double fract = 0.0;
    double angle = 0.0;
    double amp = 0.0;
    double red = 0.0;
    double grn = 0.0;
    double blu = 0.0;

    /* Ints for QColor construction */
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    /* Cubehelix algorithm */
    angle = 2.0*M_PI*(start/3.0 + 1.0 + rots*fract);
    fract = pow(fract, gamma);
    amp = hue*fract*(1.0 - fract)/2.0;
    red = fract + amp*(-0.14861*cos(angle)+1.78277*sin(angle));
    grn = fract + amp*(-0.29227*cos(angle)-0.90649*sin(angle));
    blu = fract + amp*(+1.97294*cos(angle));

    if (red < 0.0) {
        red = 0.0;
    } else if (red > 1.0) {
        red = 1.0;
    }

    if (grn < 0.0) {
        grn = 0.0;
    } else if (grn > 1.0) {
        grn = 1.0;
    }

    if (blu < 0.0) {
        blu = 0.0;
    } else if (blu > 1.0) {
        blu = 1.0;
    }

    r = (uint8_t)(red*255.0);
    g = (uint8_t)(grn*255.0);
    b = (uint8_t)(blu*255.0);

    return QColor(r, g, b);
}

QColor ColorMap::cubehelix(double fract) // n = 256, fract = x/n
{
    fract = 1 - fract;

    /* 'Default' cubehelix parameters */
    double start = 0.5;
    double rots = -1.5;
    double hue = 1.0;
    double gamma = /*1.0*/1.5;

    /* variables */
    //double fract = 0.0;
    double angle = 0.0;
    double amp = 0.0;
    double red = 0.0;
    double grn = 0.0;
    double blu = 0.0;

    /* Ints for QColor construction */
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    /* Cubehelix algorithm */
    //        for (int i = 0; i < n; i++) {
    //fract = ((double)i)/((double)n);
    angle = 2.0*M_PI*(start/3.0 + 1.0 + rots*fract);
    //fract = fract**gamma;
    fract = pow(fract, gamma);
    amp = hue*fract*(1.0 - fract)/2.0;
    red = fract + amp*(-0.14861*cos(angle)+1.78277*sin(angle));
    grn = fract + amp*(-0.29227*cos(angle)-0.90649*sin(angle));
    blu = fract + amp*(+1.97294*cos(angle));

    if (red < 0.0) {
        red = 0.0;
    } else if (red > 1.0) {
        red = 1.0;
    }

    if (grn < 0.0) {
        grn = 0.0;
    } else if (grn > 1.0) {
        grn = 1.0;
    }

    if (blu < 0.0) {
        blu = 0.0;
    } else if (blu > 1.0) {
        blu = 1.0;
    }

    r = (uint8_t)(red*255.0);
    g = (uint8_t)(grn*255.0);
    b = (uint8_t)(blu*255.0);

    //            addColorStop(fract, QColor(r, g, b));
    //        }
    return QColor(r, g, b);
}
