#Adds the following Qt modules:
#core: Core functionality (e.g., signals/slots, event loop, containers)
#gui: Basic GUI elements
#multimedia: Audio and video playback features
#network: Networking capabilities
QT       += core gui multimedia network

#If using Qt 5 or newer, include the widgets module (needed for QMainWindow, QPushButton, etc.).
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#Sets the project to use the C++17 standard during compilation.
CONFIG += c++17

#Includes the header and library path for TagLib, a library for reading and editing audio metadata (e.g., MP3 tags like title, artist).
INCLUDEPATH += /opt/homebrew/opt/taglib/include
LIBS       += -L/opt/homebrew/opt/taglib/lib -ltag

#Source and Header Files
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
