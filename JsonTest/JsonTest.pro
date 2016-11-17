QT += core
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4.8): QT += widgets

QMAKE_CC = gcc
QMAKE_CXX = g++

CONFIG += c++14

TARGET = JsonTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
