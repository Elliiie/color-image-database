#include "color.h"
#include "image.h"

Color::Color() {
    this->valid = false;
}

Color::Color(int id, QString hex)
{
    this->id = id;
    this->hex = hex;
    this->valid = true;
}

QString Color::getHex() const {
    return this->hex;
}

QString Color::toQString() const {
    std::string str = "Id:" + std::to_string(getId()) + "\n Hex: " + getHex().toStdString() + "\n-----";
    return QString::fromStdString(str);
}

std::ostream& operator << (std::ostream &os, const Color &c) {
    return (os << "-----" << "Id: " << c.getId() << "\n Hex: " << c.getHex().toStdString() << "\n-----" << std::endl);
}
