
#include "userInterface.h"

#include "game.h"
#include "log.h"
#include <locale>
#include <codecvt>
int main(int argc, char *argv[]) {

    // std::ios_base::sync_with_stdio(false);
    // std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
    // std::wcout.imbue(utf8);
    setlocale(LC_ALL, "");


    UserInterface::Init();

    Game g;
    // g.Intro();
    g.LoadMap(0);
    
    g.Run();
    getch();
    UserInterface::Exit();
}