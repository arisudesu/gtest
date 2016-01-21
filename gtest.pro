TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH +=

SOURCES += \
    client.cpp \
    game.cpp \
    main.cpp \
    shader.cpp

HEADERS += \
    gleq.h \
    client.h \
    game.h \
    shader.h

LIBS += \
    -lglfw \
    -lglbinding

CXXFLAGS += -Wall -Wsizeof-array-argument
