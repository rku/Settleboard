
CONFIG += qt debug
#DEFINES += QT_NO_DEBUG_OUTPUT
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
RESOURCES = client/resources/Resources.qrc
RC_FILE = client/icons/Settleboard.rc
ICON = client/icons/Settleboard.icns

FORMS = client/ui/connectorform.ui \
        client/ui/prefsform.ui \
        client/ui/tradeform.ui \
        client/ui/gamelobbyform.ui \
        client/ui/infoboxform.ui \
        client/ui/cardbrowserform.ui

HEADERS = client/GLWidget.h \
          client/GLTypes.h \
          client/Game.h \
          client/FileManager.h \
          client/HexTile.h \
          client/NumberChip.h \
          client/MainWindow.h \
          client/TextureManager.h \
          client/GLGameModel.h \
          client/GLGameModelProxy.h \
          client/Board.h \
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
          client/ResourceInfoPanel.h \
          client/NetworkCore.h \
          client/NetworkPacket.h \
          client/GameLobby.h \
          client/Map.h \
          client/StandardMap.h \
          client/Robber.h \
          client/GamePixmap.h \
          client/GameCardSelectionDialog.h \
          client/GameCardPanel.h \
          client/GraphicsGameCardItem.h \
          client/GraphicsCardSelectionScene.h \
          client/Utils.h

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
          client/GameRules.cpp \
          client/GameConnector.cpp \
          client/PrefsForm.cpp \
          client/Player.cpp \
          client/PlayerObject.cpp \
          client/OBJGLLoader.cpp \
          client/Crossroad.cpp \
          client/Roadway.cpp \
          client/GameCard.cpp \
          client/Bank.cpp \
          client/GameCardStack.cpp \
          client/ControlPanel.cpp \
          client/PlayerPanel.cpp \
          client/MessagePanel.cpp \
          client/GameInfoPanel.cpp \
          client/ResourceInfoPanel.cpp \
          client/NetworkCore.cpp \
          client/NetworkPacket.cpp \
          client/GameLobby.cpp \
          client/Map.cpp \
          client/StandardMap.cpp \
          client/Robber.cpp \
          client/GamePixmap.cpp \
          client/GameCardSelectionDialog.cpp \
          client/GameCardPanel.cpp \
          client/GraphicsGameCardItem.cpp \
          client/GraphicsCardSelectionScene.cpp

