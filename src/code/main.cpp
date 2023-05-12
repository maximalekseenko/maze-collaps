#include "visualizer.h"

#include "game.h"

#include <locale>
#include <codecvt>
int main() {

    std::ios_base::sync_with_stdio(false);
    std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
    std::wcout.imbue(utf8);

    Game g;
    Visualizer::Init(100, 50);
    // g.Intro();
    g.LoadMap(0);
    g.Run();
}
