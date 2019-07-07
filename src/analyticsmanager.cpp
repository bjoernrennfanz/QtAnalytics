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

QTANALYTICS_NAMESPACE_USING

QString m_keyAppOptOut;
CAnalyticsManager* CAnalyticsManager::m_pInstance = Q_NULLPTR;

QString CAnalyticsManager::m_endPointUnsecureDebug = QString("http://www.google-analytics.com/debug/collect");
QString CAnalyticsManager::m_endPointSecureDebug = QString("https://ssl.google-analytics.com/debug/collect");
QString CAnalyticsManager::m_endPointUnsecure = QString("http://www.google-analytics.com/collect");
QString CAnalyticsManager::m_endPointSecure = QString("https://ssl.google-analytics.com/collect");

CAnalyticsManager::CAnalyticsManager(IPlatformInfo* pPlatformInfo, QObject* pParent)
    : QObject(pParent)
    , m_isAppOptOutSet(false)
    , m_pPlatformInfo(pPlatformInfo)
    , m_pNetworkConfigurationManager(new QNetworkConfigurationManager(this))
    , m_pDefaultTracker(Q_NULLPTR)
{
    // Setup default values
    AppOptOut = false;
    IsEnabled = true;
    IsSecure = true;
    PostData = true;
    BustCache = false;

    // Connect needed signals
    connect(m_pNetworkConfigurationManager, &QNetworkConfigurationManager::onlineStateChanged, this, &CAnalyticsManager::onOnlineStateChanged);
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

CTracker* CAnalyticsManager::defaultTracker()
{
    return m_pDefaultTracker;
}

CTracker* CAnalyticsManager::createTracker(QString propertyId)
{
    return Q_NULLPTR;
}

void CAnalyticsManager::closeTracker(CTracker* pTracker)
{

}

IPlatformInfo* CAnalyticsManager::platformInfoProvider()
{
    return m_pPlatformInfo;
}

void CAnalyticsManager::updateConnectionStatus()
{

}

void CAnalyticsManager::loadAppOptOut()
{

}

QString CAnalyticsManager::getCacheBuster()
{
    return QString("");
}

void CAnalyticsManager::onOnlineStateChanged(bool isOnline)
{

}

void CAnalyticsManager::enqueueHit(const QMap<QString, QString> &params)
{

}
