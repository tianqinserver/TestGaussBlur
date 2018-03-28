QT += core
QT -= gui
QT       += network

CONFIG += c++11

TARGET = TestGaussBlur
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += "-msse2"
QMAKE_CXXFLAGS += "-mssse3"

win32: DEFINES += Q_OS_WIN \
_WIN32

#INCLUDEPATH += ../../Lyra_Server/NvBSEditServer/include/core
INCLUDEPATH += ./GaussBlur

#LIBS += -L../../Lyra_Server/NvBSEditServer/lib
#LIBS +=  -lqtutil

HEADERS += ./GaussBlur/IUnknown.h \
./GaussBlur/myNvcVisionUtils.h \
./GaussBlur/NvcColor.h \
./GaussBlur/NvcColorConv.h \
./GaussBlur/NvcColorConvDef.h \
./GaussBlur/NvcColorConvUtils.h \
./GaussBlur/NvcColorConvYUVPlanar.h \
./GaussBlur/NvcColorFunc.h \
./GaussBlur/NvcDef.h \
./GaussBlur/NvcGaussBlur.h \
./GaussBlur/NvcTypes.h \
./GaussBlur/NvcVisionCoreDef.h \
./GaussBlur/NvcVisionUtilsFFT.h \
./GaussBlur/NvcVisionUtilsPixel.h \

SOURCES += main.cpp \
./GaussBlur/myNvcVisionUtils.cpp \
./GaussBlur/NvcGaussBlur.cpp \
./GaussBlur/NvcVisionUtilsFFT.cpp \
