#ifndef IMAGE_H
#define IMAGE_H

#include <filesystem>
#include <map>
#include "color.h"

class Image : public DatabaseObject
{
public:
    /*!
     * \brief Constructor for image which is not persisted to db and has id of -1
     */
    Image();

    /*!
     * \brief Constructor for image which is not persisted to db, has id of -1 and a path
     * \param filesystem::path Path to the image
     */
    Image(std::filesystem::path path);

    /*!
     * \brief Constructor for image which is not persisted to db, has id of -1, a path and dominant colors
     * \param filesystem::path Path to the image
     * \param map of QStrings to Colors Dominant color of image according to each algorithm
     */
    Image(std::filesystem::path path, std::map<QString, Color> dominantColors);

    /*!
     * \brief Get the image's path
     * \return filesystem::path Path to the image
     */
    std::filesystem::path getPath() const;

    /*!
     * \brief Get the dominant colors
     * \return map of QStrings to Colors Maps each algorithm to a dominant color
     */
    const std::map<QString, Color>& getDominantColors() const;

    /*!
     * \brief Sets id (assumes the id is the same as in the db)
     * \param int id
     */
    void setId(int id);

    /*!
     * \brief Set the image's path and set persisted to false (since the object does not reflect the db anymore
     * \param filesystem::path Path to the image
     */
    void setPath(std::filesystem::path path);

    /*!
     * \brief Add a new dominant color to the dominant colors map
     * \param QString algorithm name
     * \param Color dominant color
     */
    void addDominantColor(QString algo, Color col);

    /*!
     * \brief Extract all dominant colors to QString for logging purposes
     * \return QString The dominant colors and algorithm names
     */
    QString dominantColorsToString() const;
private:
    std::filesystem::path path;
    Color dominantColor;
    std::map<QString, Color> dominantColors;
};

std::ostream& operator << (std::ostream &os, const Image &c);

#endif // IMAGE_H
