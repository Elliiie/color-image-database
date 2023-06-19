#ifndef COLOR_H
#define COLOR_H

#include "databaseobject.h"
#include <QString>

class Color: public DatabaseObject
{
public:
    Color();
    Color(int id, QString hex);
    QString getHex() const;
    QString toQString() const;
private:
    QString hex;
};

std::ostream& operator << (std::ostream &os, const Color &c);

#endif // COLOR_H
