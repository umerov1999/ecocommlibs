#ifndef PARSEINK_H
#define PARSEINK_H

#include <stdlib.h>
#include <stdio.h>

typedef int (*parseInk_addStroke_t)(
    const float* x, size_t xByteStride,
    const float* y, size_t yByteStride,
    int pointCount,
    void* userParam);

int parseInk(FILE* file, parseInk_addStroke_t addStroke, void* userParam);

#endif // end of: #ifndef PARSEINK_H
