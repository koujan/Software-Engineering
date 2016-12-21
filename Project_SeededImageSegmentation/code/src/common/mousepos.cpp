//This mousepos.cpp is the source file for the class mouse.

#include "mousepos.h"

#include <QDebug>

mousePos::mousePos(QWidget *parent): QLabel(parent)
{
    this->setMouseTracking(1);      //set mouse tracking to be true
    pressed=0;                      //set mouse press variable to be false as default
    numSeed=0;                      //set the seed to draw as background as default
    drawBGFG.setWidth(10);          //set pen width used for drawing to 10 pixels
    drawBGFG.setColor(Qt::blue);    //set pen color to be blue for background as default
}

mousePos::~mousePos()
{
}

bool mousePos::press()
{
    return pressed;                     //return true if the mouse is pressed
}

void mousePos::setnumSeed(int num)
{
    numSeed=num;                        //input the number of seed to be drawn
    switch(numSeed)                     //selection of pen color depending on seed number chosen
    {
    case 1:                             //foreground1
        drawBGFG.setColor(Qt::red);     //red color for pen
        break;
    case 2:                             //foreground2
        drawBGFG.setColor(Qt::green);   //green color for pen
        break;
    case 3:                             //foreground3
        drawBGFG.setColor(Qt::yellow);  //yellow color for pen
        break;
    default:                            //background
        drawBGFG.setColor(Qt::blue);    //blue color for pen
    }
}

void mousePos::mouseMoveEvent(QMouseEvent *mouse_event)
{
    QPoint mousePos=mouse_event->pos(); //read mouse position wrt label box used for inputting seeds
    if(pressed==1)                      //check if mouse is pressed
    {
        if (mousePos.x() <= this->size().width() && mousePos.y()<=this->size().height())    //check if mouse is over image
        {
            if(mousePos.x()>=0 && mousePos.y()>=0)  //check if mouse is over the input image
            {
                emit sendMousePosition(mousePos);   //pass the mouse location as signal to UI
                xy=mousePos;                        //paas the mouse location
                update();                           //repaint
            }
        }
    }
}

void mousePos::mousePressEvent(QMouseEvent *mouse_event)
{
    if(mouse_event->button()==Qt::LeftButton)   //check if mouse(left button) is pressed
        pressed=1;                              //set mouse press bool to true
}

void mousePos::mouseReleaseEvent(QMouseEvent *mouse_event)
{
    if(mouse_event->button()==Qt::LeftButton)   //check if mouse(left button) is released
        pressed=0;                              //set mouse press bool to false
}

void mousePos::paintEvent(QPaintEvent* paint_event)
{
    image = this->pixmap()->toImage();              //draw image on Qlabel to display image with already added seeds
    QPainter seeded(&image);                        //declare painter for drawing on the image
    QPainter seeds(this);                           //declare painter on the image
    seeded.setPen(drawBGFG);                        //set pen color
    seeds.drawImage(0,0,image);                     //draw image at starting location of Qlabel
    if (pressed)                                    //check if mouse is pressed
    {
        seeded.drawPoint(xy);                       //draw at the current location of the mouse
        this->setPixmap(QPixmap::fromImage(image)); //set the updated image as pixmap for displaying purposes
    }
}
