#include "colorconverter.h"

QColor ColorConverter::hexToColor(Color hex) {
    QColor color;

    return color.fromString(hex.getHex());
}
