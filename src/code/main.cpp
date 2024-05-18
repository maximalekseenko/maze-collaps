#define _XOPEN_SOURCE 700
#include "engine/engine.h"

#include "game.h"
#include "utility/log.h"

#include <locale>
#include <thread>

// TEMP
#include "map/reader.h"

int main(int argc, char *argv[])
{
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "en_US.UTF-8");

    int *image=nullptr, X, Y;
    Reader::ReadImage("data/testmap.png", image, X, Y);

    for (int y=0; y < Y; y ++) {
        std::string s;
        for (int x=0; x < X; x ++)
            s += std::to_string(image[y*X+x]) + " ";
        Log::Out(s);
    }

    // Engine::Init();

    // Engine::Start();

    // Game::LoadMap("data/testmap.png");
    // std::thread t_game(Game::Run);

    // t_game.join();
    // Engine::Stop();

    // UserInterface::Exit();
    Log::Out("+++END+++");
}