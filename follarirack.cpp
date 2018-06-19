#include "follarirack.h"

FollariRack::FollariRack(QObject *parent) : QObject(parent)
{

}

bool FollariRack::updateFromVariantMap(const QVariantMap r)
{
    m_stop_code=r["stop_code"].toString();
    m_name=r["name"].toString();
    m_last_seen.setMSecsSinceEpoch(r["last_seen"].toInt());

    m_bikes_avail=r["bikes_avail"].toUInt();
    m_slots_total=r["slots_total"].toUInt();
    m_slots_avail=r["slots_avail"].toUInt();

    m_geo.setLatitude(r["lat"].toDouble());
    m_geo.setLongitude(r["lon"].toDouble());

    emit bikesAvailableChanged(m_bikes_avail);

    return true;
}
