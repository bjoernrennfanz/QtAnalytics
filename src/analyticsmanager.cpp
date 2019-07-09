/*
 * Copyright (C) 2019 Björn Rennfanz (bjoern@fam-rennfanz.de)
 *
 * This file is part of QtAnalytics (https://github.com/bjoernrennfanz/QtAnalytics)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "analyticsmanager.h"
#include "platforminfo.h"

#include <QUrlQuery>
#include <QSettings>
#include <QApplication>

#include <QRandomGenerator>

#include <QNetworkReply>
#include <QNetworkRequest>

QTANALYTICS_NAMESPACE_USING

QString CAnalyticsManager::m_keyAppOptOut = "AppOptOut";
CAnalyticsManager* CAnalyticsManager::m_pInstance = Q_NULLPTR;

QString CAnalyticsManager::m_endPointUnsecureDebug = QString("http://www.google-analytics.com/debug/collect");
QString CAnalyticsManager::m_endPointSecureDebug = QString("https://ssl.google-analytics.com/debug/collect");
QString CAnalyticsManager::m_endPointUnsecure = QString("http://www.google-analytics.com/collect");
QString CAnalyticsManager::m_endPointSecure = QString("https://ssl.google-analytics.com/collect");

CAnalyticsManager::CAnalyticsManager(IPlatformInfo* pPlatformInfo, QObject* pParent)
    : QObject(pParent)
    , m_autoTrackNetworkConnectivity(false)
    , m_isAppOptOutSet(false)
    , m_appOptOut(false)
    , m_pPlatformInfo(pPlatformInfo)
    , m_pNetworkConfigurationManager(new QNetworkConfigurationManager(this))
    , m_pNetworkAccessManager(new QNetworkAccessManager(this))
    , m_pDefaultTracker(Q_NULLPTR)
    , m_isSending(false)
{
    // Setup default values
    IsEnabled = true;
    IsSecure = true;
    IsDebug = false;
    PostData = true;
    BustCache = false;

    // Connect internal signals
    connect(this, &CAnalyticsManager::sendNextHit, this, &CAnalyticsManager::onSendHit);
}

CAnalyticsManager::~CAnalyticsManager()
{
    if (m_pPlatformInfo)
    {
        m_pPlatformInfo->deleteLater();
    }
}

CAnalyticsManager* CAnalyticsManager::current()
{
    if (!m_pInstance)
    {
        m_pInstance = new CAnalyticsManager(new CPlatformInfo());
    }

    return m_pInstance;
}

bool CAnalyticsManager::autoTrackNetworkConnectivity()
{
    return m_autoTrackNetworkConnectivity;
}

void CAnalyticsManager::setAutoTrackNetworkConnectivity(bool &value)
{
    if (m_autoTrackNetworkConnectivity != value)
    {
        m_autoTrackNetworkConnectivity = value;
        if (m_autoTrackNetworkConnectivity)
        {
            updateConnectionStatus();

            // Connect needed signals
            connect(m_pNetworkConfigurationManager, &QNetworkConfigurationManager::onlineStateChanged, this, &CAnalyticsManager::onOnlineStateChanged);
        }
        else
        {
            // Disconnect needed signals
            disconnect(m_pNetworkConfigurationManager, &QNetworkConfigurationManager::onlineStateChanged, this, &CAnalyticsManager::onOnlineStateChanged);
            IsEnabled = true;
        }
    }
}

CTracker* CAnalyticsManager::defaultTracker()
{
    return m_pDefaultTracker;
}

bool CAnalyticsManager::appOptOut()
{
    if (!m_isAppOptOutSet)
    {
        loadAppOptOut();
    }

    return m_appOptOut;
}

void CAnalyticsManager::setAppOptOut(bool &value)
{
    m_appOptOut = value;
    m_isAppOptOutSet = true;

    // Persist into registry
    QSettings settings;
    settings.beginGroup("GoogleAnaltyics");
    settings.setValue(m_keyAppOptOut, value);
    settings.endGroup();
}

CTracker* CAnalyticsManager::createTracker(QString propertyId)
{
    if (m_trackers.find(propertyId) == m_trackers.end())
    {
        CTracker* tracker = new CTracker(propertyId, m_pPlatformInfo, this);
        tracker->AppName = QApplication::applicationName();
        tracker->AppVersion = QApplication::applicationVersion();

        m_trackers.insert(propertyId, tracker);
        if (!m_pDefaultTracker)
        {
            m_pDefaultTracker = tracker;
        }

        return tracker;
    }
    else
    {
        return m_trackers[propertyId];
    }
}

void CAnalyticsManager::closeTracker(CTracker* pTracker)
{
    m_trackers.remove(pTracker->getPropertyId());
    if (m_pDefaultTracker == pTracker)
    {
        m_pDefaultTracker = Q_NULLPTR;
    }
}

IPlatformInfo* CAnalyticsManager::platformInfoProvider()
{
    return m_pPlatformInfo;
}

void CAnalyticsManager::updateConnectionStatus()
{
    IsEnabled = m_pNetworkConfigurationManager->isOnline();
}

void CAnalyticsManager::loadAppOptOut()
{
    QSettings settings;
    settings.beginGroup("GoogleAnaltyics");

    m_appOptOut = settings.value(m_keyAppOptOut, false).toBool();
    m_isAppOptOutSet = true;

    settings.endGroup();
}

QString CAnalyticsManager::getCacheBuster()
{
    quint32 seed = QRandomGenerator::global()->generate();
    QString seedString = QString::number(seed).leftJustified(9, '0', true);

    return seedString;
}

void CAnalyticsManager::onOnlineStateChanged(bool isOnline)
{
    Q_UNUSED(isOnline)
    updateConnectionStatus();
}

void CAnalyticsManager::enqueueHit(const QMap<QString, QString> &params)
{
    if (!appOptOut())
    {
        CHit hit(params);
        m_hitQueue.append(hit);

        if (!m_isSending)
        {
            emit sendNextHit();
        }
    }
}

void CAnalyticsManager::onSendHit()
{
    if (m_hitQueue.isEmpty())
    {
        m_isSending = false;
        return;
    }
    else
    {
        m_isSending = true;
    }

    // Select correct endpoint
    QString endPoint = IsDebug ? (IsSecure ? m_endPointSecureDebug : m_endPointUnsecureDebug) : (IsSecure ? m_endPointSecure : m_endPointUnsecure);

    // Build query parameters
    QUrlQuery query;

    // Get first element from queue
    CHit hit = m_hitQueue.head();
    QDateTime sendTime = QDateTime::currentDateTime();
    qint64 timeDiff = hit.getTimeStamp().msecsTo(sendTime);
    query.addQueryItem("qt", QString::number(timeDiff));

    if (BustCache)
    {
        query.addQueryItem("z", getCacheBuster());
    }

    // Clone the data
    QMap<QString, QString> data = hit.getData();
    for(QMap<QString, QString>::const_iterator it = data.begin(), end = data.end(); it != end; ++it)
    {
        query.addQueryItem(it.key(), it.value());
    }

    if (PostData)
    {
        // Prepare network request for post
        QNetworkRequest request(endPoint);
        request.setHeader(QNetworkRequest::UserAgentHeader, m_pPlatformInfo->getUserAgent());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QByteArray ba;
        ba = query.query(QUrl::FullyEncoded).toUtf8();
        request.setHeader(QNetworkRequest::ContentLengthHeader, ba.length());

        QNetworkReply* reply = m_pNetworkAccessManager->post(request, ba);
        connect(reply, &QNetworkReply::finished, this, &CAnalyticsManager::onSendHitFinished);
    }
    else
    {
        // Perform get request
        QNetworkRequest request(endPoint + "?" + query.query(QUrl::FullyEncoded));
        request.setHeader(QNetworkRequest::UserAgentHeader, m_pPlatformInfo->getUserAgent());

        QNetworkReply* reply = m_pNetworkAccessManager->get(request);
        connect(reply, &QNetworkReply::finished, this, &CAnalyticsManager::onSendHitFinished);
    }
}

void CAnalyticsManager::onSendHitFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    int httpStausCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStausCode < 200 || httpStausCode > 299)
    {
        qDebug() << "[QtAnalytics]" << QString("Error sending message: %1").arg(reply->errorString());

        // An error ocurred.
        m_isSending = false;
        return;
    }
    else
    {
        qDebug() << "[QtAnalytics]" << "Message sent";
    }

    m_hitQueue.dequeue();
    emit sendNextHit();
}
