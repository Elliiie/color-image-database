#ifndef FILEOPERATIONSMANAGER_H
#define FILEOPERATIONSMANAGER_H

#include "databasemodule.h"


class FileOperationsManager
{
public:
    /*!
     * \brief Constructor - creates file FileOperationsManager object with specified reference to a database module
     * \param db DatabaseModule reference
    */
    FileOperationsManager(DatabaseModule* db);
    /*!
     * Default constructor
    */
    FileOperationsManager();
    /*!
     * \brief Opens a dialog that lets the user to choose an image from the file system
     * \param parent QWidget object that will display the dialog
     * \return string The full path of the image
    */
    QString openFile(QWidget *parent) const;
    /*!
     * \brief Runs the algorithm for dominant color extraction and svaes the image in the database
     * \param fullName The full path of an image
     * \return Image object representing the saved image in the database
    */
    Image saveImage(QString fullName);
    /*!
     * \brief Loads all images saved in the database
     * \return vector of Image objects
    */
    std::vector<Image> loadImages();
    /*!
     * \brief Loads all images with specified dominant color
     * \param QString hex of a color
     * \return vector of Image objects
    */
    std::vector<Image> loadImages(QString hex);
private:
    DatabaseModule* db;
};
#endif // FILEOPERATIONSMANAGER_H
