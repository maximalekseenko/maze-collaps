#include "engine.h"

#include "../lib/log.h"
#include <thread>


bool is_engine_initialized = false;

std::thread engine_controller_thread;
std::thread engine_renderer_thread;


void Engine::Init()
{
    // argument checks
    // -is initialized
    if (is_engine_initialized)
        if (Engine::Settings::do_logs || !Engine::Settings::do_unsafe) 
            Log::Out("Attempting to initialize Engine twice.", !Engine::Settings::do_unsafe);

    // initialize
    Engine::Controller::Init();
    Engine::Renderer::Init();

    // remember initialization
    is_engine_initialized = true;

    // log
    if (Engine::Settings::do_logs) Log::Out("Engine is initialized!");
}



void Engine::Deinit()
{
    // argument checks
    // -not initialized
    if (!is_engine_initialized)
        Log::Out("Deninitializing Engine before initialization.", true);

    // deinitialize
    Engine::Controller::Deinit();
    Engine::Renderer::Deinit();

    // log
    if (Engine::Settings::do_logs) Log::Out("Engine is deinitialized!");
}



void Engine::Start()
{
    // argument checks
    // -not initialized
    if (!is_engine_initialized)
    {
        // log
        if (Engine::Settings::do_logs) Log::Out("Starting engine without initialization. Initializing...");

        // initialize
        Engine::Init();
    }
    // -controller already running
    if (engine_controller_thread.joinable()) Log::Out("Cannot start the engine: controller already running", true);
    // -renderer already running
    if (engine_renderer_thread.joinable()) Log::Out("Cannot start the engine: renderer already running", true);

    // begin
	engine_controller_thread = std::thread(Engine::Controller::ThreadFunc);
    engine_renderer_thread = std::thread(Engine::Renderer::ThreadFunc);

    // log
    if (Engine::Settings::do_logs) Log::Out("Engine is running!");
}

void Engine::Stop()
{
    
}