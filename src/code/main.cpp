#define _XOPEN_SOURCE 700
#include "engine/engine.h"

#include "game.h"
#include "lib/log.h"

#include <locale>
#include <thread>


int main(int argc, char *argv[]) {
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "en_US.UTF-8");

    Engine::Init();

    Engine::Start();
    
    Game::LoadMap("data/testmap.png");
    std::thread t_game(Game::Run);

	t_game.join();
    Engine::Stop();

    // UserInterface::Exit();
    Log::Out("+++END+++");
}