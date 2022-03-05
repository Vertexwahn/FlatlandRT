// Code copied form paulbourke 
// Modified by julianamann

#include "flatland/imaging/color.h"

FLATLAND_BEGIN_NAMESPACE

// http://paulbourke.net/miscellaneous/colourspace/
/*
   Return a RGB colour value given a scalar v in the range [vmin,vmax]
   In this case each colour component ranges from 0 (no contribution) to
   1 (fully saturated), modifications for other ranges is trivial.
   The colour is clipped at the end of the scales if v is outside
   the range [vmin,vmax]
   typedef struct {
      double r,g,b;
   } COLOUR;
*/
Color3f hot_to_cold_color_ramp(double v, double vmin, double vmax) {
    Color3f c = {1.0, 1.0, 1.0}; // white
    double dv;

    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;
    dv = vmax - vmin;

    if (v < (vmin + 0.25 * dv)) {
        c.red() = 0;
        c.green() = 4 * (v - vmin) / dv;
    } else if (v < (vmin + 0.5 * dv)) {
        c.red() = 0;
        c.blue() = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
    } else if (v < (vmin + 0.75 * dv)) {
        c.red() = 4 * (v - vmin - 0.5 * dv) / dv;
        c.blue() = 0;
    } else {
        c.green() = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
        c.blue() = 0;
    }

    return c;
}

FLATLAND_END_NAMESPACE