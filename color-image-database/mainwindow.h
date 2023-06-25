#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fileOperationsManager.h"
#include "flowLayout.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openImageTapped();
    void on_colorTapped(QString hex);

private:
    Ui::MainWindow *ui;
    FlowLayout *flowLayout;
    QHBoxLayout* buttons;

    DatabaseModule db;
    FileOperationsManager fileOperationsManager;

    void testDb();

    void setupMainLayout();
    void setupLoadImageButton();
    void setupColorButtons();

    void showSavedImages();
    void showImagesWithDominantColor(QString hex);
    void showImage(Image image);
};
#endif // MAINWINDOW_H
