#-------------------------------------------------
#
# Project created by QtCreator 2016-01-10T01:19:52
#
#-------------------------------------------------


QT += core gui
QT += widgets

# For Windows use these lines or change directory accordingly
#INCLUDEPATH += C:\Eigen\eigen-eigen-c58038c56923
#INCLUDEPATH += C:/OpenCV2410/opencv_bin/install/include
#LIBS += -LC:\\OpenCV2410\\opencv_bin\\bin \
#    libopencv_core2410d \
#    libopencv_highgui2410d \
#    libopencv_imgproc2410d \
#    libopencv_features2d2410d \

# For Running in Linux or Mac-OS using QT please use these lines and comment out the section above
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core  -lopencv_highgui -lopencv_imgproc

TARGET = seededimagesegmentation

TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mousepos.cpp \
    images.cpp \
    output.cpp \
    seededimgseg.cpp

HEADERS  += mainwindow.h \
    mousepos.h \
    images.h \
    output.h \
    seededimgseg.h

FORMS    += mainwindow.ui \
    images.ui \
    output.ui
