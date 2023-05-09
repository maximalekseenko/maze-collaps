#define STB_IMAGE_IMPLEMENTATION



#include "visualizer.h"

#include "game.h"



#include <locale>
#include <codecvt>
int main() {

    std::ios_base::sync_with_stdio(false);
    std::locale utf8( std::locale(), new std::codecvt_utf8_utf16<wchar_t> );
    std::wcout.imbue(utf8);


    Game g;
    Visualizer::Init(100, 50);
    g.PrintIntro();
    Visualizer::Out();
    // g.Level(1);

//     // std::cout << Image::GetPixelValue(0x10, 0xa0, 0x10) << " ";
//     // std::cout << Image::GetPixelValue(0xa0, 0xa0, 0xa0) << " ";
//     // std::cout << Image::GetPixelValue(0xa0, 0x10, 0x10) << "\n";

    // OverlappingWave AA("theentrance.png", 4, 32, 16, true, true, 8, false, Model::Heuristic::Entropy);
    // if (AA.Run(-1, -1))
    //     AA.Save("AA");
    // else std::cout << "ERR";
}
