#-------------------------------------------------
#
# Project created by QtCreator 2012-05-24T22:30:54
#
#-------------------------------------------------

QT       += core gui

TARGET = Raw
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    csv.cpp

HEADERS  += mainwindow.h \
    csv.h

INCLUDEPATH =+ C:\Qwt-6.0.1\include

LIBPATH =+ C:\Qwt-6.0.1\lib

LIBS += -lqwt

OTHER_FILES += \
    import2.csv \
    import.csv

RESOURCES += \
    resources.qrc
