
CONFIG += debug
TEMPLATE = app
DEPENDPATH += . src
INCLUDEPATH += . src
MOC_DIR = build/
OBJECTS_DIR = build/
UI_DIR = build/
RCC_DIR = build/
QT += network opengl

FORMS = src/ui/connectorform.ui \
        src/ui/prefsform.ui

HEADERS = src/GLWidget.h \
          src/GLTypes.h \
          src/GameObject.h \
          src/Game.h \
          src/HexTile.h \
          src/NumberChip.h \
          src/MainWindow.h \
          src/TextureManager.h \
          src/GLGameModel.h \
          src/Board.h \
          src/UIStyle.h \
          src/GameUI.h \
          src/Resource.h \
          src/GameRules.h \
          src/GameConnector.h \
          src/PrefsForm.h \
          src/Player.h \
          src/Building.h \
          src/OBJGLLoader.h \
          src/Crossroad.h \
          src/Roadway.h

SOURCES = src/GLWidget.cpp \
          src/Game.cpp \
          src/HexTile.cpp \
          src/NumberChip.cpp \
          src/main.cpp \
          src/MainWindow.cpp \
          src/TextureManager.cpp \
          src/GLGameModel.cpp \
          src/Board.cpp\
          src/UISTyle.cpp \
          src/GameUI.cpp \
          src/Resource.cpp \
          src/GameRules.cpp \
          src/GameConnector.cpp \
          src/PrefsForm.cpp \
          src/Player.cpp \
          src/Building.cpp \
          src/OBJGLLoader.cpp \
          src/Crossroad.cpp \
          src/Roadway.cpp

