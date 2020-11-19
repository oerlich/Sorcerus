
#include "Interpolater.h"

float Interpolator::linearInterp(float begin, float end, float dur, float t)
{
    float res = begin * (1 - (t / dur)) + end * (t / dur);
    return (res > end) ? end : res;
}
