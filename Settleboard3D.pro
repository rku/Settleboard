
CONFIG += debug
TEMPLATE = app
DEPENDPATH += . Client
INCLUDEPATH += . Client
MOC_DIR = Build/
OBJECTS_DIR = Build/
UI_DIR = Build/
RCC_DIR = Build/
QT += network opengl

FORMS = Client/ui/connectorform.ui \
        Client/ui/prefsform.ui

HEADERS = Client/GLWidget.h \
          Client/GLTypes.h \
          Client/GameObject.h \
          Client/Game.h \
          Client/FileManager.h \
          Client/HexTile.h \
          Client/NumberChip.h \
          Client/MainWindow.h \
          Client/TextureManager.h \
          Client/GLGameModel.h \
          Client/GLGameModelProxy.h \
          Client/Board.h \
          Client/UIStyle.h \
          Client/GameUI.h \
          Client/Resource.h \
          Client/GameRules.h \
          Client/GameConnector.h \
          Client/PrefsForm.h \
          Client/Player.h \
          Client/PlayerObject.h \
          Client/OBJGLLoader.h \
          Client/Crossroad.h \
          Client/Roadway.h \

SOURCES = Client/GLWidget.cpp \
          Client/Game.cpp \
          Client/FileManager.cpp \
          Client/HexTile.cpp \
          Client/NumberChip.cpp \
          Client/main.cpp \
          Client/MainWindow.cpp \
          Client/TextureManager.cpp \
          Client/GLGameModel.cpp \
          Client/GLGameModelProxy.cpp \
          Client/Board.cpp\
          Client/UISTyle.cpp \
          Client/GameUI.cpp \
          Client/Resource.cpp \
          Client/GameRules.cpp \
          Client/GameConnector.cpp \
          Client/PrefsForm.cpp \
          Client/Player.cpp \
          Client/PlayerObject.cpp \
          Client/OBJGLLoader.cpp \
          Client/Crossroad.cpp \
          Client/Roadway.cpp

