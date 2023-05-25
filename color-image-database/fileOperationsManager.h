#ifndef FILEOPERATIONSMANAGER_H
#define FILEOPERATIONSMANAGER_H

#include "QtCore/qstring.h"
#include "databasemodule.h"


class FileOperationsManager
{
public:
    FileOperationsManager(DatabaseModule* db);
    FileOperationsManager();
    QString openFile(QWidget *parent) const;
    void saveImage(QString fullName);
    std::vector<Image> loadImages();
    std::vector<Image> loadImages(QString hex);

private:
    DatabaseModule* db;
};
#endif // FILEOPERATIONSMANAGER_H
