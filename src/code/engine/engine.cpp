#include "engine.h"


std::recursive_mutex Engine::ncurses_lock;



void Engine::Init()
{
    Engine::Renderer::Init();
    Engine::Controller::Init();
}



// void Engine::Run()
// { // fucking threading

// }