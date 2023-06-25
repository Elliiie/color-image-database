#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtWidgets/qcombobox.h"
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
    this->savedColors = db.readColors();

    this->setupMainLayout();

    setWindowTitle(tr("Images database"));
}

MainWindow::~MainWindow()
{
    flowLayout->clearLayout();
    delete buttons;
    delete picker;
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
    this->setupAlgorithmPicker();

    buttonsWidget->setLayout(buttons);
    ui->verticalLayout->addWidget(buttonsWidget);
}

void MainWindow::setupColorButtons()
{
    QHBoxLayout* colors = new QHBoxLayout();

    this->colorButtons = std::vector<QPushButton>(savedColors.size());

    for(int i = 0; i < savedColors.size(); i++) {
        QPushButton *button = &colorButtons.at(i);
        const Color& color = savedColors.at(i);
        button->setFixedSize(UIConstants().COLOR_BUTTON_SIZE);
        unhighlightImageWithIndex(i);

        connect(button,  &QPushButton::clicked, [this, color, i]() {
            unhighlightSelectedIndexIfNeeded();
            selectedButtonIndex = i;
            highlightImageWithIndex(i);
            on_colorTapped(color.getHex());
        });

        colors->addWidget(button);
    }

    QPushButton *button = new QPushButton("ALL");
    button->setFixedSize(UIConstants().ALL_IMAGES_BUTTON_SIZE);
    connect(button,  &QPushButton::clicked, [this]() {
        unhighlightSelectedIndexIfNeeded();
        selectedButtonIndex = -1;
        flowLayout->clearLayout();
        showSavedImages();
    });
    colors->addWidget(button);

    buttons->addItem(colors);
}

void MainWindow::unhighlightImageWithIndex(int index)
{
    QString buttonColor = QString("background-color: %1").arg(savedColors.at(index).getHex());
    QPushButton *button = &colorButtons.at(index);
    button->setStyleSheet(buttonColor);
}

void MainWindow::highlightImageWithIndex(int index)
{
    QString buttonColor = QString("background-color: %1; border-style: outset; border-width: 2px; border-color: beige;").arg(savedColors.at(index).getHex());
    QPushButton *button = &colorButtons.at(index);
    button->setStyleSheet(buttonColor);
}

void MainWindow::unhighlightSelectedIndexIfNeeded()
{
    if (selectedButtonIndex > -1) {
        unhighlightImageWithIndex(selectedButtonIndex);
    }
}

void MainWindow::setupLoadImageButton()
{
    QPushButton *add = new QPushButton(UIConstants().ADD_BUTTON_TITLE);
    add->setMaximumSize(UIConstants().ADD_BUTTON_SIZE);
    connect(add, SIGNAL(clicked()), this, SLOT(on_openImageTapped()));
    buttons->addWidget(add);
}

void MainWindow::setupAlgorithmPicker()
{
    picker = new QComboBox();
    picker->setFixedSize(UIConstants().ADD_BUTTON_SIZE);
    connect(picker, &QComboBox::currentTextChanged, [this]() {
            this->flowLayout->clearLayout();
            this->showSavedImages();
        });

    for(const auto &algorithm: DBConstants().ALGORITHMS)
    {
        picker->addItem(algorithm.second);
    }
    buttons->addWidget(picker);
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

    // Widget for the image and its actions and inforamtion
    QWidget *imageWidget = new QWidget();
    QVBoxLayout *elementLayout = new QVBoxLayout();
    imageWidget->setLayout(elementLayout);

    // Label representing the image; Added directrly to the main widget
    QPixmap pic(QString::fromStdString(path));
    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(pic.scaled(UIConstants().IMAGE_WIDTH, UIConstants().IMAGE_HEIGHT));

    // Image actions and inforamtion widget
    QWidget *buttonActionsAndInfoWidget = new QWidget();
    QHBoxLayout *buttonActionsAndInfoLayout = new QHBoxLayout();
    buttonActionsAndInfoWidget->setLayout(buttonActionsAndInfoLayout);

    // Delete image button; Added to the widget holding image's actions and information
    QPushButton *deleteImageButton = new QPushButton(UIConstants().DELETE_BUTTON_TITLE);
    connect(deleteImageButton, &QPushButton::clicked, [this, imageWidget, image]() {
        // Delete image from UI
        flowLayout->removeWidget(imageWidget);
        delete imageWidget;
        // Delete image from DB
        db.deleteImage(image.getId());
    });
    buttonActionsAndInfoLayout->addWidget(deleteImageButton);

    // Label representing image's dominant color; Added to the widget holding image's actions and information
    QLabel *imageColorLabel = new QLabel;
    QString chosenAlgorithm = picker->currentText();
    QString imageDominantColor = QString("background-color: %1").arg(image.getDominantColors().at(chosenAlgorithm).getHex());
    imageColorLabel->setStyleSheet(imageDominantColor);
    buttonActionsAndInfoLayout->addWidget(imageColorLabel);

    // Add all elements to the main layout
    elementLayout->addWidget(imageLabel);
    elementLayout->addWidget(buttonActionsAndInfoWidget);

    // Add image layout to the flow layout
    flowLayout->addWidget(imageWidget);
}

void MainWindow::on_openImageTapped()
{
    QString fileName = this->fileOperationsManager.openFile(this);
    if (fileName.isEmpty()) {
        return;
    }
    Image image = this->fileOperationsManager.saveImage(fileName);
    showImage(image);
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

    std::map<QString, Color> m1 {{ DBConstants().ALGORITHMS.at(Algorithm::HISTOGRAM), colors[0]}};
    std::filesystem::path imagePath1 { "C:\\Users\\ilian\\Desktop\\image1.jpg" };
    Image image1 = Image(imagePath1, m1);
    m1[DBConstants().ALGORITHMS.at(Algorithm::KMEANS)] = colors[5];
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
