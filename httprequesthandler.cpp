#include "httprequesthandler.h"

#include <QDebug>
#include <QEventLoop>

#include <QByteArray>
#include <QMessageBox>

#include <QNetworkRequest>

HttpRequestHandler::HttpRequestHandler(QObject *parent) :
    QObject(parent),
    m_url(QUrl()),
    m_rawsHeader(QList<RawHeader>()),
    m_reply(nullptr),
    m_codeReturned(QByteArray())
{
}

HttpRequestHandler::~HttpRequestHandler()
{
    if(m_reply != nullptr)
    {
        delete m_reply;
        m_reply = nullptr;
    }
}

/******************************************
 ***        FONCTIONS PUBLIQUES         ***
 ******************************************/
void HttpRequestHandler::setRawsHeader(QList<RawHeader> rawsHeader)
{
    m_rawsHeader = rawsHeader;
}

QByteArray HttpRequestHandler::requestGet(QString url)
{
    //check the url
    QUrl urlRequest = QUrl(url);
    if(!urlRequest.isValid())
        qCritical() << __PRETTY_FUNCTION__ << "l'URL n'est pas valide";

    //set header
    QNetworkRequest netRequest = QNetworkRequest(urlRequest);
    foreach(RawHeader data, m_rawsHeader)
        netRequest.setRawHeader(data.name, data.value);

    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //set ssl configuration
    QSslConfiguration config = netRequest.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    netRequest.setSslConfiguration(config);

    //request
    m_reply = m_accessManager.get(netRequest);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &HttpRequestHandler::onDownloadProgress);

    //blocking until answer
    QEventLoop loop;
    connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //check error
    QByteArray codeReturned;
    if(m_reply->error() != QNetworkReply::NoError)
    {
        codeReturned = "Error: " + m_reply->errorString().toLatin1() + "\n";
    }

    //read data
    codeReturned += m_reply->readAll();

    //clean up
    delete m_reply;
    m_reply = nullptr;

    return codeReturned;
}

QByteArray HttpRequestHandler::requestPost(QString url, QByteArray body)
{
    //check the url
    QUrl urlRequest = QUrl(url);
    if(!urlRequest.isValid())
        qCritical() << __PRETTY_FUNCTION__ << "l'URL n'est pas valide";

    //set header
    QNetworkRequest netRequest = QNetworkRequest(urlRequest);
    foreach(RawHeader data, m_rawsHeader)
        netRequest.setRawHeader(data.name, data.value);

    //set ssl configuration
    QSslConfiguration config = netRequest.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    netRequest.setSslConfiguration(config);
    netRequest.setHeader(QNetworkRequest::ServerHeader, "application/json");

    //request
    m_reply = m_accessManager.post(netRequest, body);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &HttpRequestHandler::onDownloadProgress);

    //blocking until answer
    QEventLoop loop;
    connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //check error
    QByteArray codeReturned;
    if(m_reply->error() != QNetworkReply::NoError)
    {
        codeReturned = "Error: " + m_reply->errorString().toLatin1() + "\n";
    }

    //read data
    codeReturned += m_reply->readAll();

    //clean up
    delete m_reply;
    m_reply = nullptr;

    return codeReturned;
}

/******************************************
 ***       FONCTIONS SLOT PRIVEES       ***
 ******************************************/
void HttpRequestHandler::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << __PRETTY_FUNCTION__ << bytesReceived << "/" << bytesTotal;
}

