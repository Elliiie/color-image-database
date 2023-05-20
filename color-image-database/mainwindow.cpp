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
    this->ui->setupUi(this);
    this->flowLayout = new FlowLayout();
    this->buttons = new QHBoxLayout();

    this->db = DatabaseModule();
    this->fileOperationsManager = FileOperationsManager(db);
//    this->testDb();

    this->setupMainLayout();

    this->showSavedImages();

    setWindowTitle(tr("Images database"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMainLayout()
{
    QWidget *flowLayouotWidget = new QWidget();
    flowLayouotWidget->setLayout(flowLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(flowLayouotWidget);

    ui->verticalLayout->addWidget(scrollArea);

    QWidget *buttonsWidget = new QWidget();

    this->setupColorButtons();
    this->setupLoadImageButton();

    buttonsWidget->setLayout(buttons);
    ui->verticalLayout->addWidget(buttonsWidget);
}

void MainWindow::setupColorButtons()
{
    QHBoxLayout* colors = new QHBoxLayout();

    std::vector<Color> savedColors = db.readColors();

    for(Color color: savedColors) {
        QPushButton *button = new QPushButton();
        button->setFixedSize(UIConstants().COLOR_BUTTON_SIZE);
        QString buttonColor = QString("background-color: %1").arg(color.getHex());
        button->setStyleSheet(buttonColor);

        mapper.setMapping(button, color.getHex());
        connect(button, SIGNAL(clicked()), &mapper, SLOT(map()));

        colors->addWidget(button);
    }

    connect(&mapper, SIGNAL(mappedString(QString)), this, SLOT(on_color_tapped(QString)));
    buttons->addItem(colors);
}

void MainWindow::setupLoadImageButton()
{
    QPushButton *add = new QPushButton(UIConstants().ADD_BUTTON_TITLE);
    add->setMaximumSize(UIConstants().ADD_BUTTON_SIZE);
    connect(add, SIGNAL(clicked()), this, SLOT(on_open_image_tapped()));
    buttons->addWidget(add);
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

void MainWindow::showSavedImages()
{
    for(Image image: fileOperationsManager.loadImages()) {
        showImage(image.getPath().u8string());
    }
}

void MainWindow::showImagesWithDominantColor(QString hex)
{
    for(Image image: fileOperationsManager.loadImages(hex)) {
        showImage(image.getPath().u8string());
    }
}

void MainWindow::showImage(std::string name)
{
    if(name.empty()) { return; }

    QPixmap pic(QString::fromStdString(name));
    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(pic.scaled(UIConstants().IMAGE_WIDTH, UIConstants().IMAGE_HEIGHT));

    flowLayout->addWidget(imageLabel);
}

void MainWindow::on_open_image_tapped()
{
    QString fileName = this->fileOperationsManager.openFile(this);
    showImage(fileName.toStdString());
    fileOperationsManager.saveImage(fileName);
}

void MainWindow::on_color_tapped(QString hex)
{
    flowLayout->clearLayout();
    showImagesWithDominantColor(hex);
}
