#ifndef COLOR_H
#define COLOR_H

#include "databaseobject.h"
#include <QString>
#include <QColor>

class Color: public DatabaseObject
{
public:
    /*!
     * \brief
     * \param QString Name of table
     * \return QString Query used to create the rows of the table
     */
    Color();

    /*!
     * \brief Create a table in the DB
     * \param int
     * \return QString
     */
    Color(int id, QString hex);

    /*!
     * \brief Get hex value
     * \return QString The hex value
     */
    QString getHex() const;

    /*!
     * \brief Turn color into QString representation for printing/debugging purposes
     * \return QString contains all of the color's members
     */
    QString toQString() const;

    /*!
     * \brief Turn color into Qt's QColor according to the hex value
     * \return QColor
     */
    QColor toQColor() const;
private:
    QString hex;
};

std::ostream& operator << (std::ostream &os, const Color &c);

#endif // COLOR_H
