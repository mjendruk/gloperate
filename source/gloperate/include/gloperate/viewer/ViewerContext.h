
#pragma once


#include <vector>
#include <string>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/variant/Variant.h>
#include <cppexpose/signal/Signal.h>

#include <gloperate/viewer/ComponentManager.h>
#include <gloperate/viewer/ResourceManager.h>
#include <gloperate/viewer/TimeManager.h>
#include <gloperate/viewer/System.h>
#include <gloperate/viewer/PipelineApi.h>
#include <gloperate/input/InputManager.h>

// [DEBUG]
#include <cppexpose/scripting/example/TreeNode.h>


namespace cppexpose {
    class ScriptContext;
    class AbstractScriptBackend;
}


namespace gloperate
{


class Surface;


/**
*  @brief
*    Context containing the top-level components needed for rendering
*
*    The viewer context holds all necessary components used for the rendering process,
*    such as the plugin manager, input manager, resource loaders, etc. It should
*    usually be instanciated once by the top-level viewer application and passed
*    down to other objects that need access these components. Some components
*    have to be provided by the windowing backend on creation of the viewer context.
*
*    A viewer context is independend from windows and OpenGL contexts and should be
*    initialized before creating actual viewers and their OpenGL contexts.
*/
class GLOPERATE_API ViewerContext : public cppexpose::Object
{
    friend class Surface;


public:
    cppexpose::Signal<int> exitApplication;   ///< Called when application shall exit


public:
    //@{
    /**
    *  @brief
    *    Constructor
    */
    ViewerContext();

    /**
    *  @brief
    *    Destructor
    */
    ~ViewerContext();
    //@}

    //@{
    /**
    *  @brief
    *    Get component manager
    *
    *  @return
    *    Component manager (must NOT be null)
    */
    const ComponentManager * componentManager() const;
    ComponentManager * componentManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get resource manager
    *
    *  @return
    *    Resource manager (must NOT be null)
    */
    const ResourceManager * resourceManager() const;
    ResourceManager * resourceManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get time manager
    *
    *  @return
    *    Time manager (must NOT be null)
    */
    const TimeManager * timeManager() const;
    TimeManager * timeManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get input manager
    *
    *  @return
    *    Input manager (must NOT be null)
    */
    const InputManager * inputManager() const;
    InputManager * inputManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get surfaces
    *
    *  @return
    *    List of registered surfaces (must NOT be null)
    */
    const std::vector<Surface *> & surfaces() const;
    std::vector<Surface *> & surfaces();
    //@}

    //@{
    /**
    *  @brief
    *    Get scripting context
    *
    *  @return
    *    Script context (can be null)
    */
    const cppexpose::ScriptContext * scriptContext() const;
    cppexpose::ScriptContext * scriptContext();
    //@}

    //@{
    /**
    *  @brief
    *    Setup scripting environment
    *
    *  @param[in] backendName
    *    Name of scripting backend to use (default: 'javascript')
    */
    void setupScripting(const std::string & backendName = "");

    /**
    *  @brief
    *    Setup scripting environment
    *
    *  @param[in] backend
    *    Scripting backend to use (must NOT be null)
    */
    void setupScripting(cppexpose::AbstractScriptBackend * backend);

    /**
    *  @brief
    *    Execute script code
    *
    *  @param[in] code
    *    Script code
    *
    *  @return
    *    Return value from the scripting context
    */
    cppexpose::Variant executeScript(const std::string & code);
    //@}

    //@{
    /**
    *  @brief
    *    Update timing
    *
    *  @return
    *    'true' if there are any timers active, 'false' if not
    *
    *  @remarks
    *    This signature measures the time since the last call
    *    for the time delta. It should usually be used in
    *    interactive applications.
    */
    bool update();

    /**
    *  @brief
    *    Update timing
    *
    *  @param[in] delta
    *    Time delta (in seconds)
    *
    *  @return
    *    'true' if there are any timers active, 'false' if not
    *
    *  @remarks
    *    This signature can be used to provide a specific time delta,
    *    e.g., when rendering videos at a fixed frame rate.
    */
    bool update(float delta);

    /**
    *  @brief
    *    Exit application
    *
    *    This function emits the signal 'exitApplication'.
    *    Windowing backends are expected to connect to that signal
    *    and exit the application when receiving the signal.
    *
    *  @param[in] exitCode
    *    Exit code (default: 0)
    */
    void exit(int exitCode = 0);
    //@}


protected:
    //@{
    /**
    *  @brief
    *    Register local plugins (contained in gloperate itself)
    */
    void registerLocalPlugins();

    /**
    *  @brief
    *    Initialize scripting environment
    *
    *  @param[in] scriptContext
    *    New script context (must NOT be nullptr!)
    *
    *  @remarks
    *    If there is a script context already active, it will be deleted
    *    before the new one is initialized.
    */
    void initializeScripting(cppexpose::ScriptContext * scriptContext);

    /**
    *  @brief
    *    Register render surface
    *
    *  @param[in] surface
    *    Render surface (must NOT be null!)
    */
    void registerSurface(Surface * surface);

    /**
    *  @brief
    *    Un-register render surface
    *
    *  @param[in] surface
    *    Render surface (must NOT be null!)
    */
    void unregisterSurface(Surface * surface);
    //@}


protected:
    ComponentManager            m_componentManager;  ///< Manager for plugin libraries and components
    ResourceManager             m_resourceManager;   ///< Resource manager for loaders/storers
    TimeManager                 m_timeManager;       ///< Manager for virtual time and timers
    System                      m_system;            ///< System functions for scripting
    PipelineApi                 m_pipeline;          ///< Pipeline API for scripting
    InputManager                m_inputManager;      ///< Manager for Devices, -Providers and InputEvents

    std::vector<Surface *>      m_surfaces;          ///< List of active surfaces

    cppexpose::TreeNode         m_tree;              ///< Test object for scripting

    cppexpose::ScriptContext  * m_scriptContext;     ///< Scripting context

    std::string                 m_helpText;          ///< Text that is displayed on 'help'
};


} // namespace gloperate
