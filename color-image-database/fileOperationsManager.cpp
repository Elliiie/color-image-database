#include "fileOperationsManager.h"
#include <QFileDialog>
#include <iostream>
#include <ctime>
#include "constants.cpp"


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

    // TODO: Run color detection algorithm to determine the dominant one and save the image with it
    Image image = Image(fullName.toStdString(), db->readColors().at(0));
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
