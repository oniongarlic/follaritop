#include "cutefollari.h"

#include <QList>

/**
id	"nb_7081294"
stop_code	"2"
operator	"nextbike"
name	"Linna "
last_seen	1527673198
lon	22.230363
lat	60.435908
bikes_avail	7
slots_total	16
slots_avail	9
*/

#define API_URL "http://data.foli.fi/citybike"

CuteFollari::CuteFollari(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this)),
    m_timer(this),
    is_polling(false),
    m_lastupdate(0)
{
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(loadData()));
}

void CuteFollari::startPolling()
{
    m_timer.setInterval(5000); // 5 seconds
    m_timer.setSingleShot(true);
    m_timer.start();
    is_polling=true;
}

FollariRack *CuteFollari::getRack(QString id)
{
    if (m_racks.contains(id))
        return m_racks.value(id);

    return nullptr;
}

const QList<FollariRack *> CuteFollari::getRacks()
{
    return m_racks.values();
}

void CuteFollari::loadData()
{
    QUrl url(API_URL);
    QNetworkRequest request;

    request.setUrl(url);

    get(request);
}

QNetworkReply *CuteFollari::get(QNetworkRequest &request)
{
    QNetworkReply *reply;

    reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(requestFinished()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(requestError(QNetworkReply::NetworkError)));

    return reply;
}

/**
 * @brief CuteFollari::parseJsonResponse
 * @param data
 * @return
 */
QVariantMap CuteFollari::parseJsonResponse(const QByteArray &data)
{
    uint utmp;
    QVariantMap map;
    QJsonDocument json=QJsonDocument::fromJson(data);

    if (json.isEmpty() || json.isNull() || !json.isObject()) {
        qWarning("API gave invalid JSON!");
        return map;
    }

    map=json.object().toVariantMap();

    utmp=map["lastupdate"].toUInt();
    if (utmp!=m_lastupdate) {
        m_lastupdate=utmp;
        m_lastupdateDateTime.setTime_t(m_lastupdate);
    } else {
        return map;
    }

    utmp=map["bikes_total_avail"].toUInt();
    if (utmp!=m_bikesAvailable) {
        m_bikesAvailable=utmp;
        emit bikesAvailableChanged(m_bikesAvailable);
    }

    m_stations=map["racks"].toMap();
    m_racks_count=m_stations.count();

    QMapIterator<QString, QVariant> i(m_stations);
    while (i.hasNext()) {
        i.next();

        QString k=i.key();
        QVariantMap r=i.value().toMap();
        FollariRack *fr;

        if (m_racks.contains(k)) {
            fr=m_racks.value(k);
        } else {
            fr=new FollariRack(this);
            m_racks.insert(k, fr);
        }
        fr->updateFromVariantMap(r);
    }   

    emit updated();    

    return map;
}

void CuteFollari::parseResponse(QNetworkReply *reply)
{
    QNetworkReply::NetworkError e = reply->error();

    if (e!=QNetworkReply::NoError)
        qWarning() << "Request error: " << e;

    const QByteArray data = reply->readAll();
    int hc=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch (hc) {
    case 200:
    case 201:
           parseJsonResponse(data);
        break;
    case 401:
    case 403:
        qWarning("Authentication error");
        emit error(hc);
        break;
    case 400:
        qWarning("Invalid request failure");
        emit error(hc);
        break;
    case 404:
        qWarning("Not found failure");
        emit error(hc);
        break;
    case 500:
        qWarning("Server failure");
        emit error(hc);
        break;
    case 0:
        qWarning("Generic network failure");
        emit error(hc);
        break;
    default: {
        qWarning() << "Unexpected and unhandled response code: " << hc;
        emit error(hc);
        break;
    }
    }
}

void CuteFollari::requestFinished() {
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

    parseResponse(reply);

    if (is_polling) {
        m_timer.start();
    }

    reply->deleteLater();
}

void CuteFollari::requestError(QNetworkReply::NetworkError e)
{
    qWarning() << "Network error" << e;

    m_timer.stop();
    is_polling=false;
}
