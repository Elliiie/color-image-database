#ifndef IMAGE_H
#define IMAGE_H

#include <filesystem>
#include "color.h"

class Image : public DatabaseObject
{
public:
    Image();
    Image(int id, std::filesystem::path path, Color dominantColor);
    Image(std::filesystem::path path, Color dominantColor);
    std::filesystem::path getPath() const;
    Color getDominantColor() const;
    void setId();
private:
    std::filesystem::path path;
    Color dominantColor;
};

std::ostream& operator << (std::ostream &os, const Image &c);

#endif // IMAGE_H
