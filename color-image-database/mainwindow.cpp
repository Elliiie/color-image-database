#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "constants.cpp"
#include <iostream>
#include <cassert>
#include <QPixmap>
#include "QtWidgets/qscrollarea.h"
#include "flowLayout.h"
#include <QPushButton>
#include "QtWidgets/qlabel.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->db = DatabaseModule();
//    this->testDb();
    loadImages();

    setupButtons();

    setWindowTitle(tr("Images database"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testDb() {
    this->db.wipeDatabase();
    std::vector<Color> colors = this->db.readColors();
    assert(colors.size() > 0);
    for (auto color : colors) {
        std::cout << color;
    }

    Color black = this->db.readColor("000000");
    std::cout << "Black: " << black << std::endl;

    std::filesystem::path imagePath1 { "C:\\Users\\ilian\\Desktop\\image1.jpg" };
    Image image1 = Image(imagePath1, colors[0]);
    std::filesystem::path imagePath2 { "C:\\Users\\ilian\\Desktop\\image2.jpg" };
    Image image2 = Image(imagePath2, colors[0]);
    Image image3 = Image(imagePath2, colors[1]);
    this->db.createImage(image1);
    this->db.createImage(image2);
    this->db.createImage(image3);

    std::vector<Image> imagesFromDb = this->db.readImages(colors[0]);
    assert(imagesFromDb.size() == 2);
    for (auto image : imagesFromDb) {
        std::cout << image;
    }

    std::vector<Image> allImagesFromDb = this->db.readImages();
    assert(allImagesFromDb.size() == 3);
    for (auto image : allImagesFromDb) {
        std::cout << image;
    }
    std::cout << "After deletion:" << std::endl;
    this->db.deleteImage(allImagesFromDb[0]);
    allImagesFromDb = this->db.readImages();
    assert(allImagesFromDb.size() == 2);
    for (auto image : imagesFromDb) {
        std::cout << image;
    }
}

void MainWindow::loadImages() {
    FlowLayout *flowLayout = new FlowLayout;

    QPixmap pic(".../malo.png");

    for(int i=0; i<20; i++) {
        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(pic.scaled(UIConstants().IMAGE_WIDTH, UIConstants().IMAGE_HEIGHT));

        flowLayout->addWidget(imageLabel);
    }

    QWidget *widget = new QWidget();
    widget->setLayout(flowLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    ui->verticalLayout->addWidget(scrollArea);
}



void MainWindow::setupButtons() {
    QHBoxLayout* buttons = new QHBoxLayout();

    QHBoxLayout* colors = new QHBoxLayout();

    for(int i=0; i<10; i++) {
        QPushButton *button = new QPushButton();
        button->setFixedSize(UIConstants().COLOR_BUTTON_SIZE);
        QColor col = QColor(Qt::red);
        QString qss = QString("background-color: %1").arg(col.name());
        button->setStyleSheet(qss);
        colors->addWidget(button);
    }

    buttons->addItem(colors);

    QPushButton *add = new QPushButton(UIConstants().ADD_BUTTON_TITLE);
    add->setMaximumSize(UIConstants().ADD_BUTTON_SIZE);
    buttons->addWidget(add);

    QWidget *widget = new QWidget();
    widget->setLayout(buttons);
    ui->verticalLayout->addWidget(widget);
}
