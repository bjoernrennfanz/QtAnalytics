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

#include "ianalyticsmanager.h"
#include "iplatforminfo.h"
#include "hit.h"

#include <QMap>

QTANALYTICS_NAMESPACE_BEGIN

class CTracker : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString propertyId READ getPropertyId)
    Q_PROPERTY(bool anonymizeIP MEMBER AnonymizeIP)

    Q_PROPERTY(QString clientId MEMBER ClientId)

    Q_PROPERTY(QString ipOverride MEMBER IpOverride)
    Q_PROPERTY(QString userAgentOverride MEMBER UserAgentOverride)
    Q_PROPERTY(QString locationOverride MEMBER LocationOverride)

    Q_PROPERTY(Dimensions screenResolution MEMBER ScreenResolution)
    Q_PROPERTY(Dimensions viewportSize MEMBER ViewportSize)
    Q_PROPERTY(QString encoding MEMBER Encoding)
    Q_PROPERTY(int screenColors MEMBER ScreenColors)
    Q_PROPERTY(QString language MEMBER Language)


    Q_PROPERTY(QString appName MEMBER AppName)
    Q_PROPERTY(QString appId MEMBER AppId)
    Q_PROPERTY(QString appVersion MEMBER AppVersion)
    Q_PROPERTY(QString appInstallerId MEMBER AppInstallerId)

public:
    CTracker(QString& propertyId, IPlatformInfo* pPlatformInfo, IAnalyticsManager* pAnalyticsManager);

    /// <summary>
    /// Gets or sets the tracking ID / web property ID. The format is UA-XXXX-Y. All collected data is associated by this ID.
    /// </summary>
    /// <remarks>Required for all hit types.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#tid"/>
    QString getPropertyId();

    /// <summary>
    /// Gets the model value for the given key added through <see cref="Set"/>.
    /// </summary>
    /// <param name="key">The key to retrieve the value for.</param>
    /// <returns>The value associated with the key.</returns>
    QString getValue(QString &key);

    /// <summary>
    /// Sets the model value for the given key.
    /// </summary>
    /// <param name="key">The key of the field that needs to be set. It starts with "&amp;" followed by the parameter name. The complete list of fields can be found at <see href="https://goo.gl/M6dK2U"/>.</param>
    /// <param name="value">A string value to be sent to Google servers. A null value denotes that the value should not be sent over wire.</param>
    void setValue(QString &key, QString &value);

    /// <summary>
    /// Merges the model values set on this Tracker with params and generates a hit to be sent.
    /// </summary>
    /// <param name="params">Dictionary of hit data to values which are merged with the existing values which are already set (using Set(String, String)). Values in this dictionary will override the values set earlier. The values in this dictionary will not be reused for the subsequent hits. If you need to send a value in multiple hits, you can use the Set(String, String) method.</param>
    /// <remarks>The hit may not be dispatched immediately.</remarks>
    void send(QMap<QString, QString> &params);

    /// <summary>
    /// Gets or sets whether the IP address of the sender will be anonymized.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aiid"/>
    bool AnonymizeIP;

    /// <summary>
    /// Gets or sets the value that anonymously identifies a particular user, device, or browser instance. For the web, this is generally stored as a first-party cookie with a two-year expiration. For
    /// mobile apps, this is randomly generated for each particular instance of an application install. The value of this field should be a random UUID (version 4) as described in <see
    /// href="http://www.ietf.org/rfc/rfc4122.txt"/>.
    /// </summary>
    /// <remarks>Required for all hit types.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cid"/>
    QString ClientId;

    /// <summary>
    /// Gets or sets the IP address of the user. This should be a valid IP address in IPv4 or IPv6 format. It will always be anonymized just as though anonymize IP had been used.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#uip"/>
    QString IpOverride;

    /// <summary>
    /// Gets or sets the User Agent of the browser. Note that Google has libraries to identify real user agents. Hand crafting your own agent could break at any time.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ua"/>
    QString UserAgentOverride;

    /// <summary>
    /// Gets or sets the geographical location of the user. The geographical ID should be a two letter country code or a criteria ID representing a city or region (see
    /// http://developers.google.com/analytics/devguides/collection/protocol/v1/geoid). This parameter takes precedent over any location derived from IP address, including the IP Override parameter.
    /// An invalid code will result in geographical dimensions to be set to '(not set)'.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#geoid"/>
    QString LocationOverride;

    /// <summary>
    /// Gets or sets the screen resolution.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sr"/>
    Dimensions ScreenResolution;

    /// <summary>
    /// Gets or sets the viewable area of the browser / device.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#vp"/>
    Dimensions ViewportSize;

    /// <summary>
    /// Gets or sets the character set used to encode the page / document.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#de"/>
    QString Encoding;

    /// <summary>
    /// Gets or sets the screen color depth.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sd"/>
    int ScreenColors;

    /// <summary>
    /// Gets or sets the language.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ul"/>
    QString Language;

    /// <summary>
    /// Gets or sets the 'Screen Name' of the screenview hit.
    /// </summary>
    /// <remarks>Required for screenview hit type.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cd"/>
    QString ScreenName;

    /// <summary>
    /// Gets or sets the application name. This field is required for any hit that has app related data (i.e., app version, app ID, or app installer ID). For hits sent to web properties, this field is
    /// optional.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#an"/>
    QString AppName;

    /// <summary>
    /// Gets or sets the application identifier.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aid"/>
    QString AppId;

    /// <summary>
    /// Gets or sets the application version.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#av"/>
    QString AppVersion;

    /// <summary>
    /// Gets or sets the application installer identifier.
    /// </summary>
    /// <remarks>Optional.</remarks>
    /// <seealso href="https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#aiid"/>
    QString AppInstallerId;

private slots:
    void onViewPortResolutionChanged();
    void onScreenResolutionChanged();

private:
    QMap<QString, QString> addRequiredHitData(QMap<QString, QString> &params);

    IAnalyticsManager* m_pAnalyticsManager;
    IPlatformInfo* m_pPlatformInfo;

    QMap<QString, QString> m_data;
    QString m_propertyId;
};

QTANALYTICS_NAMESPACE_END
