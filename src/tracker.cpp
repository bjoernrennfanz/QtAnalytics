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

#include "tracker.h"
#include "analyticsmanager.h"

QTANALYTICS_NAMESPACE_USING

CTracker::CTracker(QString& propertyId, IPlatformInfo* pPlatformInfo, IAnalyticsManager* pAnalyticsManager)
    : AnonymizeIP(false)
    , m_pAnalyticsManager(pAnalyticsManager)
    , m_pPlatformInfo(pPlatformInfo)
    , m_propertyId(propertyId)
{
    if (pPlatformInfo)
    {
        ClientId = pPlatformInfo->getAnonymousClientId();
        ScreenColors = pPlatformInfo->getScreenColors();
        ScreenResolution = pPlatformInfo->getScreenResolution();
        ViewportSize = pPlatformInfo->getViewPortResolution();

        connect(pPlatformInfo, &IPlatformInfo::screenResolutionChanged, this, &CTracker::onScreenResolutionChanged);
        connect(pPlatformInfo, &IPlatformInfo::viewPortResolutionChanged, this, &CTracker::onViewPortResolutionChanged);
    }
}

QString CTracker::getPropertyId()
{
    return m_propertyId;
}

QString CTracker::getValue(QString &key)
{
    return m_data.value(key);
}

void CTracker::setValue(QString &key, QString &value)
{
    if (m_data.contains(key))
    {
        m_data.take(key);
    }

    m_data.insert(key, value);
}

void CTracker::send(QMap<QString, QString> params)
{
    m_pAnalyticsManager->enqueueHit(addRequiredHitData(params));
}

void CTracker::onViewPortResolutionChanged()
{
    ViewportSize = m_pPlatformInfo->getViewPortResolution();
}

void CTracker::onScreenResolutionChanged()
{
    ScreenResolution = m_pPlatformInfo->getScreenResolution();
}

QMap<QString, QString> CTracker::addRequiredHitData(QMap<QString, QString> &params)
{
    QMap<QString, QString> result;

    result.insert("v", "1");
    result.insert("tid", getPropertyId());
    result.insert("cid", ClientId);
    result.insert("an", AppName);
    result.insert("av", AppVersion);

    if (!AppId.isEmpty()) result.insert("aid", AppId);
    if (!AppInstallerId.isEmpty()) result.insert("aiid", AppInstallerId);
    if (!ScreenName.isEmpty()) result.insert("cd", ScreenName);

    if (AnonymizeIP) result.insert("aip", "1");

    if (!qFuzzyCompare(ScreenResolution.Width, 0) && !qFuzzyCompare(ScreenResolution.Height, 0))
        result.insert("sr", QString("%1x%2").arg(ScreenResolution.Width).arg(ScreenResolution.Height));
    if (!qFuzzyCompare(ViewportSize.Width, 0) && !qFuzzyCompare(ViewportSize.Height, 0))
        result.insert("vp", QString("%1x%2").arg(ViewportSize.Width).arg(ViewportSize.Height));
    if (ScreenColors) result.insert("sd", QString("%1-bits").arg(ScreenColors));

    if (!Language.isEmpty()) result.insert("ul", Language);
    if (!Encoding.isEmpty()) result.insert("de", Encoding);

    if (!IpOverride.isEmpty()) result.insert("uip", IpOverride);
    if (!UserAgentOverride.isEmpty()) result.insert("ua", UserAgentOverride);
    if (!LocationOverride.isEmpty()) result.insert("geoid", LocationOverride);

    for(QMap<QString, QString>::const_iterator it = m_data.begin(), end = m_data.end(); it != end; ++it)
    {
        result.insert(it.key(), it.value());
    }

    for(QMap<QString, QString>::const_iterator it = params.begin(), end = params.end(); it != end; ++it)
    {
        result.insert(it.key(), it.value());
    }

    return result;
}
