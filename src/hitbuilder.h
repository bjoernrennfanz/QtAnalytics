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

#include "hit.h"
#include "qtanalytics_global.h"

#include <QMap>
#include <QVector>

QTANALYTICS_NAMESPACE_BEGIN

class CHitBuilder
{
private:
    CHitBuilder(QMap<QString, QString>& data);
    CHitBuilder(QVector<CHitBuilder*>& lineage, QMap<QString, QString>& data);

    static QString EHitType_Screenview;
    static QString EHitType_Event;
    static QString EHitType_Exception;
    static QString EHitType_UserTiming;

    QVector<CHitBuilder*> m_lineage;
    QMap<QString, QString> m_data;

public:
    CHitBuilder();

    static CHitBuilder createScreenView();
    static CHitBuilder createScreenView(QString screenName);

    static CHitBuilder createCustomEvent(QString category, QString action, QString label = QString(), long long value = 0);

    static CHitBuilder createException(QString description, bool isFatal);

    static CHitBuilder createTiming(QString category, QString variable, quint64 time, QString label);

    QString getValue(QString paramName);
    CHitBuilder setValue(QString paramName, QString paramValue);
    CHitBuilder setAll(QMap<QString, QString>& params);

    CHitBuilder setCustomDimension(int index, QString dimension);
    CHitBuilder setCustomMetric(int index, long long metric);

    CHitBuilder setNewSession();
    CHitBuilder setNonInteraction();

    QMap<QString, QString> build();
};

QTANALYTICS_NAMESPACE_END
