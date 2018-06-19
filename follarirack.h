#ifndef FOLLARIRACK_H
#define FOLLARIRACK_H

#include <QObject>
#include <QDateTime>
#include <QGeoCoordinate>
#include <QVariantMap>

class FollariRack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint bikesAvailable READ bikesAvailable NOTIFY bikesAvailableChanged)
public:
    FollariRack(QObject *parent=0);
    bool updateFromVariantMap(const QVariantMap r);

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

signals:
    void bikesAvailableChanged(uint bikesAvailable);

private:
    QString m_stop_code;
    QString m_name;
    QDateTime m_last_seen;
    uint m_bikes_avail;
    uint m_slots_total;
    uint m_slots_avail;
    QGeoCoordinate m_geo;
};

#endif // FOLLARIRACK_H
