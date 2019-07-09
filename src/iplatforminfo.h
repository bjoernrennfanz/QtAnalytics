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
#include "dimensions.h"

#include <QObject>

QTANALYTICS_NAMESPACE_BEGIN

class IPlatformInfo : public QObject
{
    Q_OBJECT

public:
    virtual ~IPlatformInfo() {}

    ///
    /// \brief Gets the value that anonymously identifies a particular user, device, or browser instance.
    ///
    virtual QString getAnonymousClientId() const = 0;

    ///
    /// \brief Gets the viewport resolution.
    ///
    virtual Dimensions getViewPortResolution() const = 0;

    ///
    /// \brief Gets the screen resolution.
    ///
    virtual Dimensions getScreenResolution() const = 0;

    ///
    /// \brief Gets the screen color depth.
    ///
    virtual int getScreenColors() const = 0;

    ///
    /// \brief Gets the language (e.g. 'en-us').
    ///
    virtual QString getUserLanguage() const = 0;

    ///
    /// \brief Gets the user agent
    ///
    virtual QString getUserAgent() const = 0;

signals:
    ///
    /// \brief Raised to indicate that the <see cref="getViewPortResolution"/> has changed.
    ///
    void viewPortResolutionChanged();

    ///
    /// \brief Raised to indicate that the <see cref="getScreenResolution"/> has changed.
    ///
    void screenResolutionChanged();
};

QTANALYTICS_NAMESPACE_END
