#include <QCoreApplication>

#include "cutefollari.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CuteFollari cf;

    cf.startPolling();

    return a.exec();
}
