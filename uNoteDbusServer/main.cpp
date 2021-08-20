
#include <QCoreApplication>
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QDBusError>
#include <QDBusMetaType>
#include <QDBusMessage>

#include "uNoteDbusServer.h"
#include "note_adaptor.h"

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QDBusConnection sessionDbus = QDBusConnection::sessionBus();
    if(!sessionDbus.registerService("org.ukui.note")) {
        qCritical() << "QDbus register service failed reason:" << sessionDbus.lastError().message();
        //exit(1);
    }
    UNoteDbusServer *dbus = new UNoteDbusServer();
    new InterfaceAdaptor(dbus);
    if(!sessionDbus.registerObject("/org/ukui/note", dbus)) {
        qCritical() << "QDbus register object failed reason:" << sessionDbus.lastError().message();
    }

    return a.exec();
}

