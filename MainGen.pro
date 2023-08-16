QT       += core printsupport gui sql network core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    c_algin.cpp \
    dialogProgress.cpp \
    magazin.cpp \
    magazinmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    dbManager.cpp \
    dialogrepetition.cpp \
    dialogSettings.cpp \
    dialogStart.cpp \
    mfile.cpp \
    parser_placeholder.cpp \
    parser_programm.cpp \
    project.cpp \
    project_loader.cpp \
    project_saver.cpp \
    settings.cpp \
    tablemodel.cpp \
    tableprinter.cpp \
    tool.cpp \
    toollist.cpp \
    license.cpp

HEADERS += \
    c_algin.h \
    dialogProgress.h \
    magazin.h \
    magazinmodel.h \
    mainwindow.h \
    dbManager.h \
    dialogrepetition.h\
    dialogSettings.h \
    dialogStart.h \
    mfile.h \
    parser_placeholder.h \
    parser_programm.h \
    project.h \
    project_loader.h \
    project_saver.h \
    settings.h \
    tablemodel.h \
    tableprinter.h \
    tool.h \
    toollist.h \
    license.h

FORMS += \
    c_algin.ui \
    dialogProgress.ui \
    magazin.ui \
    mainwindow.ui \
    dialogrepetition.ui \
    dialogSettings.ui \
    dialogStart.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc
