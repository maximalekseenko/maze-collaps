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

    Engine::Start();
	std::thread t_renderer(Engine::Renderer::Run);
	std::thread t_controller(Engine::Controller::Run);
    
    Game::LoadMap("data/testmap.png");
    std::thread t_game(Game::Run);

	t_renderer.join();
	t_controller.join();
	t_game.join();

    // UserInterface::Exit();
    Log::Out("+++END+++");
}