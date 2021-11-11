#ifndef CARRYTRADE_H
#define CARRYTRADE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>

class CarryTrade : public QObject
{
    Q_OBJECT
public:
    explicit CarryTrade(QObject *parent = nullptr);
    QJsonObject getJSONfromURL(QString URL, QString authHeader = "", QString authToken = "");

signals:

};

#endif // CARRYTRADE_H
