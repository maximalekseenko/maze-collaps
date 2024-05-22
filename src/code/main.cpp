#define _XOPEN_SOURCE 700
#include "engine/engine.h"

#include "game.h"
#include "utility/log.h"

#include <locale>
#include <thread>

// TEMP
#include "map/reader.h"

class PatternUI : public VisualComponent
{
public:
    int action;
    std::string text;
    PatternUI(int *data, int sizeX, int sizeY, int posX, int posY, int value1=-1, int value2=-1)
        : VisualComponent( posX, posY, sizeX+1, sizeY, VisualComponent::Layer::BUTTONS, "", Color::BLACK)
    {
        std::lock_guard el_lock(lock);

        for (int _x = 0; _x < sizeX; _x++)
            for (int _y = 0; _y < sizeY; _y++)
                switch (data[_y * sizeX + _x])
                {
                case 1052688:
                    this->AddLine(1+_x, _y, "█", Color::BRIGHT_WHITE, Color::WHITE);
                    break;
                case 10526880:
                    this->AddLine(1+_x, _y, ".", Color::BRIGHT_BLACK, Color::WHITE);
                    break;
                case 1052832:
                    this->AddLine(1+_x, _y, "O", Color::RED, Color::WHITE);
                    break;
                case 1089552:
                    this->AddLine(1+_x, _y, "X", Color::GREEN, Color::WHITE);
                    break;
                default: break;
                }
        this->AddLine(0, 0, std::to_string(value1).c_str(), Color::CYAN);
        this->AddLine(0, 1, std::to_string(value2).c_str(), Color::CYAN);
    }
};

int main(int argc, char *argv[])
{
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "en_US.UTF-8");
    
    Engine::Init();

    Engine::Start();

    int *image = nullptr, X, Y;
    Reader::ReadImage("data/testmap.png", image, X, Y);

    for (int y = 0; y < Y; y++)
    {
        std::string s;
        for (int x = 0; x < X; x++)
            s += std::to_string(image[y * X + x]) + " ";
        Log::Out(s);
    }

    Mapdata *md = Reader::GetMapdata("data/testmap.png");

    Log::Out(std::to_string(md->GetPatternsAmount()));

    Log::Out(std::to_string(Reader::GetPixelValue(255, 255, 255)));

    PatternUI pImageRaw = PatternUI(image, X, Y, 110, 0);
    delete[] image;
    pImageRaw.Activate();


    std::vector<PatternUI *> AAA;
    for (int _patternIndex = 0; _patternIndex < md->GetPatternsAmount(); _patternIndex++)
    {
        PatternUI *_newP = new PatternUI(md->GetPattern(_patternIndex), 
            md->patternSize, md->patternSize,
            (_patternIndex % 20) * (md->patternSize + 2), 
            (_patternIndex / 20) * (md->patternSize + 1), 
            md->GetPatternWeight(_patternIndex),
            md->GetPatternAgreementsAmount(_patternIndex, 0)
        );
        AAA.push_back(_newP);
        _newP->Activate();
    }
    // Game::LoadMap("data/testmap.png");
    // std::thread t_game(Game::Run);

    // t_game.join();
    while (1)
    {
    }
    Engine::Stop();

    Log::Out("+++END+++");
}