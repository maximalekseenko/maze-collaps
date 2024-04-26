/// @file engine.h
/// @brief Header with engine core functions.
/// Implementations is splitted into multiple files:
/// @see ./engine/engine.cpp for Engine implementation;
/// @see ./engine/engine_controller.cpp for Engine::Controller implementation;
/// @see ./engine/engine_renderer.cpp for Engine::Renderer implementation;
/// @see ./engine/engine_settings.cpp for Engine::Settings implementation;
/// @author https://github.com/maximalekseenko

#ifndef __ENGINE_ENGINE_H
#define __ENGINE_ENGINE_H

// dependencies
#include "./visualcomponent.h"
#include "./color.h"
#include "./curses.h"

/// @brief Core of the program. It handles user interface.
/// @see Engine::Controller for user input functions.
/// @see Engine::Renderer for user output functions.
/// @see Engine::Settings for settings for engine.
/// @see file ./engine.cpp for implementations.
namespace Engine
{
    /// @brief Initializes Renderer and Controller.
    /// @throws Throws an error if already initialized.
    /// @see Engine::Renderer::Init().
    /// @see Engine::Controller::Init().
    void Init();

    /// @brief Deinitializes Renderer and Controller.
    /// @throws Throws an error if not initialized
    /// @see Engine::Renderer::Deinit().
    /// @see Engine::Controller::Deinit().
    void Deinit();

    /// @brief Starts Renderer and Controller as threads.
    /// @throws Throws an error if any thread is already running.
    /// @note Initializes if uninitialized.
    void Start();

    /// @brief Stops threads of Controller and Renderer.
    /// @throws Throws an error if any thread is not running.
    /// @see Engine::Start() for starting.
    void Stop();

    /// @brief Module of the engine, that handles user input.
    /// @see file ./engine_controller.cpp for implementations.
    namespace Controller
    {

        /// @brief Initializes Controller.
        /// @throws Throws an error if already initialized.
        /// @warning Dont forget to deinitialize.
        /// @see Engine::Controller::Deinit().
        void Init();

        /// @brief Deinitializes controller.
        /// @throws Throws an error if not initialized.
        void Deinit();

        /// @brief function for running Controller as a thread.
        /// @throws Throws an error if not initialized.
        /// @throws Throws an error if already running.
        /// @see Engine::Controller::StopThreadLoop() to stop the thread;
        /// @warning This function should be used as a thread.
        void ThreadFunc();

        /// @brief Breaks the loop in controller's thread.
        /// @throws Throws an error if loop is already broken.
        void StopThreadLoop();
    };

    /// @brief Module the of engine, that handles output for user.
    /// @see file ./engine_renderer.cpp for implementations.
    namespace Renderer
    {

        /// @brief Initializes Renderer.
        /// @throws Throws an error if already initialized.
        /// @warning Dont forget to deinitialize.
        /// @see Engine::Renderer::Deinit().
        void Init();

        /// @brief Deinitializes controller.
        /// @throws Throws an error if not initialized.
        void Deinit();

        /// @brief function for running Renderer as a thread.
        /// @throws Throws an error if not initialized.
        /// @throws Throws an error if already running.
        /// @see Engine::Renderer::StopThreadLoop() to stop the thread;
        /// @warning This function should be used as a thread.
        void ThreadFunc();

        /// @brief Breaks the loop in renderer's thread.
        /// @throws Throws an error if loop is already broken.
        void StopThreadLoop();

        /// @brief Sets background color of main window.
        /// @throws Throws an error if not initialized.
        /// @param __colorB New color for background.
        void SetBackground(Color __colorB);

        /// @brief Gets background color of main window.
        /// @throws Throws an error if not initialized.
        /// @return Background color of main window.
        Color GetBackground();
    };

    namespace Settings
    {

        /// @brief Enables non-error logs during runtime;
        extern bool do_logs;

        /// @brief Disables some error throws that may not actually cause an error.
        extern bool do_unsafe;
    }
};

#endif // __ENGINE_ENGINE_H