
CONFIG = qt debug
TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += . src
MOC_DIR = build/
OBJECTS_DIR = build/
QT += opengl

# Input
HEADERS += src/GLWidget.h \
           src/Game.h \
           src/HexTile.h \
           src/NumberChip.h \
           src/MainWindow.h \
           src/TextureManager.h \
           src/OpenGLDrawObject.h \
           src/Board.h \
           src/UIStyle.h \
           src/GameUI.h \
           src/ResourceManager.h \
           src/GameRules.h

SOURCES += src/GLWidget.cpp \
           src/Game.cpp \
           src/HexTile.cpp \
           src/NumberChip.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/TextureManager.cpp \
           src/OpenGLDrawObject.cpp \
           src/Board.cpp\
           src/UISTyle.cpp \
           src/GameUI.cpp \
           src/ResourceManager.cpp \
           src/GameRules.cpp

