#include "follarirack.h"

FollariRack::FollariRack(QObject *parent) :
    QObject(parent),
    m_last_update(0),
    m_slots_avail(0),
    m_slots_total(0),
    m_bikes_avail(0)
{

}

bool FollariRack::updateFromVariantMap(const QVariantMap r)
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

    tmp=r["last_seen"].toInt();
    if (tmp!=m_last_update) {
        m_last_update=tmp;
        m_last_seen.setMSecsSinceEpoch(tmp);
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
