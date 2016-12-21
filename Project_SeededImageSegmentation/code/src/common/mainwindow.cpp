//This mainwindow.cpp is the source file for the class mainwindow.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "images.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);          //setting up the main window
    ui->rb1->setChecked(1);     //setting single region segmentation as default (if the user forgets to choose the level of segmentation)
}

MainWindow::~MainWindow()
{
    delete ui->rb1;             //deleting radio button for single region segmentation
    delete ui->rb2;             //deleting radio button for three region segmentation
    delete ui->rb3;             //deleting radio button for four region segmentation
    delete ui->btnBrowse;       //deleting button for selecting the input image
    delete ui->groupBox_2;      //deleting the frame box that contained the radiobuttons for selecting the type of segmentation
    delete ui->label;           //deleting the label box for the title box
    delete ui;                  //deleting the GUI/mainwindow
}

void MainWindow::on_btnBrowse_clicked()
{
    //this creates a QDialog box that asks the user to select input image of eihter .jpg, .png or .bmp formats
    QString const origName = QFileDialog::getSaveFileName(this, tr("Select Unseeded Input Image to Segment"), "/home", tr("Images (*.jpg *.png *.bmp)"));
    Mat imgOrg=imread(origName.toUtf8().constData());       //read the file that user selected and save it in opencv format
    if (!imgOrg.data)               //check if the input file is valid
    {
        QMessageBox::critical(this,tr("Error"), tr("The Image could not load!!!"));     //display an error box if the input file is not valid
    }
    else
    {
        images* frmSeed;                    //create a pointer that points to the UI that will take input seeds from the user
        frmSeed=new images(origName);       //create a new instance of the seed input UI with the address of input image used as input argument

        if(ui->rb3->isChecked())            //check whiich radiobutton(2) did the user select for the level of segmentation
            frmSeed->setNumSeeds(3);        //pass the number of foreground seeds to the seed input UI
        else if(ui->rb2->isChecked())       //check which radiobutton(3) did the user select for the level of segmentation
            frmSeed->setNumSeeds(2);        //pass the number of foreground seeds to the seed input UI

        this->setVisible(0);                //hide the current UI
        frmSeed->setModal(1);               //set the seed input UI as modal
        frmSeed->exec();                    //execute the seed input UI
        delete frmSeed;                     //after execution of 1 segmentation, delete the instance to the seed input UI
        this->setVisible(1);                //show the current UI again
    }
}
