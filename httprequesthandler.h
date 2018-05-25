#ifndef HTTPREQUESTHANDLER_H
#define HTTPREQUESTHANDLER_H

#include <QObject>

#include "common.h"

#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class QNetworkReply;

class HttpRequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequestHandler(QObject *parent = nullptr);
    ~HttpRequestHandler();

    void setRawsHeader(QList<RawHeader> rawsHeader);

    QByteArray requestGet(QString url);
    QByteArray requestPost(QString url, QByteArray body);

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QUrl m_url;
    QList<RawHeader> m_rawsHeader;
    QNetworkAccessManager m_accessManager;
    QNetworkReply *m_reply;
    QByteArray m_codeReturned;
};

#endif // HTTPREQUESTHANDLER_H
