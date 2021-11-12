#include "carrytrade.h"
#include <iostream>
#include <cmath>
#define ESC_CODE_CLEAR_SCREEN "\u001b[2J"

void CarryTrade::getBaseQuoteAssetsBinance(CryptoPairsData &binanceData)
{
    //Getting data about pairs
    QJsonArray jsonArray = getJSONfromURL("https://api.binance.com/api/v3/exchangeInfo").object()["symbols"].toArray();
    int size = jsonArray.size();
    int inTrading = 0;
    for (int counter = 0; counter < size; counter++)
    {
        QString ticker = jsonArray[counter].toObject()["symbol"].toString();
        if (jsonArray[counter].toObject()["status"].toString() == "TRADING")
        {
            TickerData& tickerData = binanceData[ticker] ;
            tickerData.baseAsset = jsonArray[counter].toObject()["baseAsset"].toString();
            tickerData.quoteAsset = jsonArray[counter].toObject()["quoteAsset"].toString();
            if (tickerData.baseAsset == "" || tickerData.quoteAsset == "")
            {
                std::cout << "Ошибка. Отсутствуют данные Base or Quote Asset по " << ticker.toStdString() << "." << std::endl;
                binanceData.remove(ticker);
            }
            else
                inTrading++;
        }
    }
    std::cout << "Получены Base&Quote Assets по " << std::to_string(inTrading) << " парам." << std::endl;
}

void CarryTrade::getAskBidPricesBinance(CryptoPairsData &binanceData)
{
    //Getting ask and bid prices
    QJsonArray jsonArray = getJSONfromURL("https://api.binance.com/api/v3/ticker/bookTicker").array();
    int gettedAskBidPrices = 0;
    int size = jsonArray.size();
    for (int counter = 0; counter < size; counter++)
    {
        QString ticker = jsonArray[counter].toObject()["symbol"].toString();
        if (binanceData.contains(ticker))
        {
            TickerData& tickerData = binanceData[ticker] ;
            tickerData.askPrice = jsonArray[counter].toObject()["askPrice"].toString().toDouble();
            tickerData.bidPrice = jsonArray[counter].toObject()["bidPrice"].toString().toDouble();
            binanceData[ticker] = tickerData;
            if (qFpClassify(tickerData.askPrice) == FP_ZERO || qFpClassify(tickerData.bidPrice) == FP_ZERO)
            {
                std::cout << "Отсутствуют Ask&Bid Prices по " << ticker.toStdString() << "." << std::endl;
                binanceData.remove(ticker);
            }
            else
                gettedAskBidPrices++;
        }
    }
    std::cout << "Получены Ask Bid Prices по " << std::to_string(gettedAskBidPrices) << " парам." << std::endl;
}

CarryTrade::CarryTrade(QObject *parent) : QObject(parent)
{
    getBaseQuoteAssetsBinance(binanceData);
    getAskBidPricesBinance(binanceData);
}





QJsonDocument CarryTrade::getJSONfromURL(QString URL, QString authHeader, QString authToken)
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
    return jsonDocument;
}
