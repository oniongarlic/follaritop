#include "cutefollari.h"

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
    is_polling(false)
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
    //QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(requestError(QNetworkReply::NetworkError)));

    return reply;
}

void CuteFollari::printStations()
{
    printf("\e[1;1H\e[2J");

    printf("Available bikes: %3d\n", m_bikesAvailable);
    printf(" ID  Name                              Bikes      Slots       Available\n");

    QMapIterator<QString, QVariant> i(m_stations);
    while (i.hasNext()) {
        i.next();

        QVariantMap r=i.value().toMap();

        QString stop_code=r["stop_code"].toString();
        QString name=r["name"].toString();
        QDateTime last_seen;
        last_seen.setMSecsSinceEpoch(r["last_seen"].toInt());

        //lon	22.230363
        //lat	60.435908
        uint bikes_avail=r["bikes_avail"].toUInt();
        uint slots_total=r["slots_total"].toUInt();
        uint slots_avail=r["slots_avail"].toUInt();

        QString p=QString("%1 %2: [%3] [%4] [%5]").arg(stop_code,3).arg(name, 32).arg(bikes_avail,8).arg(slots_total,8).arg(slots_avail,8);

        printf("%s %s\n", p.toUtf8().constData(), bikes_avail<3 ? "!" : "");

//        printf("%-3ls %-30ls: [%8d] [%8d] [%8d]\n", stop_code.utf16(),
//               name.toUtf8().constData(),
//               bikes_avail,
//               slots_total,
//               slots_avail);
    }
}

/**
 * @brief CuteFollari::parseJsonResponse
 * @param data
 * @return
 */
QVariantMap CuteFollari::parseJsonResponse(const QByteArray &data)
{
    QVariantMap map;
    QJsonDocument json=QJsonDocument::fromJson(data);

    if (json.isEmpty() || json.isNull() || !json.isObject()) {
        qWarning("API gave invalid JSON!");
        return map;
    }

    map=json.object().toVariantMap();
    m_bikesAvailable=map["bikes_total_avail"].toInt();
    m_stations=map["racks"].toMap();

    printStations();

    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();

        QVariantMap tmp=i.value().toMap();

        //qDebug() << tmp;
    }

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
        //parseErrorResponse(hc, op, data);
        break;
    case 400:
        qWarning("Invalid request failure");
        //parseErrorResponse(hc, op, data);
        break;
    case 404:
        qWarning("Not found failure");
        //parseErrorResponse(hc, op, data);
        break;
    case 500:
        qWarning("Server failure");
        //parseErrorResponse(hc, op, data);
        break;
    case 0:
        qWarning("Generic network failure");
        //emit requestFailure(hc, "Network error "+reply->errorString());
        break;
    default: {
        qWarning() << "Unexpected and unhandled response code: " << hc;

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