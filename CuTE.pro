QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    cppsyntaxhighlighter.cpp \
    editorsettings.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp

HEADERS += \
    codeeditor.h \
    cppsyntaxhighlighter.h \
    editorsettings.h \
    mainwindow.h \
    settingsdialog.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    app_resources.qrc
