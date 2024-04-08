#include "engine/engine.h"

#include "game.h"
#include "lib/log.h"
#include <locale>
#include <codecvt>
#include <thread>


int main(int argc, char *argv[]) {
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "");

    Engine::Init();

	std::thread t_renderer(Engine::Renderer::Run);
	std::thread c_renderer(Engine::Controller::Run);
    
    Game::LoadMap("data/theentrance.png");
    std::thread g_renderer(Game::Run);

	t_renderer.join();
	c_renderer.join();
	g_renderer.join();

    // UserInterface::Exit();
    Log::Out("+++END+++");
}