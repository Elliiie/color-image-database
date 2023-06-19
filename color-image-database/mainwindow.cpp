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

#include <chrono>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->flowLayout = new FlowLayout();
    this->buttons = new QHBoxLayout();

    this->fileOperationsManager = FileOperationsManager(&db);
//    this->testDb();
//    this->db.wipeDatabase();
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

    connect(&mapper, SIGNAL(mappedString(QString)), this, SLOT(on_colorTapped(QString)));
    buttons->addItem(colors);
}

void MainWindow::setupLoadImageButton()
{
    QPushButton *add = new QPushButton(UIConstants().ADD_BUTTON_TITLE);
    add->setMaximumSize(UIConstants().ADD_BUTTON_SIZE);
    connect(add, SIGNAL(clicked()), this, SLOT(on_openImageTapped()));
    buttons->addWidget(add);
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

void MainWindow::on_openImageTapped()
{
    QString fileName = this->fileOperationsManager.openFile(this);
    fileOperationsManager.saveImage(fileName);

    showImage(fileName.toStdString());
}

void MainWindow::on_deleteImageTapped(int id) {
    //TODO: Get Image by id for DB and delete
    this->db.deleteImage(id);
    auto start = std::chrono::high_resolution_clock::now();
    flowLayout->clearLayout();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << " microseconds" << std::endl;
    this->showSavedImages();
}

void MainWindow::on_colorTapped(QString hex)
{
    flowLayout->clearLayout();
    showImagesWithDominantColor(hex);
}

// ---------------------- DEBUG CODE ------------------------------

void MainWindow::testDb() {
    this->db.wipeDatabase();
    std::vector<Color> colors = this->db.readColors();
    assert(colors.size() == sizeof(UIConstants().COLORS) / sizeof(UIConstants().COLORS[0]));
    for (auto color : colors) {
        std::cout << color;
    }

    Color black = this->db.readColor(colors[0].getHex());
    std::cout << "First color:\n " << black << std::endl;

    std::map<QString, Color> m1 {{DBConstants().ALGORITHMS[0], colors[0]}};
    std::filesystem::path imagePath1 { "C:\\Users\\ilian\\Desktop\\image1.jpg" };
    Image image1 = Image(imagePath1, m1);
    m1[DBConstants().ALGORITHMS[1]] = colors[5];
    std::filesystem::path imagePath2 { "C:\\Users\\ilian\\Desktop\\image2.jpg" };
    Image image2 = Image(imagePath2, m1);
    Image image3 = Image(imagePath2, m1);
    this->db.createImage(image1);
    this->db.createImage(image2);
    this->db.createImage(image3);

    std::vector<Image> imagesFromDb = this->db.readImages(colors[5]);
    assert(imagesFromDb.size() == 2);
    for (const auto& image : imagesFromDb) {
        std::cout << image;
    }

    std::vector<Image> allImagesFromDb = this->db.readImages();
    std::cout << "All images\n";
    assert(allImagesFromDb.size() == 3);
    for (auto image : allImagesFromDb) {
        std::cout << image;
    }
    std::cout << "After deletion:" << std::endl;
    this->db.deleteImage(allImagesFromDb[0].getId());
    allImagesFromDb = this->db.readImages();
    assert(allImagesFromDb.size() == 2);
    for (auto image : imagesFromDb) {
        std::cout << image;
    }
}
