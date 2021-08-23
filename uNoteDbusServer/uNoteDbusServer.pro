TEMPLATE = app
TARGET = ukui-notebook-server
INCLUDEPATH += .

QT += dbus core sql
# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
DBUS_ADAPTORS += ../data/org.ukui.note.xml
DBUS_INTERFACES += ../data/org.ukui.note.xml

HEADERS +=   \
    uNoteDbusServer.h

SOURCES +=   \
    main.cpp \
    uNoteDbusServer.cpp

target.path = /usr/bin/
!isEmpty(target.path): INSTALLS += target

dbus_conf.path = /etc/dbus-1/system.d
dbus_conf.files +=  ../data/org.ukui.note.conf
INSTALLS += dbus_conf

systemd_service.path = /usr/share/dbus-1/services/
#systemd_service.path = /lib/systemd/system
systemd_service.files += ../data/org.ukui.note.service
INSTALLS += systemd_service






