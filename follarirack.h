#ifndef FOLLARIRACK_H
#define FOLLARIRACK_H

#include <QObject>
#include <QDateTime>
#include <QGeoCoordinate>
#include <QVariantMap>
#include <QDebug>

class FollariRack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint bikesAvailable READ bikesAvailable NOTIFY bikesAvailableChanged)
    Q_PROPERTY(uint slotsAvailable READ slotsAvailable NOTIFY slotsAvailableChanged)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen NOTIFY lastSeenChanged)
public:
    FollariRack(QObject *parent=nullptr);
    bool updateFromVariantMap(const QVariantMap &r);

    uint bikesAvailable() const
    {
        return m_bikes_avail;
    }

    QString stopCode() const{
        return m_stop_code;
    }

    QString stopName() const{
        return m_name;
    }

    uint slotsAvailable() const
    {
        return m_slots_avail;
    }

    uint slotsTotal() const
    {
        return m_slots_total;
    }

    QDateTime lastSeen() const
    {
        return m_last_seen;
    }

    static bool compareStopCode(const FollariRack *fr1, const FollariRack *fr2);
    static bool compareBikes(const FollariRack *fr1, const FollariRack *fr2);
signals:
    void bikesAvailableChanged(uint bikesAvailable);
    void slotsAvailableChanged(uint slotsAvailable);
    void lastSeenChanged(QDateTime lastSeen);

private:
    QString m_stop_code;
    QString m_name;
    qint64 m_last_update;
    QDateTime m_last_seen;
    uint m_bikes_avail;
    uint m_slots_total;
    uint m_slots_avail;
    QGeoCoordinate m_geo;
};


#endif // FOLLARIRACK_H
