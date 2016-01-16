TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/freetype2

SOURCES += \
    client.cpp \
    game.cpp \
    main.cpp

HEADERS += \
    gleq.h \
    client.h \
    game.h

LIBS += \
    -lglfw \
    -lglbinding
