#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "QtWidgets/qcombobox.h"
#include "fileOperationsManager.h"
#include "flowLayout.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Default constructor, sets up the UI.
     * \param QWidget the widget's parent.
     */
    MainWindow(QWidget *parent = nullptr);
    /*!
     * \brief Destructor Deallocates memory
     */
    ~MainWindow();

private slots:
    /*!
     * \brief Slot Executed when "Load new image" is tapped
    */
    void on_openImageTapped();
    /*!
     * \brief Slot Executed when a color from the palette is tapped
     * \param QString HEX representing the color
     */
    void on_colorTapped(QString hex);

private:
    Ui::MainWindow *ui;
    /*!
     * \brief Custom layout class representing the grid of images
     */
    FlowLayout *flowLayout;
    /*!
     * \brief Images controls panel
     */
    QHBoxLayout* buttons;
    /*!
     * \brief Algorithm picker
     */
    QComboBox *picker;
    /*!
     * \brief Color palette
     */
    std::vector<QPushButton> colorButtons;

    // -1 means all colors
    int selectedButtonIndex = -1;
    /*!
     * \brief The colors that are fetched from the database
     */
    std::vector<Color> savedColors;

    DatabaseModule db;
    FileOperationsManager fileOperationsManager;

    void testDb();

    /*!
     * \brief Sets up the main components of the layout - the grid of images and images controls panel
     */
    void setupMainLayout();
    /*!
     * \brief Sets up the "Load new image" button - sets title and connects the button to a lambda function
     */
    void setupLoadImageButton();
    /*!
     * \brief Sets up the color palette buttons
     */
    void setupColorButtons();
    /*!
     * \brief Sets up the algorithm picker
     */
    void setupAlgorithmPicker();

    /*!
     * \brief Loads all images that are saved in the database in the UI
     */
    void showSavedImages();
    /*!
     * \brief Loads all images with specified dominant color
     * \param hex
     */
    void showImagesWithDominantColor(QString hex);
    /*!
     * \brief Loads a specified image in the grid
     * \param Image Image to be loaded in the grid
     */
    void showImage(Image image);

    /*!
     * \brief Unhighlights a color button
     * \param int Index of the button
     */
    void unhighlightButtonWithIndex(int index);
    /*!
     * \brief Highlights a color button
     * \param int Index of the button
     */
    void highlightButtonWithIndex(int index);
    /*!
     * \brief Unhighlights the already selected button if needed
     */
    void unhighlightSelectedButtonIfNeeded();
};
#endif // MAINWINDOW_H
