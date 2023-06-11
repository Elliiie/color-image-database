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

    this->fileOperationsManager = FileOperationsManager(&db);
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

        colorsMapper.setMapping(button, color.getHex());
        connect(button, SIGNAL(clicked()), &colorsMapper, SLOT(map()));

        colors->addWidget(button);
    }

    connect(&colorsMapper, SIGNAL(mappedString(QString)), this, SLOT(on_colorTapped(QString)));
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
    for(Image image: db.readImages()) {
        showImage(image);
    }
}

void MainWindow::showImagesWithDominantColor(QString hex)
{
     Color color = db.readColor(hex);
    for(Image image: db.readImages(color)) {
        showImage(image);
    }
}

void MainWindow::showImage(Image image)
{
    std::string path = image.getPath().string();

    if(path.empty()) { return; }

    QWidget* deleteButtonWidget = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(deleteButtonWidget);
    deleteButtonWidget->setLayout(layout);

    QPixmap pic(QString::fromStdString(path));
    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(pic.scaled(UIConstants().IMAGE_WIDTH, UIConstants().IMAGE_HEIGHT));

    QPushButton *deleteImageButton = new QPushButton(UIConstants().DELETE_BUTTON_TITLE);

    imagesActionMapper.setMapping(deleteImageButton, image.getId());
    connect(deleteImageButton, SIGNAL(clicked()), &imagesActionMapper, SLOT(map()));
    connect(&imagesActionMapper, SIGNAL(mappedInt(int)), this, SLOT(on_deleteImageTapped(int)));

    layout->addWidget(imageLabel);
    layout->addWidget(deleteImageButton);

    flowLayout->addWidget(deleteButtonWidget);
}

void MainWindow::on_openImageTapped()
{
    QString fileName = this->fileOperationsManager.openFile(this);

    Image image = Image(fileName.toStdString(), fileOperationsManager.getDominantColor(fileName));
    db.createImage(image);

    showImage(image);
}

void MainWindow::on_deleteImageTapped(int id) {
    //TODO: Get Image by id for DB and delete
    this->db.deleteImage(id);
    flowLayout->clearLayout();
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
