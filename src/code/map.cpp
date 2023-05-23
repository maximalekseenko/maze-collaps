#include "map.h"


#include <string>


#include "overlappingwave.h"
#include "enemy.h"
#include "log.h"



#define GENTRYLIMIT 10


// +++PRIVATE MEMBERS+++
Map::TILE* Map::map = nullptr;


// +++READONLY* MEMBERS+++
int Map::MX = 32;
int Map::MY = 16;
int Map::MI = Map::MX * Map::MY;
std::string Map::mapName;
int Map::mapId = -1;
double Map::difficulty;


// +++LOAD FUNCTIONS+++
bool Map::Load(std::string fileName)
{
    mapName = fileName;
    auto generator = OverlappingWave(
        "data/" + fileName + ".png", 3, Map::MX, Map::MY, 
        true, true, 8, false, Model::Heuristic::Entropy);
        
    // generate
    int trys = GENTRYLIMIT;
    for(; trys != 0; trys --) if (generator.Run(-1, -1)) break;
    if (trys == 0) 
    {
        Log::Out("--ERR: Map::Load() failed due try limit for '" + fileName + "'");
        return false;
    }

    // set map
    if (Map::map != nullptr) delete[] Map::map;

    int* result = generator.Result();
    Map::map = new TILE[MI];
    for (int i = 0; i < MI; i ++) Map::map[i] = static_cast<TILE>(result[i]);
    delete[] result;

    Log::Out("--LOG: Map::Load() success for '" + fileName + "'");
    return true;
}


bool Map::Load(int mapId)
{
    Map::mapId = mapId;
    switch (mapId)
    {
        case -1: Load("test"); return true;
        case 0: Load("hostilecave"); return true;
        case 1: Load("theentrance"); return true;
        default: return false;
    }
}
bool Map::LoadNext() { return Load(mapId + 1); }


// +++ACCESS FUNCTIONS+++
Map::TILE& Map::Get(int i)
{
    return map[Fix(i)];
}

int Map::Move(int* i, int dX, int dY)
{
    dX %= MX;
    dY %= MY;

    int fX = (MX + ((*i % MX + dX) % MX)) % MX;
    int fY = (MY + ((*i / MX + dY) % MY)) % MY;
    *i = fX + fY * MX;
    return *i;
}
int Map::Move(int i, int dX, int dY) { return Move(&i, dX, dY); }

int Map::Up(int* i)
{
    *i = Map::Fix(*i - MX);
    return *i;
}
int Map::Up(int i) { return Up(&i); }

int Map::Down(int* i)
{
    *i = Map::Fix(*i + MX);
    return *i;
}
int Map::Down(int i) { return Down(&i); }

int Map::Left(int* i)
{
    *i = *i % MX == 0 ? *i + MX - 1 : *i - 1;
    return *i;
}
int Map::Left(int i) { return Left(&i); }

int Map::Right(int* i)
{
    *i = *i % MX == MX - 1 ? *i - MX + 1 : *i + 1;
    return *i;
}
int Map::Right(int i) { return Right(&i); }


int Map::Fix(int* i)
{
    if (*i < 0) *i = MI + *i % MI;
    else if (*i >= MI) *i = *i % MI;
    return *i;
}
int Map::Fix(int i) { return Fix(&i); }

int Map::FixX(int* x)
{
    *x = (MX + *x % MX) % MX;
    return *x;
}
int Map::FixX(int x) { return FixX(&x); }

int Map::FixY(int* y)
{
    *y = (MY + (*y % MY)) % MY;
    return *y;
}
int Map::FixY(int y) { return FixY(&y); }

// +++GEOMETRY FUNCTIONS+++
bool Map::IsNotObstacle(int i)
{
    if (Get(i) == TILE::WALL) return false;
    for (auto enemy : Enemy::enemies) if (enemy.position == i) return false;
    return true;
}
bool Map::IsLineOfSight(int i1, int i2)
{ 
    int x1 = Map::X(i1);
    int y1 = Map::Y(i1);
    int x2 = Map::X(i2);
    int y2 = Map::Y(i2);

    // return true;
    // fix fo recursive
    if (abs(x2 - x1) > MX / 2)
    {
        if (x2 < x1) x2 += MX;
        else         x2 -= MX;
    }

    if (abs(y2 - y1) > MY / 2)
    {
        if (y2 < y1) y2 += MY;
        else         y2 -= MY;
    }

    // find obstacles
    int len = std::max(abs(x2 - x1), abs(y2 - y1));
    for (int i = 0; i < len; i ++)
    {
        double step = double(i) / len;
        int x = FixX(round(x1 * (1.0 - step) + x2 * step));
        int y = FixY(round(y1 * (1.0 - step) + y2 * step));
        
        if (Get(x + y * MX) == Map::TILE::WALL) return false;
    }

    // no obstacles found
    return true;
}

int Map::X(int position) { return position % MX; }
int Map::Y(int position) { return position / MX; }
int Map::I(int x, int y) { return x + y * MX; }
