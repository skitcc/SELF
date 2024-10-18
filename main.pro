QT += widgets

SOURCES += main.cpp \
           mainwindow.cpp \
           ./maths/src/math_module.c

HEADERS += mainwindow.h \
           ./maths/inc/definitions_math.h \
           ./maths/inc/math_module.h 

INCLUDEPATH += ./math_module/inc  
