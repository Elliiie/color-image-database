#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H


#include "color.h"
#include "qcolor.h"
#include "qimage.h"

class ImageUtil
{
public:
    static Color dominantColorFrom(QString imagePath, const std::vector<Color>& colors);

private:
    static QColor dominantColor(const QImage& image); // Histogram approach
};


#endif // IMAGEUTIL_H
