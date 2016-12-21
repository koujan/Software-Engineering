//This mainwindow.h is the header file for the class mainwindow.
//This class is used to take input from the user about the level of segmentation desired

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QMessageBox>

namespace Ui {
class MainWindow;       //creating a namespace for the ui and associating it with the class
}

class MainWindow : public QMainWindow
{
    Q_OBJECT            //creating object of mainwindow class

public:
    explicit MainWindow(QWidget *parent = 0);   //constructor for the class
    ~MainWindow();                              //destructor for the class

private slots:
    void on_btnBrowse_clicked();                //prototype of function that is called when the browse button is clicked (search for images)

private:
    Ui::MainWindow *ui;         //creating a data member called ui to point to the GUI of the class
};

#endif // MAINWINDOW_H
