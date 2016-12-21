//This output.h is the header file for the class output.
//This class is used to runt he segmentation algorithm and display the results of segmentation

#ifndef OUTPUT_H
#define OUTPUT_H

#include <QImage>
#include <string>
#include <QLabel>
#include <QDialog>

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "opencv2/core/core.hpp"
#include "opencv2/core/eigen.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

using namespace std;
using namespace Eigen;
using namespace cv;


using namespace cv;

namespace Ui {
class output;       //creating a namespace for the ui and associating it with the class
}

class output : public QDialog
{
    Q_OBJECT        //creating object of UI class used to display the segmentation results

public:
    explicit output(QWidget *parent = 0);   //constructor for class
    ~output();                              //destructor for class
    void inputSeededImage(QImage);          //used for taking unseeded image input
    void inputOrigImage(QImage);            //used for taking seeded image as input and running the algorithm

private:
    Ui::output *ui;         //creating a data member called ui to point to the GUI of the class
    Mat image;              //variable to represent unseeded image
    Mat imageSeeded;        //variable to represent seeded image
    QLabel *seedImage;      //create a Qlabel to display seeded image
    QLabel *segImage;       //create a Qlabel to display the segmentation results
};

#endif // OUTPUT_H
