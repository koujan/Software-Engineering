//This images.cpp is the source file for the class images.

#include "images.h"
#include "ui_images.h"
#include "mousepos.h"

using namespace cv;

images::images(QString origName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::images)
{
    ui->setupUi(this);          //setting up the UI
    ui->rbBG->setChecked(1);    //setting background seed as default for inputing seeds
    numSeeds=1;                 //default value for number of foreground seeds
    frmOutput=new output;       //create a new instance of UI to diplay the output of the segmentation
    QPixmap orignal(origName);  //creating a pixmap of the input image to be displayed
    ui->imgShow->resize(orignal.width(),orignal.height());  //resize the Qlabel dedicated for image display/seed input according to the size of the image
    ui->imgShow->setPixmap(orignal);                        //display the input image
    resize(orignal.width()+20,orignal.height()+100);        //resize the UI according to the size of the input image
    frmOutput->inputOrigImage(orignal.toImage());           //passing input image to the output UI to use for segmentation
    ui->rb2->setVisible(0);                                 //set visibilty of foreground2 seed selection button to false as default
    ui->rb3->setVisible(0);                                 //set visibilty of foreground3 seed selection button to false as default

    connect(ui->imgShow,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));  //connect the display the mouse location wrt to the image on the dedicated label box
}

images::~images()
{
    delete ui->rb1;             //delete radiobuttion used for selecting foreground1 seeds
    delete ui->rb2;             //delete radiobuttion used for selecting foreground2 seeds
    delete ui->rb3;             //delete radiobuttion used for selecting foreground3 seeds
    delete ui->rbBG;            //delete radiobuttion used for selecting background seeds
    delete ui->btnDone;         //delete button used for execution of the segmentation algorithm
    delete ui->imgShow;         //delete the Qlabel used to take input seeds and display image
    delete ui->mouseLocation;   //delete labelbox used to display the location of the mouse
    delete frmOutput;           //delete the output UI
    delete ui;                  //delete UI used for taking input seeds
}

void images::setNumSeeds(int multi)
{
    numSeeds=multi;                     //setting the data member of the class used to represent number of seeding desired by the user on th eprevious UI
    switch (numSeeds)                   //checking how many seed buttons to show
    {
        case 3:                         //4 seeds required
            ui->rb2->setVisible(1);     //set foreground2 seed button as visible
            ui->rb3->setVisible(1);     //set foreground3 seed button as visible
            break;
        case 2:                         //3 seeds required
            ui->rb2->setVisible(1);     //set foreground2 seed button as visible
            break;
    }
}

void images::showMousePosition(QPoint &pos)
{
    ui->mouseLocation->setText("x: "+ QString::number(pos.x())+"\ty: "+ QString::number(pos.y()));  //display the location of the mouse wrt image
    xy=pos;     //setting the mouse location in a variable used to draw
}

void images::on_btnDone_clicked()
{
    image=ui->imgShow->pixmap()->toImage();     //saving the image with seeds to be passed to the segmentation algorithm
    this->setWindowTitle("Please Wait: The Segmentation Process is going on!"); //since the segmentation algorithm takes some time, display this information asking user to be patient
    frmOutput->inputSeededImage(image);         //pass the image with seeds to the UI for diplaying the image and use it in the algorithm
    frmOutput->setModal(1);                     //set output window as active
    frmOutput->exec();                          //display the UI used to display results
    this->setWindowTitle("Setup Seeds");        //set window title to input seeds again
    this->close();                              //close itself so that the user can restart with a new image
}

void images::on_rbBG_clicked()
{
    ui->imgShow->setnumSeed(0);         //set background seed selection as active
}

void images::on_rb1_clicked()
{
    ui->imgShow->setnumSeed(1);         //set foreground1 seed selection as active
}

void images::on_rb2_clicked()
{
    ui->imgShow->setnumSeed(2);         //set foreground2 seed selection as active
}

void images::on_rb3_clicked()
{
    ui->imgShow->setnumSeed(3);         //set foreground3 seed selection as active
}
