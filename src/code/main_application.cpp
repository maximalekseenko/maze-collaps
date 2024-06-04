#define _XOPEN_SOURCE 700
#include "engine/engine.h"

#include "application/game.h"
#include "utility/log.h"

#include <locale>
#include <thread>

// TEMP
#include "map/generator.h"
#include "map/reader.h"

class PatternUI : public VisualComponent
{
public:
    int action;
    std::string text;
    PatternUI(Map *map, int sizeX, int sizeY, int posX, int posY, int value1 = 0, int value2 = 0, int value3=0)
        : VisualComponent(posX, posY, sizeX + 1, sizeY+1, VisualComponent::Layer::BUTTONS, "", Color::BLACK)
    {
        int *data = new int[map->MI];
        for (int _mapI = 0; _mapI < map->MI; _mapI++)
        {
            data[_mapI] = map->Get(_mapI)->type;
        }

        SetData(data,
                value1,
                value2, value3);

        delete data;
    }
    PatternUI(int *data, int sizeX, int sizeY, int posX, int posY, int value1 = 0, int value2 = 0, int value3=0)
        : VisualComponent(posX, posY, sizeX + 1, sizeY+1, VisualComponent::Layer::BUTTONS, "", Color::BLACK)
    {
        SetData(data,
                value1,
                value2,value3);
    }
    void SetData(int *data, int value1, int value2, int value3)
    {
        for (int _x = 0; _x < (this->GetW() - 1); _x++)
            for (int _y = 0; _y < (this->GetH() - 1); _y++)
                switch (data[_y * (this->GetW() - 1) + _x])
                {
                case 1052688:
                    this->AddLine(1 + _x, _y, "█", Color::BRIGHT_WHITE, Color::WHITE);
                    break;
                case 10526880:
                    this->AddLine(1 + _x, _y, ".", Color::BRIGHT_BLACK, Color::WHITE);
                    break;
                case 1052832:
                    this->AddLine(1 + _x, _y, "O", Color::RED, Color::WHITE);
                    break;
                case 1089552:
                    this->AddLine(1 + _x, _y, "X", Color::GREEN, Color::WHITE);
                    break;
                default:
                    break;
                }
        this->AddLine(0, 0, std::to_string(value1).c_str(), Color::CYAN);
        this->AddLine(0, 1, std::to_string(value2).c_str(), Color::CYAN);
        this->AddLine(0, GetH()-1, std::to_string(value3).c_str(), Color::CYAN);
    }
};

int main(int argc, char *argv[])
{
    Log::Out("+++BEGIN+++");
    setlocale(LC_ALL, "en_US.UTF-8");

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

    // Generator g(md);
    // g.Run(-1, -1);
    Map *map = Generator::Generate(md);
    // g.Save(map);


    Engine::Init();
    Engine::Start();
    PatternUI pImageRaw = PatternUI(image, X, Y, 110, 0);
    pImageRaw.Activate();
    std::vector<PatternUI *> AAA;
    for (int _patternIndex = 0; _patternIndex < md->GetPatternsAmount(); _patternIndex++)
    {
        PatternUI *_newP = new PatternUI(md->GetPattern(_patternIndex),
            md->patternSize, md->patternSize,
            (_patternIndex % 20) * (md->patternSize + 2),
            (_patternIndex / 20) * (md->patternSize + 2),
            md->GetPatternAgreementsAmount(_patternIndex, 0),
            md->GetPatternAgreementsAmount(_patternIndex, 1),
            _patternIndex
        );
        AAA.push_back(_newP);
        _newP->Activate();
    }
    PatternUI pMap = PatternUI(map, md->mapWidth, md->mapHeight, 110, Y+5);
    pMap.Activate();


    // Game::LoadMap("data/testmap.png");
    // std::thread t_game(Game::Run);

    // t_game.join();
    delete[] image;
    while (1)
    {
    }
    Engine::Stop();

    Log::Out("+++END+++");
}