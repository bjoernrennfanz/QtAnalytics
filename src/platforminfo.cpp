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

#include "platforminfo.h"

#include <QApplication>
#include <QDateTime>
#include <QLocale>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QSettings>

#ifdef Q_OS_LINUX
#  include <sys/utsname.h>
#endif

QTANALYTICS_NAMESPACE_USING

QString CPlatformInfo::m_keyAnonymousClientId = "AnonymousClientId";

CPlatformInfo::CPlatformInfo()
    : IPlatformInfo()
    , m_pActiveDesktop(Q_NULLPTR)
    , m_pActiveWindow(Q_NULLPTR)
    , m_windowInitialized(false)
{
    initializeWindow();
}

CPlatformInfo::~CPlatformInfo()
{
    // Uninstall event filters
    if (m_pActiveWindow)
    {
        m_pActiveWindow->removeEventFilter(this);
    }

    if (m_pActiveDesktop)
    {
        m_pActiveDesktop->removeEventFilter(this);
    }

    m_pActiveWindow = Q_NULLPTR;
    m_pActiveDesktop = Q_NULLPTR;
    m_windowInitialized = false;
}

void CPlatformInfo::setAnonymousClientId(const QString& value)
{
    m_anonymousClientId = value;
}

void CPlatformInfo::initializeWindow()
{
    QWidget* pActiveWindow = qApp->activeWindow();
    QDesktopWidget* pActiveDesktop = qApp->desktop();
    if (pActiveWindow && pActiveDesktop)
    {
        // Setup resolutions
        setViewPortResolution(pActiveWindow->geometry());
        setScreenResolution(pActiveDesktop->geometry());

        // Store for event processing
        m_pActiveWindow = pActiveWindow;
        m_pActiveDesktop = pActiveDesktop;

        // Install event filters
        m_pActiveWindow->installEventFilter(this);
        m_pActiveDesktop->installEventFilter(this);

        m_windowInitialized = true;
    }
}

Dimensions CPlatformInfo::parseDimensionsUpdate(Dimensions& currentDimensions, const QRect& newSize, bool& hasChanged)
{
    Dimensions valueDimensions;
    if (newSize.isValid())
    {
        valueDimensions.Width = newSize.size().width();
        valueDimensions.Height = newSize.size().height();
        if (qFuzzyCompare(currentDimensions.Height, newSize.size().height()) || qFuzzyCompare(currentDimensions.Width, newSize.size().width()))
        {
            hasChanged = true;
        }
    }

    return valueDimensions;
}

void CPlatformInfo::setViewPortResolution(const QRect& value)
{
    bool hasChanged = false;
    m_viewPortResolution = parseDimensionsUpdate(m_viewPortResolution, value, hasChanged);
    if (hasChanged)
    {
        emit viewPortResolutionChanged();
    }
}

void CPlatformInfo::setScreenResolution(const QRect& value)
{
    bool hasChanged = false;
    m_screenResolution = parseDimensionsUpdate(m_screenResolution, value, hasChanged);
    if (hasChanged)
    {
        emit screenResolutionChanged();
    }
}

void CPlatformInfo::onViewPortResolutionChanged(QResizeEvent* event)
{
    setScreenResolution(QRect(QPoint(0, 0), event->size()));
}

void CPlatformInfo::onScreenResolutionChanged(QResizeEvent* event)
{
    setViewPortResolution(QRect(QPoint(0, 0), event->size()));
}

bool CPlatformInfo::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::Resize)
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        if (obj == m_pActiveWindow)
        {
            onViewPortResolutionChanged(resizeEvent);
        }
        else if (obj == m_pActiveDesktop)
        {
            onScreenResolutionChanged(resizeEvent);
        }

        return false;
    }

    return QObject::eventFilter(obj, event);
}

QString CPlatformInfo::getAnonymousClientId() const
{
    if (!m_anonymousClientId.isEmpty())
    {
        return m_anonymousClientId;
    }

    QSettings settings;
    const QString defaultId("NoKey");

    settings.beginGroup("GoogleAnaltyics");
    m_anonymousClientId = settings.value(m_keyAnonymousClientId, defaultId).toString();
    if (m_anonymousClientId.contains(defaultId))
    {
        // Generate id like javascript one
        quint32 time = static_cast<quint32>(QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000);
        quint32 seed = QRandomGenerator::global()->generate();

        // Concat to anonymous client id
        QString timeString = QString::number(time).rightJustified(10, '0', true);
        QString seedString = QString::number(seed).rightJustified(10, '0', true);
        m_anonymousClientId = QString("%1.%2").arg(timeString).arg(seedString);

        // Persist into registry
        settings.setValue(m_keyAnonymousClientId, m_anonymousClientId);
    }

    settings.endGroup();

    return m_anonymousClientId;
}

Dimensions CPlatformInfo::getViewPortResolution() const
{
    return m_viewPortResolution;
}

Dimensions CPlatformInfo::getScreenResolution() const
{
    return m_screenResolution;
}

int CPlatformInfo::getScreenColors() const
{
    return 0;
}

QString CPlatformInfo::getUserLanguage() const
{
    return QLocale::system().name();
}

QString CPlatformInfo::getSystemInfo() const
{
    QString osString;

#if defined (Q_OS_MAC)
    QSysInfo::MacVersion version = QSysInfo::macVersion();
    switch (version)
    {
    case QSysInfo::MV_10_10:
        osString += "Macintosh; Mac OS 10.10";
        break;
    case QSysInfo::MV_10_11:
        osString += "Macintosh; Mac OS 10.11";
        break;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    case QSysInfo::MV_10_12:
        osString += "Macintosh; Mac OS 10.12";
        break;
#endif
    default:
        osString += "Macintosh; Mac OS unknown";
        break;
    }
    return os;
#elif defined(Q_OS_WIN)
    osString += "Windows; ";
    QSysInfo::WinVersion version = QSysInfo::windowsVersion();
    switch (version)
    {
    case QSysInfo::WV_WINDOWS7:
        osString += "Win 7";
        break;
    case QSysInfo::WV_WINDOWS8:
        osString += "Win 8";
        break;
    case QSysInfo::WV_WINDOWS8_1:
        osString += "Win 8.1";
        break;
      case QSysInfo::WV_WINDOWS10:
        osString += "Win 10";
        break;
    default:
        osString += "Windows (unknown)";
        break;
    }
#elif defined(Q_OS_LINUX)
    osString += "Linux; ";

    struct utsname uts;
    if (uname(&uts) == 0)
    {
        osString += QLatin1String(uts.sysname) + QLatin1Char(' ') + QLatin1String(uts.release);
    }
    else
    {
        osString += QLatin1String("Linux (Unknown)");
    }
#else
    osString += QLatin1String("Unknown OS");
#endif

    return osString;
}

QString CPlatformInfo::getUserAgent() const
{
    QString locale = getUserLanguage();
    QString system = getSystemInfo();

    QString appName = qApp->applicationName();
    QString appVersion = qApp->applicationVersion();
    QString userAgent = QString("%1/%2 (%3; %4) QtAnalytics/1.0 (Qt/%5)").arg(appName).arg(appVersion).arg(system).arg(locale).arg(QT_VERSION_STR);

    return userAgent;
}
