#-------------------------------------------------
#
# Project created by QtCreator 2016-06-07T22:08:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = DFT
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp

unix:!macx:!symbian: LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml
