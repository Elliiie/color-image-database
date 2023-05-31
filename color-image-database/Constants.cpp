#include "QtCore/qsize.h"

struct UIConstants
{
    const int IMAGE_WIDTH = 150;
    const int IMAGE_HEIGHT = 150;

    const QSize COLOR_BUTTON_SIZE = QSize(30, 30);
    const QSize ADD_BUTTON_SIZE = QSize(200, 30);

    const QString ADD_BUTTON_TITLE = "Load new image";

    const QString COLORS[16] = {"#ff0000", "#ff5500", "#ffaa00", "#ffff00", "#aaff00", "#55ff00",
                               "#00ffaa", "#00ffff", "#00aaff", "#0055ff", "#0000ff", "#5500ff",
                               "#aa00ff", "#ff00ff", "#ff00aa", "#ff0055"};
};
