#ifndef IMAGE_H
#define IMAGE_H

#include <filesystem>
#include <map>
#include "color.h"

class Image : public DatabaseObject
{
public:
    Image();
    Image(int id, std::filesystem::path path, std::map<QString, Color> dominantColors);
    Image(std::filesystem::path path);
    Image(std::filesystem::path path, std::map<QString, Color> dominantColors);
    std::filesystem::path getPath() const;
    const std::map<QString, Color>& getDominantColors() const;
    void setId(int id);
    void setPath(std::filesystem::path path);
    void addDominantColor(QString algo, Color col);
    QString dominantColorsToString() const;
private:
    std::filesystem::path path;
    Color dominantColor;
    std::map<QString, Color> dominantColors;
};

std::ostream& operator << (std::ostream &os, const Image &c);

#endif // IMAGE_H
