#ifndef FILEOPERATIONSMANAGER_H
#define FILEOPERATIONSMANAGER_H

#include "databasemodule.h"


class FileOperationsManager
{
public:
    FileOperationsManager(DatabaseModule* db);
    FileOperationsManager();
    QString openFile(QWidget *parent) const;
    Color getDominantColor(QString fullName);
    Image saveImage(QString fullName);
    std::vector<Image> loadImages();
    std::vector<Image> loadImages(QString hex);
private:
    DatabaseModule* db;
};
#endif // FILEOPERATIONSMANAGER_H
