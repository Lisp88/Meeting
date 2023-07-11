QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat_dialog.cpp \
    chat_info_widget.cpp \
    kernel.cpp \
    login_dialog.cpp \
    main.cpp \
    main_dialog.cpp \
    room_dialog.cpp \
    set_user.cpp \
    user_show.cpp \
    work_thread.cpp

HEADERS += \
    chat_dialog.h \
    chat_info_widget.h \
    kernel.h \
    login_dialog.h \
    main_dialog.h \
    room_dialog.h \
    set_user.h \
    user_show.h \
    work_thread.h


FORMS += \
    chat_dialog.ui \
    chat_info_widget.ui \
    login_dialog.ui \
    main_dialog.ui \
    room_dialog.ui \
    set_user.ui \
    user_show.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(./netapi/netapi.pri)
include(./MD5/MD5.pri)
include(./uiapi/uiapi.pri)
include(./AudioApi/AudioApi.pri)
include(./VideoApi/videoapi.pri)

INCLUDEPATH += ./netapi/\
            ./MD5/\
            ./uiapi/\
            ./AudioApi\
            ./VideoApi

RESOURCES += \
    source.qrc
