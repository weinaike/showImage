#-------------------------------------------------
#
# Project created by QtCreator 2017-02-13T10:20:09
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = showImage
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    bbox.cpp \
    food.cpp

HEADERS  += dialog.h \
    bbox.h \
    food.h

FORMS    += dialog.ui

RC_FILE +=rcfile.rc

DISTFILES += \
    foodlist.txt


