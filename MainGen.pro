QT       += core printsupport gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbMainManager.cpp \
    dbManager.cpp \
    dialogSettings.cpp \
    dialogStart.cpp \
    dialogWrite.cpp \
    dialogrecover.cpp \
    holder.cpp \
    holderlist.cpp \
    main.cpp \
    parser_programm.cpp \
    tool.cpp \
    toollist.cpp \
    parser_wzm.cpp \
    tablemodel.cpp \
    parser.cpp \
    tableprinter.cpp \
    mainwindow.cpp

HEADERS += \
    dbMainManager.h \
    dbManager.h \
    dialogSettings.h \
    dialogStart.h \
    dialogWrite.h \
    dialogrecover.h \
    holder.h \
    holderlist.h \
    parser_programm.h \
    tool.h \
    toollist.h \
    parser_wzm.h \
    tablemodel.h \
    parser.h \
    tableprinter.h \
    mainwindow.h

FORMS += \
    dialogSettings.ui \
    dialogStart.ui \
    dialogWrite.ui \
    dialogrecover.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

