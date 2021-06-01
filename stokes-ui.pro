QT       += core gui sql datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calc12window.cpp \
    calc3window.cpp \
    calculations/chisqr.cpp \
    calculations/gradient.cpp \
    calculations/laplace.cpp \
    calculations/stokesvector.cpp \
    calculations/task12.cpp \
    calculations/task3.cpp \
    database.cpp \
    editexperimentdialog.cpp \
    experimentswindow.cpp \
    main.cpp \
    mainwindow.cpp \
    materialrefractiontable.cpp \
    newexperimentdialog.cpp \
    plotwidget.cpp \
    roweditingdelegate.cpp

HEADERS += \
    calc12window.h \
    calc3window.h \
    calculations/calculation.h \
    calculations/calculation.h \
    calculations/chisqr.h \
    calculations/task12.h \
    calculations/task3.h \
    database.h \
    editexperimentdialog.h \
    experimentswindow.h \
    mainwindow.h \
    materialrefractiontable.h \
    newexperimentdialog.h \
    plotwidget.h \
    roweditingdelegate.h

FORMS += \
    calc12window.ui \
    calc3window.ui \
    editexperimentdialog.ui \
    experimentswindow.ui \
    mainwindow.ui \
    materialrefractiontable.ui \
    newexperimentdialog.ui \
    plotwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L"C:\Program Files\PostgreSQL\13\bin" -llibpq
