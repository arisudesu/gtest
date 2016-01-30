TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH +=

SOURCES += \
    src/client.cpp \
    src/game.cpp \
    src/main.cpp \
    src/shader.cpp

HEADERS += \
    include/client.h \
    include/game.h \
    include/gleq.h \
    include/shader.h

LIBS += \
    -lglfw \
    -lglbinding

CXXFLAGS += -Wall -Wsizeof-array-argument

DISTFILES += \
    data/shader/fragment.glsl \
    data/shader/vertex.glsl
