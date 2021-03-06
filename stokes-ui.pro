QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    editexperimentdialog.cpp \
    experimentselectordialog.cpp \
    main.cpp \
    mainwindow.cpp \
    materialrefractiontable.cpp \
    newexperimentdialog.cpp \
    roweditingdelegate.cpp \
    spreadsheet.cpp

HEADERS += \
    database.h \
    editexperimentdialog.h \
    experimentselectordialog.h \
    mainwindow.h \
    materialrefractiontable.h \
    newexperimentdialog.h \
    roweditingdelegate.h \
    spreadsheet.h

FORMS += \
    editexperimentdialog.ui \
    experimentselectordialog.ui \
    mainwindow.ui \
    materialrefractiontable.ui \
    newexperimentdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    test.py

LIBS += -L"C:\Program Files\PostgreSQL\13\bin" -llibpq
