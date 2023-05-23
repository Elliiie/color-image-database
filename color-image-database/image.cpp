#include "image.h"

Image::Image()
{
    this->valid = false;
}

Image::Image(int id, std::filesystem::path path, Color dominantColor) {
    this->id = id;
    this->path = path;
    this->dominantColor = dominantColor;
    this->valid = true;
}

Image::Image(std::filesystem::path path, Color dominantColor) {
    this->id = -1;
    this->path = path;
    this->dominantColor = dominantColor;
    this->valid = false;
}

std::filesystem::path Image::getPath() const {
    return this->path;
}

Color Image::getDominantColor() const {
    return this->dominantColor;
}

void Image::setId() {
    this->id = id;
    this->valid = true;
}

std::ostream& operator << (std::ostream &os, const Image &i) {
    return (os << "-----" << "Image:\n Id: " << i.getId() << "\n Path: " << i.getPath().string() << "\nImage's dominant color:\n" << i.getDominantColor() << "-----" << std::endl);
}
