#include "image.h"

Image::Image()
{
    this->id = -1;
    this->persisted = false;
}

Image::Image(std::filesystem::path path) {
    this->id = -1;
    this->path = path;
    this->persisted = false;
}

Image::Image(std::filesystem::path path, std::map<QString, Color> dominantColors) {
    this->id = -1;
    this->path = path;
    this->dominantColors = dominantColors;
    this->persisted = false;
}

std::filesystem::path Image::getPath() const {
    return this->path;
}

const std::map<QString, Color>& Image::getDominantColors() const {
    return this->dominantColors;
}

void Image::setId(int id) {
    this->id = id;
}

void Image::setPath(std::filesystem::path path) {
    this->path = path;
    this->persisted = false;
}

void Image::addDominantColor(QString algo, Color col) {
    dominantColors[algo] = col;
    this->persisted = false;
}

QString Image::dominantColorsToString() const
{
    QString out = "";
    for (const auto& n : getDominantColors())
        out = out.append(n.first).append(" = ").append(n.second.toQString()).append(" ");

   return out;
}

std::ostream& operator << (std::ostream &os, const Image &i) {
    return (os << "-----" << "Image:\n Id: " << i.getId() << "\n Path: " << i.getPath().string() << "\nImage's dominant colors:\n" << i.dominantColorsToString().toStdString() << "-----" << std::endl);
}
