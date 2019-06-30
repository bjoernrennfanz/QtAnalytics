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
#include "hit.h"

#include <QObject>
#include <QNetworkRequest>

QTANALYTICS_NAMESPACE_BEGIN

struct SHitFailedArgs
{
    SHitFailedArgs(QtAnalytics::CHit &hit, QString &errorMessage)
        : ErrorMessage(errorMessage)
        , Hit(hit)
    {
    }

    QString ErrorMessage;
    QtAnalytics::CHit Hit;
};

struct SHitSentArgs
{
    SHitSentArgs(QtAnalytics::CHit &hit, QString &response)
        : Response(response)
        , Hit(hit)
    {
    }

    QString Response;
    QtAnalytics::CHit Hit;
};

struct SHitMalformedArgs
{
    SHitMalformedArgs(QtAnalytics::CHit &hit, int httpStatusCode)
        : HttpStatusCode(httpStatusCode)
        , Hit(hit)
    {
    }

    int HttpStatusCode;
    QtAnalytics::CHit Hit;
};


class CAnalyticsManager : public QObject, public IAnalyticsManager
{
    Q_OBJECT

public:
    static CAnalyticsManager Current();

private:
    CAnalyticsManager(QObject *pParent = Q_NULLPTR);

    static QString m_endPointUnsecureDebug;
    static QString m_endPointSecureDebug;
    static QString m_endPointUnsecure;
    static QString m_endPointSecure;

public slots:

    // IAnalyticsManager interface
public:
    void EnqueueHit(const QMap<QString, QString> &params);
};

QTANALYTICS_NAMESPACE_END
