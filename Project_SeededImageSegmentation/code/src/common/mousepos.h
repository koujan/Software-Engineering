//This mousepos.h is the header file for the class mouse.
//This class is used to draw the seeds on to the image

#ifndef MOUSEPOS_H
#define MOUSEPOS_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QtGui>
#include <QtCore>
#include <QImage>

class mousePos : public QLabel
{
    Q_OBJECT            //create the object for a class inherited from Qlabel class to draw the seeds according to the mouse position

public:
    mousePos(QWidget* parent = 0);  //constructor for the class
    ~mousePos();                    //destructor for the class
    bool press();                   //used to output whether the emouse is pressed or not
    void setnumSeed(int);           //select color for drawing seeds

private:
    bool pressed;                   //represents the mouse press function of the class
    QPoint xy;                      //represents the location of the mouse on the image
    int numSeed;                    //represents which seed to draw
    QPen drawBGFG;                  //Qpen to draw seeds
    QImage image;                   //represents the image which is being drawn at

protected:
    void mouseMoveEvent(QMouseEvent* mouse_event);      //overload mouse move event function to draw
    void mousePressEvent(QMouseEvent* mouse_event);     //overload mouse press event to see if to draw seeds
    void mouseReleaseEvent(QMouseEvent* mouse_event);   //overload mouse release event function to see when the mouse press has been stopped
    void paintEvent(QPaintEvent* paint_event);          //overload paint event to draw on image

signals:
    void sendMousePosition(QPoint&);                    //output mouse location on the image
};

#endif // MOUSEPOS_H
