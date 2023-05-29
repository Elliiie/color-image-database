#include "fileOperationsManager.h"
#include <QFileDialog>
#include <iostream>
#include <ctime>
#include "constants.cpp"
#include "imageutil.h"


FileOperationsManager::FileOperationsManager()
{
    this->db = nullptr;
}

FileOperationsManager::FileOperationsManager(DatabaseModule* db)
{
    this->db = db;
}

QString FileOperationsManager::openFile(QWidget *parent) const
{
    QString filename= QFileDialog::getOpenFileName(parent, UIConstants().ADD_BUTTON_TITLE, "", "Images (*.png *.jpg)");

    if(filename.isEmpty()) { return ""; }

    QFile file(filename);
    return file.fileName();
}

void FileOperationsManager::saveImage(QString fullName)
{
    if(fullName.isEmpty()) { return; }

    std::vector<Color> colors = db->readColors();
    Color dominantColor = ImageUtil::dominantColorFrom(fullName, colors);

    int id = db->readImages().size() + 1;
    Image image = Image(id, fullName.toStdString(), dominantColor);
    db->createImage(image);
}

std::vector<Image> FileOperationsManager::loadImages()
{
    return db->readImages();
}

std::vector<Image> FileOperationsManager::loadImages(QString hex)
{
    Color color = db->readColor(hex);
    return db->readImages(color);
}
