#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H


#include "color.h"
#include "qcolor.h"

class ColorConverter
{
public:
    static QColor hexToColor(Color color);
};

#endif // COLORCONVERTER_H
