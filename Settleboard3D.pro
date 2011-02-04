
CONFIG += debug
TEMPLATE = app
DEPENDPATH += . client
INCLUDEPATH += . client
MOC_DIR = build/
OBJECTS_DIR = build/
UI_DIR = build/
RCC_DIR = build/
TARGET = Settleboard3D
QT += network opengl
RCC_DIR = build/
RESOURCES = client/data/Resources.qrc

FORMS = client/ui/connectorform.ui \
        client/ui/prefsform.ui \
        client/ui/tradeform.ui

HEADERS = client/GLWidget.h \
          client/GLTypes.h \
          client/GameObject.h \
          client/Game.h \
          client/FileManager.h \
          client/HexTile.h \
          client/NumberChip.h \
          client/MainWindow.h \
          client/TextureManager.h \
          client/GLGameModel.h \
          client/GLGameModelProxy.h \
          client/Board.h \
          client/UIStyle.h \
          client/GameRules.h \
          client/GameConnector.h \
          client/PrefsForm.h \
          client/Player.h \
          client/PlayerObject.h \
          client/OBJGLLoader.h \
          client/Crossroad.h \
          client/Roadway.h \
          client/GameCard.h \
          client/GameCardStack.h \
          client/Bank.h \
          client/ControlPanel.h \
          client/PlayerPanel.h \
          client/MessagePanel.h \
          client/GameInfoPanel.h \
          client/NetworkCore.h

SOURCES = client/GLWidget.cpp \
          client/Game.cpp \
          client/FileManager.cpp \
          client/HexTile.cpp \
          client/NumberChip.cpp \
          client/main.cpp \
          client/MainWindow.cpp \
          client/TextureManager.cpp \
          client/GLGameModel.cpp \
          client/GLGameModelProxy.cpp \
          client/Board.cpp\
          client/UISTyle.cpp \
          client/GameRules.cpp \
          client/GameConnector.cpp \
          client/PrefsForm.cpp \
          client/Player.cpp \
          client/PlayerObject.cpp \
          client/OBJGLLoader.cpp \
          client/Crossroad.cpp \
          client/Roadway.cpp \
          client/Bank.cpp \
          client/GameCardStack.cpp \
          client/ControlPanel.cpp \
          client/PlayerPanel.cpp \
          client/MessagePanel.cpp \
          client/GameInfoPanel.cpp \
          client/NetworkCore.cpp

