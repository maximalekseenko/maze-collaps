#include "userinterface/renderer.h"
#include "userinterface/controls.h"

#include "game.h"
#include "lib/log.h"
#include <locale>
#include <codecvt>
#include <thread>


int main(int argc, char *argv[]) {
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "");

    Renderer::Init();
    Controls::Init();

	std::thread t_renderer(Renderer::Run);
	std::thread c_renderer(Controls::Run);
    
    Game::LoadMap("data/theentrance.png");
    std::thread g_renderer(Game::Run);

	t_renderer.join();
	c_renderer.join();
	g_renderer.join();

    // UserInterface::Exit();
    Log::Out("+++END+++");
}