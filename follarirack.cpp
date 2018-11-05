#include "follarirack.h"

#include <QDebug>

FollariRack::FollariRack(QObject *parent) :
    QObject(parent),
    m_last_update(0),    
    m_bikes_avail(0),
    m_slots_total(0),
    m_slots_avail(0)
{

}

bool FollariRack::compareStopCode(const FollariRack *fr1, const FollariRack *fr2)
{
    return fr1->stopCode()<fr2->stopCode();
}

bool FollariRack::compareBikes(const FollariRack *fr1, const FollariRack *fr2)
{    
    return fr1->m_bikes_avail<fr2->m_bikes_avail;
}

bool FollariRack::updateFromVariantMap(const QVariantMap &r)
{    
    uint tmp;

    // Initial update, these shouldn't change (?)
    // XXX: Popup stations can move!
    if (m_stop_code.isEmpty()) {
        m_stop_code=r["stop_code"].toString();
        m_geo.setLatitude(r["lat"].toDouble());
        m_geo.setLongitude(r["lon"].toDouble());
        m_name=r["name"].toString();
    }

    qint64 ts=r["last_seen"].toLongLong();
    qDebug() << ts;
    if (ts!=m_last_update) {
        m_last_update=ts;
        m_last_seen.setMSecsSinceEpoch(ts*1000); // "setSecsSinceEpoch in 5.8"
        emit lastSeenChanged(m_last_seen);
    }

    // XXX: Put this in initial update ?
    tmp=r["slots_total"].toUInt();
    if (tmp!=m_slots_total) {
        m_slots_total=tmp;
    }

    tmp=r["bikes_avail"].toUInt();
    if (tmp!=m_bikes_avail) {
        m_bikes_avail=tmp;
        emit bikesAvailableChanged(m_bikes_avail);
    }

    tmp=r["slots_avail"].toUInt();
    if (tmp!=m_slots_avail) {
        m_slots_avail=tmp;
        emit slotsAvailableChanged(m_slots_avail);
    }

    return true;
}
