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
    //m_accessManager(QNetworkAccessManager(parent)),
    m_reply(nullptr),
    m_codeReturned(QByteArray())
{
    connect(&m_accessManager, &QNetworkAccessManager::authenticationRequired, this, &HttpRequestHandler::onAuthentificationRequired);
    //connect(&m_accessManager, &QNetworkAccessManager::sslErrors, this, &HttpRequestHandler::onSslError_AccessManager);
}

HttpRequestHandler::~HttpRequestHandler()
{
    if(m_reply != nullptr)
    {
        delete m_reply;
        m_reply = nullptr;
    }
}

void HttpRequestHandler::setRawsHeader(QList<RawHeader> rawsHeader)
{
    m_rawsHeader = rawsHeader;
}

QByteArray HttpRequestHandler::requestGet(QString url)
{
    QUrl urlRequest = QUrl(url);
    if(!urlRequest.isValid())
        qCritical() << __PRETTY_FUNCTION__ << "l'URL n'est pas valide";

    QNetworkRequest netRequest = QNetworkRequest(urlRequest);
    foreach(RawHeader data, m_rawsHeader)
        netRequest.setRawHeader(data.name, data.value);

    qDebug() << __PRETTY_FUNCTION__ << "rawHeader=" << netRequest.rawHeaderList();

    /*QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    netRequest.setSslConfiguration(config);*/
    //netRequest.setHeader(QNetworkRequest::ServerHeader, "application/json");
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    netRequest.setHeader(QNetworkRequest::ContentLengthHeader, 47);
    netRequest.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36");

    //connect(&m_accessManager, &QNetworkAccessManager::finished, this, &HttpRequestHandler::onFinished_NetworkReply);

    m_reply = m_accessManager.get(netRequest);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &HttpRequestHandler::onDownloadProgress);
    //connect(m_reply, &QNetworkReply::readyRead, this, &HttpRequestHandler::onReadyRead_NetworkReply);
    //connect(m_reply, &QNetworkReply::finished, this, &HttpRequestHandler::onFinished_NetworkReply);

    qDebug() << __PRETTY_FUNCTION__ << "requête partie";
    //connect(m_reply, &QNetworkReply::error, this, &HttpRequestHandler::onError_NetworkReply);

    QEventLoop loop;
    connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray codeReturned = m_reply->readAll();
    delete m_reply;
    m_reply = nullptr;

    return codeReturned;
}

QByteArray HttpRequestHandler::requestPost(QString url, QByteArray body)
{
    QUrl urlRequest = QUrl(url);
    if(!urlRequest.isValid())
        qCritical() << __PRETTY_FUNCTION__ << "l'URL n'est pas valide";

    QNetworkRequest netRequest = QNetworkRequest(urlRequest);
    foreach(RawHeader data, m_rawsHeader)
        netRequest.setRawHeader(data.name, data.value);

    netRequest.setHeader(QNetworkRequest::ServerHeader, "application/json");

    m_reply = m_accessManager.post(netRequest, body);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &HttpRequestHandler::onDownloadProgress);

    QEventLoop loop;
    connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray codeReturned = m_reply->readAll();
    delete m_reply;
    m_reply = nullptr;

    return codeReturned;
}

void HttpRequestHandler::onAuthentificationRequired(QNetworkReply *, QAuthenticator *authenticator)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void HttpRequestHandler::onReadyRead_NetworkReply()
{
    qDebug() << "Reply" << m_reply->readAll();
}

void HttpRequestHandler::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << __PRETTY_FUNCTION__ << bytesReceived << "/" << bytesTotal;
}

void HttpRequestHandler::onFinished_NetworkReply()
{
    //qDebug() << __PRETTY_FUNCTION__ << "Local" << reply->readAll();
    qDebug() << __PRETTY_FUNCTION__ << "Global" << m_reply->readAll();
}

void HttpRequestHandler::onError_NetworkReply(QNetworkReply::NetworkError code)
{
    qDebug() << __PRETTY_FUNCTION__ << code;
}

/*void HttpRequestHandler::onSslError_AccessManager(QNetworkReply *, const QList<QSslError> &errors)
{
    qDebug() << "Error SSL";

    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }

    qDebug() << "Error SSL" << errorString;
    //m_reply->ignoreSslErrors();
}*/
