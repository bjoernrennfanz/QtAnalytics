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

#pragma once

#include "qtanalytics_global.h"
#include "tracker.h"
#include "ianalyticsmanager.h"
#include "iplatforminfo.h"
#include "hit.h"

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkConfigurationManager>

QTANALYTICS_NAMESPACE_BEGIN

class CAnalyticsManager : public QObject, public IAnalyticsManager
{
    Q_OBJECT
    Q_PROPERTY(bool autoTrackNetworkConnectivity MEMBER AutoTrackNetworkConnectivity)
    Q_PROPERTY(bool appOptOut MEMBER AppOptOut)
    Q_PROPERTY(bool isSecure MEMBER IsSecure)
    Q_PROPERTY(bool isDebug MEMBER IsDebug)
    Q_PROPERTY(bool isEnabled MEMBER IsEnabled)
    Q_PROPERTY(bool postData MEMBER PostData)
    Q_PROPERTY(bool bustCache MEMBER BustCache)

public:
    CAnalyticsManager(IPlatformInfo* pPlatformInfo, QObject* pParent = Q_NULLPTR);
    virtual ~CAnalyticsManager();

    ///
    /// \brief Shared, singleton instance of CAnalyticsManager.
    ///
    static CAnalyticsManager* current();

    ///
    /// \brief Enables (when set to true) listening to network connectivity events
    ///        to have trackers behave accordingly to their connectivity status.
    ///
    bool AutoTrackNetworkConnectivity;

    CTracker* defaultTracker();

    ///
    /// \brief True when the user has opted out of analytics, this disables
    ///        all tracking activities.
    ///
    bool AppOptOut;

    ///
    /// \brief Gets the instance of PlatformInfo used by the Tracker instantiated by this manager.
    ///
    IPlatformInfo* platformInfoProvider();

    ///
    /// \brief Creates a new CTracker using a given property ID.
    ///
    CTracker* createTracker(QString propertyId);

    ///
    /// \brief Removes and cleans up a given CTracker.
    ///
    void closeTracker(CTracker* pTracker);

    ///
    /// \brief Gets or sets whether CHit should be sent via SSL. Default is true.
    ///
    bool IsSecure;

    ///
    /// \brief Gets or sets whether CHit should be sent to the debug endpoint. Default is false.
    ///
    bool IsDebug;

    ///
    /// \brief Gets or sets whether the CTracker sends data.
    ///
    bool IsEnabled;

    ///
    /// \brief Gets or sets whether data should be sent via POST or GET method. Default is POST.
    ///
    bool PostData;

    ///
    /// \brief Gets or sets whether a cache buster should be applied to all requests. Default is false.
    ///
    bool BustCache;

private:
    void updateConnectionStatus();
    void loadAppOptOut();
    static QString getCacheBuster();

    static QString m_keyAppOptOut;
    bool m_isAppOptOutSet;

    static CAnalyticsManager* m_pInstance;
    IPlatformInfo* m_pPlatformInfo;
    QNetworkConfigurationManager* m_pNetworkConfigurationManager;

    QMap<QString, CTracker*> m_trackers;
    CTracker* m_pDefaultTracker;

    static QString m_endPointUnsecureDebug;
    static QString m_endPointSecureDebug;
    static QString m_endPointUnsecure;
    static QString m_endPointSecure;

private slots:
    void onOnlineStateChanged(bool isOnline);

    // IAnalyticsManager interface
public:
    void enqueueHit(const QMap<QString, QString> &params);
};

QTANALYTICS_NAMESPACE_END
