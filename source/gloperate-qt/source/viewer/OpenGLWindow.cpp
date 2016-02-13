
#include "gloperate-qt/viewer/OpenGLWindow.h"

#include <QApplication>
#include <QResizeEvent>
#include <QOpenGLContext>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/GLContextFactory.h>


namespace gloperate_qt
{


OpenGLWindow::OpenGLWindow(const gloperate::GLContextFormat & format)
: m_context(nullptr)
, m_initialized(false)
, m_updatePending(false)
{
    // Create window with OpenGL capability
    setSurfaceType(OpenGLSurface);
    create();

    // Set desired OpenGL context format
    setContextFormat(format);

    // Create an OpenGL context
    createContext();
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_context;
}

GLContext * OpenGLWindow::context() const
{
    return m_context;
}

void OpenGLWindow::updateGL()
{
    if (!m_updatePending) {
        m_updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void OpenGLWindow::setContextFormat(const gloperate::GLContextFormat & format)
{
    m_format = format;
}

void OpenGLWindow::createContext()
{
    // Destroy old context
    if (m_context)
    {
        deinitializeContext();
        delete m_context;
    }

    // Create OpenGL context
    GLContextFactory factory(this);
    m_context = static_cast<GLContext*>(factory.createContext(m_format));

    // Initialize new context
    initializeContext();
}

void OpenGLWindow::initializeContext()
{
    if (m_initialized)
    {
        return;
    }

    m_context->qtContext()->makeCurrent(this);
    onContextInit();
    m_context->qtContext()->doneCurrent();

    m_initialized = true;
}

void OpenGLWindow::deinitializeContext()
{
    if (!m_initialized)
    {
        return;
    }

    m_context->qtContext()->makeCurrent(this);
    onContextDeinit();
    m_context->qtContext()->doneCurrent();

    m_initialized = false;
}

void OpenGLWindow::resize(QResizeEvent * event)
{
    if (!m_initialized) {
        initializeContext();
    }

    m_context->qtContext()->makeCurrent(this);

    QResizeEvent deviceSpecificResizeEvent(event->size() * devicePixelRatio(), event->oldSize() * devicePixelRatio());
    onResize(&deviceSpecificResizeEvent);

    m_context->qtContext()->doneCurrent();
}

void OpenGLWindow::paint()
{
    if (!m_initialized) {
        initializeContext();
    }

    if (!isExposed()) {
        return;
    }

    m_updatePending = false;

    m_context->qtContext()->makeCurrent(this);

    onPaint();
    m_context->qtContext()->swapBuffers(this);

    m_context->qtContext()->doneCurrent();
}

void OpenGLWindow::onContextInit()
{
}

void OpenGLWindow::onContextDeinit()
{
}

void OpenGLWindow::onResize(QResizeEvent * )
{
}

void OpenGLWindow::onPaint()
{
}

bool OpenGLWindow::event(QEvent * event)
{
    switch (event->type())
    {
        case QEvent::UpdateRequest:
            paint();
            return true;

        case QEvent::Enter:
            enterEvent(event);
            return true;

        case QEvent::Leave:
            leaveEvent(event);
            return true;

        default:
            return QWindow::event(event);
    }
}

void OpenGLWindow::resizeEvent(QResizeEvent * event)
{
    resize(event);
    paint();
}

void OpenGLWindow::exposeEvent(QExposeEvent * )
{
    paint();
}

void OpenGLWindow::enterEvent(QEvent *)
{
}

void OpenGLWindow::leaveEvent(QEvent *)
{
}


} // namespace gloperate-qt
