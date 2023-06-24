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

Color FileOperationsManager::getDominantColor(QString fullName)
{
    std::vector<Color> colors = db->readColors();
    return ImageUtil::dominantColorFrom(fullName, colors);
}
