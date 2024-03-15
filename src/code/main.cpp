
// #include "userInterface.h"

#include "game.h"
#include "log.h"
#include <locale>
#include <codecvt>

int main(int argc, char *argv[]) {
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "");
    // UserInterface::Init();


    Game *game = new Game();

    Game::game.LoadMap("data/hostilecave.png");
    Game::game.Run();

    delete game;

    // UserInterface::Exit();
    Log::Out("+++END+++");
}
