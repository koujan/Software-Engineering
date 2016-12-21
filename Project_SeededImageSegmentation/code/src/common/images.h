//This images.h is the header file for the class images.
//This class is used to take input from the user about the seeds used for segmentation

#ifndef IMAGES_H
#define IMAGES_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "output.h"

namespace Ui {
class images;       //creating a namespace for the ui and associating it with the class
}

class images : public QDialog
{
    Q_OBJECT        //creating object of UI class used for inputing seeds

public:
    explicit images(QString, QWidget *parent = 0);  //constructor for the class
    ~images();                                      //destructor for the class
    void setNumSeeds(int);                          //prototype for function that hide/shows the desired number of seeds for choosing

private:
    Ui::images *ui;         //creating a data member called ui to point to the GUI of the class
    QPoint xy;              //represents the location of the mouse of the image
    int numSeeds;           //represents the number of seeds the user desired
    output *frmOutput;      //creates a pointer that points to the UI used for displaying the segmentation results
    QImage image;           //represents the input image to be segmented

public slots:
    void showMousePosition(QPoint& pos);    //display the mouse location wrt to the image if the mouse is pressed

private slots:
    void on_btnDone_clicked();      //represents button click function which is to be pressed/clicked when the user has input the seeds
    void on_rbBG_clicked();         //when background seeds are to be inserted
    void on_rb1_clicked();          //when foreground3 seeds are to be inserted
    void on_rb2_clicked();          //when foreground2 seeds are to be inserted
    void on_rb3_clicked();          //when foreground1 seeds are to be inserted
};

#endif // IMAGES_H
