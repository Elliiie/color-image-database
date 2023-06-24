#include "databasemodule.h"
#include "qdebug.h"
#include "Constants.cpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <iostream>
#include <string>

DatabaseModule::DatabaseModule() {
    this->db = QSqlDatabase::addDatabase("QPSQL", DBConstants().DB_NAME);
    db.setHostName("127.0.0.1");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");

    if(const char* env_p = std::getenv(DBConstants().DB_PASS_ENV_VAR)) {
        db.setPassword(env_p);
    } else {
        qDebug() << "Please set an env variable called " << DBConstants().DB_PASS_ENV_VAR << " to your PostgreSQL password.";
    }

    bool ok = db.open();
    if (ok) {
        this->initialSetup();
    } else {
        qDebug() << db.lastError();
    }
}

void DatabaseModule::wipeDatabase() {
    QSqlQuery qry(this->db);
    if( !qry.exec(QString("DROP TABLE IF EXISTS images_colors;") ))
        qDebug() << qry.lastError();
    else
        qDebug() << "images_colors table deleted!";

    if( !qry.exec(QString("DROP TABLE IF EXISTS images;")))
        qDebug() << qry.lastError();
    else
        qDebug() << "images table deleted!";

    if( !qry.exec(QString("DROP TABLE IF EXISTS colors;") ))
        qDebug() << qry.lastError();
    else
        qDebug() << "colors table deleted!";

    if( !qry.exec(QString("DROP TABLE IF EXISTS algorithms;") ))
        qDebug() << qry.lastError();
    else
        qDebug() << "algorithms table deleted!";

    this->initialSetup();
}

void DatabaseModule::createTable(QString tableName, QString tableStructure) {
    QSqlQuery qry(this->db);
    QString creationQry  = QString("CREATE TABLE IF NOT EXISTS ").append(tableName).append(" (").append(tableStructure).append(");");
    if(!qry.exec(creationQry))
        qDebug() << qry.lastError();
    else
        qDebug() << tableName << "table created or already exists, proceeding...";
}

void DatabaseModule::initialSetup() {
    qDebug() << "Setting up database connection...";

    createTable(QString("colors"), QString("color_id SERIAL NOT NULL PRIMARY KEY, hex CHAR(7) NOT NULL"));
    createTable(QString("images"), QString("image_id SERIAL NOT NULL PRIMARY KEY, path VARCHAR(100)"));
    createTable(QString("algorithms"), QString("name VARCHAR(100) PRIMARY KEY"));
    createTable(QString("images_colors"), QString("image_id INTEGER REFERENCES images(image_id), color_id INTEGER REFERENCES colors(color_id), algorithm VARCHAR(100) REFERENCES algorithms(name), CONSTRAINT images_colors_pk PRIMARY KEY (image_id, algorithm)"));

    QSqlQuery qry(this->db);
    // Insert default algorithms
    bool algorithmsTableIsEmpty = this->readAlgorithms().empty();
    if (algorithmsTableIsEmpty) {
        for (QString algo : DBConstants().ALGORITHMS) {
            qry.prepare("INSERT INTO algorithms (name) VALUES (:name)");
            qry.bindValue(":name", algo);
            if (!qry.exec())
                   qDebug() << qry.lastError();
        }
    }

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

std::vector<QString> DatabaseModule::readAlgorithms() {
    std::vector<QString> algorithms;
    QSqlQuery qry(this->db);
    qry.prepare("SELECT name FROM algorithms");
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        while (qry.next()) {
            algorithms.push_back( qry.value(0).toString());
        }
    }
    return algorithms;
}

void DatabaseModule::createImage(Image& image) {
    QSqlQuery qry(this->db);
    qry.prepare("INSERT INTO images (path) VALUES (:path)");
    qry.bindValue(":path", QString::fromStdString(image.getPath().string()));
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        int imageId = qry.lastInsertId().toInt();
        for (const auto& algo_color : image.getDominantColors()) {
            qry.prepare("INSERT INTO images_colors (image_id, color_id, algorithm) VALUES (:image_id, :color_id, :algorithm)");
            qry.bindValue(":image_id", imageId);
            qry.bindValue(":color_id", algo_color.second.getId());
            qry.bindValue(":algorithm", algo_color.first);
            if (!qry.exec())
                qDebug() << qry.lastError();
        }
        image.setId(imageId);
    }
}

// TODO!
std::vector<Image> DatabaseModule::readImages() {
    std::vector<Image> images;
    QSqlQuery qry(this->db);
    qry.prepare("SELECT image_id, path FROM images");
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        while (qry.next()) {
            images.push_back(Image(std::filesystem::path(qry.value(1).toString().toStdString())));
            images.at(images.size() - 1).setId(qry.value(0).toInt());
        }
    }

    for (auto& image : images) {
        std::map<QString, Color> dominantColors;
        QSqlQuery qryColors(this->db);
        qryColors.prepare("SELECT image_id, i_c.color_id, algorithm, colors.hex FROM images_colors AS i_c JOIN colors ON i_c.color_id = colors.color_id WHERE image_id = :image_id");
        qryColors.bindValue(":image_id", image.getId());
        if (!qryColors.exec()) {
            qDebug() << qry.lastError();
        } else {
            while (qryColors.next()) {
                image.addDominantColor(qryColors.value(2).toString(), Color(qryColors.value(1).toInt(), qryColors.value(3).toString()));
            }
        }
    }

    return images;
}

std::vector<Image> DatabaseModule::readImages(Color color) {
    std::vector<Image> images;
    QSqlQuery qry(this->db);
    qry.prepare("SELECT image_ids.image_id, image_ids.path, colors.color_id, colors.hex, i_c.algorithm FROM (SELECT images.image_id, path FROM images JOIN images_colors ON images.image_id = images_colors.image_id WHERE images_colors.color_id = :color_id) AS image_ids INNER JOIN images_colors AS i_c ON image_ids.image_id = i_c.image_id INNER JOIN colors ON i_c.color_id = colors.color_id ORDER BY image_id;");
    qry.bindValue(":color_id", color.getId());
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    } else {
        int lastId = -1;
        Image image;
        while (qry.next()) {
            if (lastId != qry.value(0).toInt() || lastId == -1) {
                // Create new image and start adding dominant colors
                if (lastId != -1) {
                    images.push_back(image);
                }
                image = Image(std::filesystem::path(qry.value(1).toString().toStdString()));
                lastId = qry.value(0).toInt();
                image.setId(lastId);

            }
            // Add dominant color to image
            image.addDominantColor(qry.value(4).toString(), Color(qry.value(2).toInt(), qry.value(3).toString()));

        }
        if (lastId != -1) {
            // add last image to vector (if there were any images in the query)
            images.push_back(image);
        }
    }
    return images;
}

bool DatabaseModule::deleteImage(int id) {
    QSqlQuery qry(this->db);

    qry.prepare("DELETE FROM images_colors WHERE image_id = :id");
    qry.bindValue(":id", id);
    if (!qry.exec()) {
        qDebug() << qry.lastError();
    }

    qry.prepare("DELETE FROM images WHERE image_id = :id");
    qry.bindValue(":id", id);
    if (!qry.exec()) {
        qDebug() << qry.lastError();
        return false;
    } else {
        return true;
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
