
// #include "userInterface.h"

#include "renderer.h"
#include "game.h"
#include "lib/log.h"
#include <locale>
#include <codecvt>


int main(int argc, char *argv[]) {
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "");

    Renderer::Init();

    Game::game.LoadMap("data/hostilecave.png");
    Game::game.Run();

    // UserInterface::Exit();
    Log::Out("+++END+++");
}
