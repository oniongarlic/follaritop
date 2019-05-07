#include "follaridisplay.h"


#include <QList>

FollariDisplay::FollariDisplay(CuteFollari *cf, QObject *parent) : QObject(parent), m_cf(cf)
{
    QObject::connect(m_cf, SIGNAL(updated()), this, SLOT(updated()));
}

void FollariDisplay::updated()
{
    printStations();
}

void FollariDisplay::printStations()
{
    printf("\e[1;1H\e[2J");

    printf("Updated: %s\nAvailable bikes: %3d\n", m_cf->getUpdateDateTime().toString().toLocal8Bit().constData(), m_cf->getBikesAvailable());
    printf("ID  Name                               Bikes      Slots       Available\n");

    QList<FollariRack *> k=m_cf->getRacks();

    std::sort(k.begin(), k.end(), FollariRack::compareBikes);

    QListIterator<FollariRack *> i(k);
    while (i.hasNext()) {
        FollariRack *fr=i.next();

        QString p=QString("%1 %2: [%3] [%4] [%5]").arg(fr->stopCode(),3).arg(fr->stopName(), 32).arg(fr->bikesAvailable(),8).arg(fr->slotsAvailable(),8).arg(fr->slotsTotal(),8);

        printf("%s %s %s\n", p.toUtf8().constData(), fr->bikesAvailable()<3 ? "!" : "", fr->lastSeen().toString().toLocal8Bit().constData());
    }
}
