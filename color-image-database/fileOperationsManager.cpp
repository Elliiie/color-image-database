#include "fileOperationsManager.h"
#include <QFileDialog>
#include <iostream>
#include <ctime>
#include "imageutil.h"
#include <QApplication>

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
    QString filename= QFileDialog::getOpenFileName(parent, UIConstants().ADD_BUTTON_TITLE, "", "Images (*.png *.jpg, *jpeg)");

    if(filename.isEmpty()) { return ""; }

    QFile file(filename);
    return file.fileName();
}

Image FileOperationsManager::saveImage(QString fullName)
{
    std::vector<Color> colors = db->readColors();
    std::map<QString, Color> dominantColors;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    for(const auto &algorithm: DBConstants().ALGORITHMS)
    {
        dominantColors[algorithm.second] = ImageUtil::dominantColorFrom(fullName, colors, algorithm.first);
    }

    Image image = Image(fullName.toStdString(), dominantColors);
    db->createImage(image);

    QApplication::restoreOverrideCursor();

    return image;
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
