QT += widgets

SOURCES += main.cpp \
           mainwindow.cpp \
           ./math_module/src/math_module.c 

HEADERS += mainwindow.h \
           ./math_module/inc/math_module.h  

INCLUDEPATH += ./math_module/inc  
