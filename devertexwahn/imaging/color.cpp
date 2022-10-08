// Code copied form paulbourke 
// Modified by julianamann

#include "imaging/color.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

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
Color3f hot_to_cold_color_ramp(float v, float vmin, float vmax) {
    Color3f c = {1.f, 1.f, 1.f}; // white

    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;
    double dv = vmax - vmin;

    if (v < (vmin + 0.25f * dv)) {
        c.red() = 0.f;
        c.green() = 4.f * (v - vmin) / dv;
    } else if (v < (vmin + 0.5f * dv)) {
        c.red() = 0;
        c.blue() = 1.f + 4.f * (vmin + 0.25f * dv - v) / dv;
    } else if (v < (vmin + 0.75f * dv)) {
        c.red() = 4.f * (v - vmin - 0.5f * dv) / dv;
        c.blue() = 0.f;
    } else {
        c.green() = 1.f + 4.f * (vmin + 0.75f * dv - v) / dv;
        c.blue() = 0;
    }

    return c;
}

DE_VERTEXWAHN_END_NAMESPACE