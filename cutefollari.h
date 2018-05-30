#ifndef CUTEFOLLARI_H
#define CUTEFOLLARI_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QHttpPart>

#include <QTimer>
#include <QTimerEvent>

#include <QMap>
#include <QUrl>

#include <QStringListModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>

class CuteFollari : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint bikesAvailable READ getBikesAvailable NOTIFY bikeAvailableChanged)

public:
    explicit CuteFollari(QObject *parent = nullptr);

    void startPolling();
    uint getBikesAvailable() const
    {
        return m_bikesAvailable;
    }

signals:

    void bikeAvailableChanged(uint bikesAvailable);

public slots:

private slots:
    void requestFinished();
    void loadData();
private:
    QNetworkAccessManager *m_manager;
    QTimer m_timer;
    QVariantMap m_stations;
    QVariantMap parseJsonResponse(const QByteArray &data);
    QNetworkReply *get(QNetworkRequest &request);
    void parseResponse(QNetworkReply *reply);

    bool is_polling;
    uint m_bikesAvailable;
    uint m_lastupdate;
    QDateTime m_lastupdateDateTime;
    void printStations();
};

#endif // CUTEFOLLARI_H
