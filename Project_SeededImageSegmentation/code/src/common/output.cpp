//This output.cpp is the source file for the class output.

#include "output.h"
#include "ui_output.h"
#include "seededimgseg.h"

output::output(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::output)
{
    ui->setupUi(this);                  //setting up the UI
    segImage = new QLabel(this);        //creating a new instance of label used to display segmentation results
    seedImage = new QLabel(this);       //creating a new instance of label used to display seed image
}

output::~output()
{
    delete seedImage;       //delete the label used to display seeded image
    delete segImage;        //delete the label used to display the segmentation result
    delete ui;              //delete the UI instance
}
void output::inputSeededImage(QImage imsds)
{
    //converting QImage to cv::Mat so opencv can be used to manipulate the image
    imageSeeded= Mat(imsds.height(),imsds.width(),CV_8UC4,const_cast<uchar*>(imsds.bits()), imsds.bytesPerLine()).clone();
    seedImage->setPixmap(QPixmap::fromImage(imsds));    //display the seeded image
    cvtColor(image,image, CV_BGRA2BGR);                 //convert 4 channel unseeded image to 3 channel image
    cvtColor(imageSeeded,imageSeeded, CV_BGRA2BGR);     //convert 4 channel seeded image to 3 channel image
    Mat diff=image.clone();

    for(int i=0;i<imageSeeded.rows;i++)  //two nested for loops for extracting the seeds from the seeded image
    {
        for(int j=0;j<imageSeeded.cols;j++){
                if(image.at<Vec3b>(i,j)-imageSeeded.at<Vec3b>(i,j)!=Vec3b(0,0,0))
                    diff.at<Vec3b>(i,j)=imageSeeded.at<Vec3b>(i,j);
                else
                    diff.at<Vec3b>(i,j)=Vec3b(255,255,255);
        }

    }
//    imshow("seeds",diff);
    seededimgseg segments(diff, image,0.05,0.1,1,3,pow(10,-6)); //create an object of class used for segmentation algorithm
    segments.segmentImage();                        //run the segmentation algorithm
    Mat segment=segments.get_segmentedImg();        //get the segmentation results as a cv::Mat
    cvtColor(segment, segment,CV_BGR2RGB);          //convert BGR to RGB for displaying
    QImage segmentedOutput((const uchar *) segment.data, segment.cols, segment.rows, segment.step, QImage::Format_RGB888);  //convert cv::Mat to Qimage for displaying purposes
    segImage->setPixmap(QPixmap::fromImage(segmentedOutput));   //display the segmentation output image
}

void output::inputOrigImage(QImage imsds)
{
    //convert QImage to cv::Mat
    image= Mat(imsds.height(),imsds.width(),CV_8UC4,const_cast<uchar*>(imsds.bits()), imsds.bytesPerLine()).clone();
    seedImage->move(20,100);                        //set starting location on the UI for the seeded image
    segImage->move(40+image.cols,100);              //set starting location on the UI for the segmented image
    this->resize(60+2*image.cols,120+image.rows);   //set the size of the UI according to the size of the input image
}
