#include "databasemodule.h"
#include "qdebug.h"
#include "Constants.cpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <iostream>
#include <string>

DatabaseModule::DatabaseModule() {
    this->db = QSqlDatabase::addDatabase("QPSQL", "colors");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    // Change to own password
    // Should we use an env var?
    db.setPassword("PASSWORD");
    bool ok = db.open();
    if (ok) {
        this->initialSetup();
    } else {
        qDebug() << db.lastError();
    }
}

void DatabaseModule::wipeDatabase() {
    QSqlQuery qry(this->db);
    if( !qry.exec(QString("DROP TABLE IF EXISTS images;")))
        qDebug() << qry.lastError();
    else
        qDebug() << "images table deleted!";

    if( !qry.exec(QString("DROP TABLE IF EXISTS colors;") ))
        qDebug() << qry.lastError();
    else
        qDebug() << "colors table deleted!";


    this->initialSetup();
}

void DatabaseModule::initialSetup() {
    qDebug() << "Setting up database connection...";
    QSqlQuery qry(this->db);
    if(!qry.exec(QString("CREATE TABLE IF NOT EXISTS colors (color_id SERIAL NOT NULL PRIMARY KEY, hex CHAR(7) NOT NULL) ")))
        qDebug() << qry.lastError();
    else
        qDebug() << "colors table created or already exists!";

    if(!qry.exec(QString("CREATE TABLE IF NOT EXISTS images (image_id SERIAL NOT NULL PRIMARY KEY, path VARCHAR(100), dominant_color_id INT, CONSTRAINT fk_dominant_color FOREIGN KEY(dominant_color_id) REFERENCES colors(color_id));")))
        qDebug() << qry.lastError();
    else
        qDebug() << "images table created or already exists!";

    // Insert default colors if they do not exist
    bool colorsTableIsEmpty = this->readColors().empty();
    if (colorsTableIsEmpty) {
        for (QString color : UIConstants().COLORS) {
            qry.prepare("INSERT INTO colors (hex) VALUES (:hex)");
            qry.bindValue(":hex", color);
            if (!qry.exec())
                   qDebug() << qry.lastError();
        }
    }

}

void DatabaseModule::createImage(Image image) {
    QSqlQuery qry(this->db);
    qry.prepare("INSERT INTO images (path, dominant_color_id) VALUES (:path, :dominant_color_id)");
    qry.bindValue(":path", QString::fromStdString(image.getPath().string()));
    qry.bindValue(":dominant_color_id", image.getDominantColor().getId());
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        image.setId();
    }
}

std::vector<Image> DatabaseModule::readImages() {
    std::vector<Image> images;
    QSqlQuery qry(this->db);
    qry.prepare("SELECT image_id, path, dominant_color_id, colors.hex FROM images JOIN colors ON dominant_color_id = colors.color_id");
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        while (qry.next()) {
            images.push_back(Image(qry.value(0).toInt(), std::filesystem::path(qry.value(1).toString().toStdString()), Color(qry.value(2).toInt(), qry.value(3).toString())));
        }
    }
    return images;
}

std::vector<Image> DatabaseModule::readImages(Color color) {
    std::vector<Image> images;
    QSqlQuery qry(this->db);
    qry.prepare("SELECT image_id, path, dominant_color_id, colors.hex FROM images JOIN colors ON dominant_color_id = colors.color_id WHERE dominant_color_id = :color_id");
    qry.bindValue(":color_id", color.getId());
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        while (qry.next()) {
            images.push_back(Image(qry.value(0).toInt(), std::filesystem::path(qry.value(1).toString().toStdString()), Color(qry.value(2).toInt(), qry.value(3).toString())));
        }
    }
    return images;
}

void DatabaseModule::deleteImage(Image image) {
    QSqlQuery qry(this->db);
    qry.prepare("DELETE FROM images WHERE image_id = :id");
    qry.bindValue(":id", image.getId());
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    }
}

std::vector<Color> DatabaseModule::readColors() {
    std::vector<Color> colors;
    QSqlQuery qry(this->db);
    qry.prepare("SELECT color_id, hex FROM colors");
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        while (qry.next()) {
            colors.push_back(Color(qry.value(0).toInt(), qry.value(1).toString()));
        }
    }
    return colors;
}

Color DatabaseModule::readColor(QString hex) {
    QSqlQuery qry(this->db);
    qry.prepare("SELECT color_id, hex FROM colors WHERE hex = :hex");
    qry.bindValue(":hex", hex);
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        if (qry.next()) {
            return Color(qry.value(0).toInt(),qry.value(1).toString());
        }
    }
    return Color();
}
