#include "imageutil.h"
#include "constants.cpp"

QColor ImageUtil::dominantColor(const QImage& image)
{
    std::vector<int> histogram(UIConstants().HUE_HISTOGRAM_SIZE, 0);

    // Loop through the rows and count hue values.
    for (int y = 0; y < image.height(); ++y) {
        const QRgb* row = reinterpret_cast<const QRgb*>(image.constScanLine(y));
        const QRgb* end = row + image.width();

        while (row < end) {
            QColor pixelColor = QColor::fromRgb(*row++);
            QColor hsvColor = pixelColor.toHsv();
            int hue = hsvColor.hue();

            if (hue == -1) {
                hue = 360; // Replace -1 with 360. (achromatic color)
            }

            histogram[hue]++;
        }
    }

    // Find hue value with the highest count.
    int dominantHue = std::distance(histogram.begin(), std::max_element(histogram.begin(), histogram.end()));

    if (dominantHue == 360) {
        dominantHue = -1; // Switch back to -1, in case the dominant hue is achromatic.
    }

    return QColor::fromHsv(dominantHue, UIConstants().HSV_SATURATION, UIConstants().HSV_VALUE);
}

Color ImageUtil::dominantColorFrom(QString imagePath, const std::vector<Color>& colors) {
    QImage image(imagePath);

    QColor dominantColor = ImageUtil::dominantColor(image);

    Color closestColor;
    int closestHue = 360;

    for(Color const& color : colors) {
        QColor currentColor = color.toQColor().toHsv();

        int hueDifference = abs(dominantColor.hue() - currentColor.hue());
        if (hueDifference < closestHue) {
            closestHue = hueDifference;
            closestColor = color;
        }
    }

    return closestColor;
}
