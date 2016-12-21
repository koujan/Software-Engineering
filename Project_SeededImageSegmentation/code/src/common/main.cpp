//This main.cpp file is the starting point of the software. Here it executes all the functions in the code

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])    //main function
{
    QApplication a(argc, argv);     //creating an instance of QApplication for use
    MainWindow mainForm;            //creating a variable of mainwindow class
    mainForm.show();                //display the main window/form
    return a.exec();                //program end
}
