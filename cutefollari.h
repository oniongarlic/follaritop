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

#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>

#include "follarirack.h"

class CuteFollari : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint bikesAvailable READ getBikesAvailable NOTIFY bikesAvailableChanged)

public:
    explicit CuteFollari(QObject *parent = nullptr);

    void startPolling();
    uint getBikesAvailable() const
    {
        return m_bikesAvailable;
    }
    const QDateTime getUpdateDateTime() {
        return m_lastupdateDateTime;
    }
    FollariRack *getRack(QString id);
    const QList<FollariRack *> getRacks();

signals:
    void bikesAvailableChanged(uint bikesAvailable);
    void updated();
    void error(int code);
    //void

public slots:

private slots:
    void requestFinished();
    void requestError(QNetworkReply::NetworkError e);
    void loadData();

private:
    QNetworkAccessManager *m_manager;
    QTimer m_timer;
    QVariantMap m_stations;
    QVariantMap parseJsonResponse(const QByteArray &data);
    QNetworkReply *get(QNetworkRequest &request);
    void parseResponse(QNetworkReply *reply);

    bool is_polling;
    uint m_racks_count;
    uint m_bikesAvailable;
    uint m_lastupdate;
    QDateTime m_lastupdateDateTime;    

    QMap<QString, FollariRack *> m_racks;
};

#endif // CUTEFOLLARI_H
