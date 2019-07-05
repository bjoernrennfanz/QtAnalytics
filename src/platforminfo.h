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
#include "iplatforminfo.h"

#include <QObject>

#include <QWidget>
#include <QDesktopWidget>

QTANALYTICS_NAMESPACE_BEGIN

class CPlatformInfo: public IPlatformInfo
{
    Q_OBJECT
    Q_PROPERTY(QString anonymousClientId MEMBER m_anonymousClientId READ getAnonymousClientId WRITE setAnonymousClientId)
    Q_PROPERTY(Dimensions viewPortResolution MEMBER m_viewPortResolution READ getViewPortResolution)
    Q_PROPERTY(Dimensions screenResolution MEMBER m_screenResolution READ getScreenResolution)
    Q_PROPERTY(int screenColors READ getScreenColors)
    Q_PROPERTY(QString userLanguage READ getUserLanguage)

public:
    CPlatformInfo();
    virtual ~CPlatformInfo();

    void setAnonymousClientId(const QString &value);

private:
    void initializeWindow();
    QString constructUserAgent() const;
    Dimensions parseDimensionsUpdate(Dimensions &currentDimensions, const QRect &newSize, bool &hasChanged);

    void setViewPortResolution(const QRect &value);
    void setScreenResolution(const QRect &value);

    Dimensions m_viewPortResolution;
    Dimensions m_screenResolution;

    static QString m_keyAnonymousClientId;
    mutable QString m_anonymousClientId;

    QDesktopWidget *m_pActiveDesktop;
    QWidget *m_pActiveWindow;
    bool m_windowInitialized;

private slots:
    void onViewPortResolutionChanged(QResizeEvent *event);
    void onScreenResolutionChanged(QResizeEvent *eventy);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    // IPlatformInfo interface
public:
    QString getAnonymousClientId() const;
    Dimensions getViewPortResolution() const;
    Dimensions getScreenResolution() const;
    int getScreenColors() const;
    QString getUserLanguage() const;
};

QTANALYTICS_NAMESPACE_END