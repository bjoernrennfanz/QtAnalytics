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

#include "hitbuilder.h"

QTANALYTICS_NAMESPACE_USING

QString CHitBuilder::EHitType_Screenview = QString("screenview");
QString CHitBuilder::EHitType_Event = QString("event");
QString CHitBuilder::EHitType_Exception = QString("exception");
QString CHitBuilder::EHitType_UserTiming = QString("timing");

CHitBuilder::CHitBuilder()
{
    m_lineage = QVector<CHitBuilder>();
    m_lineage.append(*this);

    m_data = QMap<QString, QString>();
}

CHitBuilder::CHitBuilder(QMap<QString, QString> &data)
{
    m_lineage = QVector<CHitBuilder>();
    m_lineage.append(*this);

    m_data = QMap<QString, QString>();
    for(QMap<QString, QString>::const_iterator it = data.begin(), end = data.end(); it != end; ++it)
    {
        m_data.insert(it.key(), it.value());
    }
}

CHitBuilder::CHitBuilder(QVector<CHitBuilder> &lineage, QMap<QString, QString> &data)
{
    m_lineage = QVector<CHitBuilder>();
    for(QVector<CHitBuilder>::const_iterator it = lineage.begin(), end = lineage.end(); it != end; ++it)
    {
        m_lineage.append(*it);
    }
    m_lineage.append(*this);

    m_data = QMap<QString, QString>();
    for(QMap<QString, QString>::const_iterator it = data.begin(), end = data.end(); it != end; ++it)
    {
        m_data.insert(it.key(), it.value());
    }
}

CHitBuilder CHitBuilder::createScreenView()
{
    auto data = QMap<QString, QString>();
    data.insert("t", EHitType_Screenview);

    return CHitBuilder(data);
}

CHitBuilder CHitBuilder::createScreenView(QString& screenName)
{
    auto data = QMap<QString, QString>();
    data.insert("t", EHitType_Screenview);
    if (!screenName.isEmpty())
    {
        data.insert("cd", screenName);
    }

    return CHitBuilder(data);
}

CHitBuilder CHitBuilder::createCustomEvent(QString& category, QString action, QString label, long long value)
{
    auto data = QMap<QString, QString>();
    data.insert("t", EHitType_Event);
    data.insert("ec", category);
    data.insert("ea", action);

    if (!label.isEmpty()) data.insert("el", label);
    if (value) data.insert("ev", QString("%1").arg(value));

    return CHitBuilder(data);
}

CHitBuilder CHitBuilder::createException(QString& description, bool isFatal)
{
    auto data = QMap<QString, QString>();
    data.insert("t", EHitType_Exception);

    if (!description.isEmpty()) data.insert("exd", description);
    if (!isFatal) data.insert("exf", "0");

    return CHitBuilder(data);
}

CHitBuilder CHitBuilder::createTiming(QString& category, QString& variable, quint64 time, QString& label)
{
    auto data = QMap<QString, QString>();
    data.insert("t", EHitType_UserTiming);

    if (!category.isEmpty()) data.insert("utc", category);
    if (!variable.isEmpty()) data.insert("utv", variable);
    if (time != 0) data.insert("utt", QString("%1").arg(time));
    if (!label.isEmpty()) data.insert("utl", label);

    return CHitBuilder(data);
}

QString CHitBuilder::getValue(QString& paramName)
{
    return m_data.value(paramName);
}

CHitBuilder CHitBuilder::setValue(QString& paramName, QString& paramValue)
{
    if (m_data.contains(paramName))
    {
        m_data.take(paramName);
    }
    m_data.insert(paramName, paramValue);

    return CHitBuilder(m_lineage, m_data);
}

CHitBuilder CHitBuilder::setAll(QMap<QString, QString>& params)
{
    return CHitBuilder(m_lineage, params);
}

CHitBuilder CHitBuilder::setCustomDimension(int index, QString dimension)
{
    auto data = QMap<QString, QString>();
    data.insert(QString("cd%1").arg(index), dimension);

    return CHitBuilder(m_lineage, data);
}

CHitBuilder CHitBuilder::setCustomMetric(int index, long long metric)
{
    auto data = QMap<QString, QString>();
    data.insert(QString("cd%1").arg(index), QString("%1").arg(metric));

    return CHitBuilder(m_lineage, data);
}

CHitBuilder CHitBuilder::setNewSession()
{
    auto data = QMap<QString, QString>();
    data.insert("sc", "start");

    return CHitBuilder(m_lineage, data);
}

CHitBuilder CHitBuilder::setNonInteraction()
{
    auto data = QMap<QString, QString>();
    data.insert("ni", "1");

    return CHitBuilder(m_lineage, data);
}

QMap<QString, QString> CHitBuilder::build()
{
    auto data = QMap<QString, QString>();
    for(QVector<CHitBuilder>::const_iterator lineageIt = m_lineage.begin(), end = m_lineage.end(); lineageIt != end; ++lineageIt)
    {
        for(QMap<QString, QString>::const_iterator it = lineageIt->m_data.begin(), end = lineageIt->m_data.begin(); it != end; ++it)
        {
            if (data.contains(it.key()))
            {
                data.take(it.key());
            }
            data.insert(it.key(), it.value());
        }
    }

    return data;
}
