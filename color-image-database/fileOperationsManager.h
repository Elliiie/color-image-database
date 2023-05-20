#ifndef FILEOPERATIONSMANAGER_H
#define FILEOPERATIONSMANAGER_H

#include "QtCore/qstring.h"
#include "databasemodule.h"

static int ImagesCount = 0;

class FileOperationsManager
{
public:
    FileOperationsManager();
    FileOperationsManager(DatabaseModule db);

    QString openFile(QWidget *parent) const;
    void saveImage(QString fullName);
    std::vector<Image> loadImages();
    std::vector<Image> loadImages(QString hex);

private:
    DatabaseModule db;
};
#endif // FILEOPERATIONSMANAGER_H
