
#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace sparky { namespace maths {

    inline  float toRadiants(float degrees)
    {
        return degrees * ((float) M_PI / 180.0f);
    }

} }
