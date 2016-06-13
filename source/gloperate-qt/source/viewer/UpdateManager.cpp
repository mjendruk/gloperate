
#include <gloperate-qt/viewer/UpdateManager.h>

#include <QTime>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate_qt
{


UpdateManager::UpdateManager(gloperate::ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
{
    // Connect update timer
    QObject::connect(
        &m_timer, &QTimer::timeout,
        this, &UpdateManager::onTimer
    );

    // Setup timer
    m_timer.setSingleShot(false);
    m_timer.start(0);

    // Start time measurement
    m_time.start();
}

UpdateManager::~UpdateManager()
{
}

void UpdateManager::wakeTimer()
{
    m_timer.start(0);
}

void UpdateManager::onTimer()
{
    // Restart timer
    m_time.restart();

    // Update timing
    if (!m_viewerContext->update())
    {
        m_timer.stop();
    }
}


} // namespace gloperate_qt
