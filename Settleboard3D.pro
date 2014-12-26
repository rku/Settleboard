CONFIG += qt debug
#DEFINES += QT_NO_DEBUG_OUTPUT
TEMPLATE = app
DEPENDPATH += . src
INCLUDEPATH += . src
MOC_DIR = build/
OBJECTS_DIR = build/
UI_DIR = build/
RCC_DIR = build/
TARGET = Settleboard3D
QT += network opengl widgets
LIBS += -lGLU
RCC_DIR = build/
RESOURCES = src/resources/Resources.qrc
RC_FILE = src/icons/Settleboard.rc
ICON = src/icons/Settleboard.icns

FORMS = src/ui/connectorform.ui \
        src/ui/prefsform.ui \
        src/ui/gamelobbyform.ui \
        src/ui/infoboxform.ui \
        src/ui/tradeofferform.ui \
        src/ui/createtradeofferform.ui \
        src/ui/tradeform.ui

HEADERS = src/GLWidget.h \
          src/GLTypes.h \
          src/Game.h \
          src/FileManager.h \
          src/HexTile.h \
          src/NumberChip.h \
          src/MainWindow.h \
          src/TextureManager.h \
          src/ResourceManager.h \
          src/GLGameModel.h \
          src/GLGameModelProxy.h \
          src/Board.h \
          src/GameRules.h \
          src/GameConnector.h \
          src/PrefsForm.h \
          src/Player.h \
          src/PlayerObject.h \
          src/OBJGLLoader.h \
          src/Crossroad.h \
          src/Roadway.h \
          src/GameCard.h \
          src/GameCardStack.h \
          src/Bank.h \
          src/ControlPanel.h \
          src/PlayerPanel.h \
          src/MessagePanel.h \
          src/GameInfoPanel.h \
          src/ResourceInfoPanel.h \
          src/NetworkCore.h \
          src/NetworkPacket.h \
          src/GameLobby.h \
          src/Map.h \
          src/StandardMap.h \
          src/Robber.h \
          src/GamePixmap.h \
          src/GameCardSelectionDialog.h \
          src/GameCardPanel.h \
          src/GraphicsGameCardItem.h \
          src/GraphicsCardSelectionScene.h \
          src/TradeDialog.h \
          src/TradeOffer.h \
          src/TradeOfferDialog.h \
          src/CreateTradeOfferDialog.h \
          src/TradeManager.h \
          src/ResourceInfoWidget.h \
          src/Utils.h

SOURCES = src/GLWidget.cpp \
          src/Game.cpp \
          src/FileManager.cpp \
          src/HexTile.cpp \
          src/NumberChip.cpp \
          src/main.cpp \
          src/MainWindow.cpp \
          src/TextureManager.cpp \
          src/ResourceManager.cpp \
          src/GLGameModel.cpp \
          src/GLGameModelProxy.cpp \
          src/Board.cpp\
          src/GameRules.cpp \
          src/GameConnector.cpp \
          src/PrefsForm.cpp \
          src/Player.cpp \
          src/PlayerObject.cpp \
          src/OBJGLLoader.cpp \
          src/Crossroad.cpp \
          src/Roadway.cpp \
          src/GameCard.cpp \
          src/Bank.cpp \
          src/GameCardStack.cpp \
          src/ControlPanel.cpp \
          src/PlayerPanel.cpp \
          src/MessagePanel.cpp \
          src/GameInfoPanel.cpp \
          src/ResourceInfoPanel.cpp \
          src/NetworkCore.cpp \
          src/NetworkPacket.cpp \
          src/GameLobby.cpp \
          src/Map.cpp \
          src/StandardMap.cpp \
          src/Robber.cpp \
          src/GamePixmap.cpp \
          src/GameCardSelectionDialog.cpp \
          src/GameCardPanel.cpp \
          src/GraphicsGameCardItem.cpp \
          src/GraphicsCardSelectionScene.cpp \
          src/TradeDialog.cpp \
          src/TradeOffer.cpp \
          src/TradeOfferDialog.cpp \
          src/TradeManager.cpp \
          src/CreateTradeOfferDialog.cpp \
          src/ResourceInfoWidget.cpp
