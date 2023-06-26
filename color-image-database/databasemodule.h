 #ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include <vector>
#include "color.h"
#include "image.h"
#include "qsqldatabase.h"
#include <QFileInfo>

class DatabaseModule
{
public:
    /*!
     * \brief Constructor which creates the tables of the DB if it does not exist
    */
    DatabaseModule();

    /*!
     * \brief Delete all tables
     */
    void wipeDatabase();

    /*!
     * \brief Read all algorithms in the DB
     * \return vector of QStrings, the names of the algorithms
     */
    std::vector<QString> readAlgorithms();

    /*!
     * \brief Insert an image in the DB
     * \param Image& image object whose members will be used to create the image record, and if successful, will be set as persisted
     */
    void createImage(Image& image);

    /*!
     * \brief Read all images in the DB
     * \return vector of Images
     */
    std::vector<Image> readImages();

    /*!
     * \brief Read images which have a dominant color
     * \param Color dominant color to search by
     * \return vector of Images - the resulting objects of the query
     */
    std::vector<Image> readImages(Color color);

    /*!
     * \brief Delete image in DB by image id
     * \param int Image's id
     * \return bool Whether the deletion was successful
     */
    bool deleteImage(int id);

    /*!
     * \brief Read all colors in the DB
     * \return vector of Colors
     */
    std::vector<Color> readColors();

    /*!
     * \brief Read a color by its hex value
     * \param QString Hex value
     * \return Colo
     */
    Color readColor(QString hex);
private:
    /*!
     * \brief Create a table in the DB
     * \param QString Name of table
     * \return QString Query used to create the rows of the table
     */
    void createTable(QString tableName, QString tableStructure);

    /*!
     * \brief Create all tables in the DB and fills them with the default values
     */
    void initialSetup();
    QSqlDatabase db;
};

#endif // DATABASEMODULE_H
