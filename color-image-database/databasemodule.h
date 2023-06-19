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
    DatabaseModule();
    void createTable(QString tableName, QString tableStructure);
    void wipeDatabase();
    std::vector<QString> readAlgorithms();
    void createImage(Image image);
    std::vector<Image> readImages();
    std::vector<Image> readImages(Color color);
    void deleteImage(int id);
    std::vector<Color> readColors();
    Color readColor(QString hex);
private:
    void initialSetup();
    QSqlDatabase db;
};

#endif // DATABASEMODULE_H
