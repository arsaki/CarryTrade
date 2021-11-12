#ifndef CARRYTRADE_H
#define CARRYTRADE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTimer>

struct TickerData
{
    double askPrice;
    double bidPrice;
    QString baseAsset;
    QString quoteAsset;
};
typedef QMap<QString, TickerData> CryptoPairsData;

class CarryTrade : public QObject
{
    Q_OBJECT
    CryptoPairsData binanceData;     //<Ticker, {Ask,Bid,baseAsset,quoteAsset}>
    void getBaseQuoteAssetsBinance(CryptoPairsData & binanceData);
    void getAskBidPricesBinance(CryptoPairsData & binanceData);
public:
    explicit CarryTrade(QObject *parent = nullptr);
    QJsonDocument getJSONfromURL(QString URL, QString authHeader = "", QString authToken = "");

signals:

};

#endif // CARRYTRADE_H
