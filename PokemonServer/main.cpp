#include <QCoreApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server* myServer = new Server;
    qDebug() << myServer->port;
    return a.exec();
}
