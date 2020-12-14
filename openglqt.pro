QT       += core gui

LIBS += -lopengl32 -lglu32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    objloader.cpp \
    widget.cpp

HEADERS += \
    mainwindow.h \
    objloader.h \
    thinkgear.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lthinkgear
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lthinkgear

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

FORMS += \
    mainwindow.ui
