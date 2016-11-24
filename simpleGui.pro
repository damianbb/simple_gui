#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T10:46:39
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4.8): QT += widgets

QMAKE_CC = gcc
QMAKE_CXX = g++
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

TARGET = simpleGui
TEMPLATE = app

LIBS += -lboost_system


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    addressdialog.cpp \
    paramscontainer.cpp \
    netparser.cpp \
    commandparser.cpp \
    dataeater.cpp \
    netmgr.cpp \
    debugdialog.cpp \
    trivialserialize.cpp \
    netclient.cpp \
    commandexecutor.cpp \
    get_host_info.cpp

HEADERS += \
    mainwindow.hpp \
    addressdialog.hpp \
    paramscontainer.hpp \
    netparser.hpp \
    commandparser.hpp \
    dataeater.hpp \
    commandexecutor.hpp \
    json.hpp \
    netmgr.hpp \
    debugdialog.hpp \
    trivialserialize.hpp \
    netclient.hpp \
    get_host_info.hpp

FORMS += \
    mainwindow.ui \
    addressdialog.ui \
    debugdialog.ui \
    get_host_info.ui


debug {
	DESTDIR = build/debug
}

release {
	DESTDIR = build/release
}
