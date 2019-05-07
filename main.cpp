#include <QCoreApplication>

#include "cutefollari.h"
#include "follaridisplay.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CuteFollari cf;
    FollariDisplay fd(&cf, nullptr);

    cf.startPolling();

    return a.exec();
}
