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

private:
    DatabaseModule* db;
};
#endif // FILEOPERATIONSMANAGER_H
