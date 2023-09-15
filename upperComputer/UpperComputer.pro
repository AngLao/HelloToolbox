QT       += core gui
QT       += serialport      #加入串口模块
QT       += printsupport    #加入画图模块
QT       += network         #加入网络编程

#DEFINES += QT_NO_WARNING_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    ParameterConfiguration.cpp \
    SerialPortBase.cpp \
    WaveformDisplay.cpp \
    extend/EasyPact.c \
    extend/panelwidget.cpp \
    uiinit.cpp \
    widget.cpp

HEADERS += \
    ParameterConfiguration.h \
    SerialPortBase.h \
    WaveformDisplay.h \
    extend/EasyPact.h \
    extend/panelwidget.h \
    network/networkHeader.h \
    uiinit.h \
    widget.h

FORMS += \
    SerialPortBase.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main.qrc

#设置图标
RC_ICONS = icon/software.ico

#波形图qcustomplot2 dll路径
LIBS += -L$$PWD/lib/ -lqcustomplot2

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

#opencv路径
#INCLUDEPATH += D:\Work\openCv\cmakeBuild\install\include
#               D:\Work\openCv\cmakeBuild\install\include\opencv2

#LIBS += -L D:\Work\openCv\cmakeBuild\lib\libopencv_*.a

DISTFILES +=
