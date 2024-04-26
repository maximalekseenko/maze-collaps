#include "engine.h"

#include "curses.h"
#include "../lib/log.h"
#include <thread>

bool is_engine_initialized = false;

std::thread engine_controller_thread;
std::thread engine_renderer_thread;

void Engine::Init()
{
    // argument checks
    // -if already initialized
    if (is_engine_initialized)
        Log::Out("Attempting to initialize Engine twice.", true);

    // initialize
    Engine::Controller::Init();
    Engine::Renderer::Init();

    // remember initialization
    is_engine_initialized = true;

    // log
    if (Engine::Settings::do_logs)
        Log::Out("Engine is initialized!");
}

void Engine::Deinit()
{
    // argument checks
    // -if not initialized
    if (!is_engine_initialized)
        Log::Out("Deninitializing Engine before initialization.", true);

    // deinitialize
    Engine::Controller::Deinit();
    Engine::Renderer::Deinit();
    {
        std::lock_guard curses_locker(Curses::curses_lock);
        endwin();
    }

    // log
    if (Engine::Settings::do_logs)
        Log::Out("Engine is deinitialized!");
}

void Engine::Start()
{
    // argument checks
    // -if any thread is already running
    if (engine_controller_thread.joinable())
        Log::Out("Cannot start the engine: controller already running", true);
    if (engine_renderer_thread.joinable())
        Log::Out("Cannot start the engine: renderer already running", true);
    // -if uninitialized
    if (!is_engine_initialized)
    {
        // log
        if (Engine::Settings::do_logs)
            Log::Out("Starting engine without initialization. Initializing...");

        // initialize
        Engine::Init();
    }

    // begin
    engine_controller_thread = std::thread(Engine::Controller::ThreadFunc);
    engine_renderer_thread = std::thread(Engine::Renderer::ThreadFunc);

    // log
    if (Engine::Settings::do_logs)
        Log::Out("Engine is running!");
}

void Engine::Stop()
{
    // argument checks
    // -if any thread is not running
    if (!engine_controller_thread.joinable())
        Log::Out("Cannot start the engine: controller already running", true);
    if (!engine_renderer_thread.joinable())
        Log::Out("Cannot start the engine: renderer already running", true);

    // break loops in threads
    Engine::Controller::StopThreadLoop();
    Engine::Renderer::StopThreadLoop();

    // join threads
    engine_controller_thread.join();
    engine_renderer_thread.join();
}