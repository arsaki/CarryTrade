#include "carrytrade.h"

CarryTrade::CarryTrade(QObject *parent) : QObject(parent)
{

}

QJsonObject CarryTrade::getJSONfromURL(QString URL, QString authHeader, QString authToken)
{
    QUrl url = QUrl::fromUserInput(URL);
    QNetworkRequest  request(url);
    request.setRawHeader("Accept","application/json");
    if (authHeader != "" && authToken != "")
        request.setRawHeader(authHeader.toLatin1(), authToken.toLatin1());
    QNetworkAccessManager networkaccess;
    QNetworkReply * reply = networkaccess.get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray buffer;
    buffer.append(reply->readAll());
    delete reply;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(buffer);
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
