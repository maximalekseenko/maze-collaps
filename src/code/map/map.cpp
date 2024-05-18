#include "./map.h"

// TEMP
#include "utility/log.h"

Map::Map(int __x, int __y, const char *__name)
    : MX(__x), MY(__y), MI(__y), name(__name)
{
    this->mapTileArray = new Tile[this->MI];
}

Map::~Map()
{
    delete[] this->mapTileArray;
}

void Map::FixPoint(int *__x, int *__y)
{
    // negative fix
    if (*__x < 0)
        *__x = (*__x % this->MX) + this->MX;
    if (*__y < 0)
        *__y = (*__y % this->MY) + this->MY;

    *__x %= this->MX;
    *__y %= this->MY;
}

bool Map::HasPoint(int __i)
{
    return __i > 0 && __i < this->MI;
}

bool Map::HasPoint(int __x, int __y)
{
    return __x > 0 && __x < this->MX && __y > 0 && __y < this->MY;
}

int Map::X(int __i)
{
    // check arguments
    // - if provided point is out of map borders
    if (!this->HasPoint(__i))
        Log::Out("Map got point out of bounds.", true);

    // return
    return __i % this->MX;
}

int Map::Y(int __i)
{
    // check arguments
    // - if provided point is out of map borders
    if (!this->HasPoint(__i))
        Log::Out("Map got point out of bounds.", true);

    // return
    return __i / this->MX;
}

int Map::I(int __x, int __y)
{
    // fix arguments
    this->FixPoint(&__x, &__y);

    // return
    return __x + __y * this->MX;
}

int Map::Move(int *__pI, int __dX, int __dY)
{
    // move
    *__pI = this->Move(
        this->X(*__pI),
        this->Y(*__pI),
        __dX, __dY);

    // return
    return *__pI;
}

int Map::Move(int __i, int __dX, int __dY) { return this->Move(&__i, __dX, __dY); }

int Map::Move(int *__pX, int *__pY, int __dX, int __dY)
{
    // check arguments
    // - if provided starting point is out of map borders
    if (!this->HasPoint(*__pX, *__pY))
        Log::Out("Map got point out of bounds.", true);

    // move
    *__pX += __dX;
    *__pY += __dY;
    this->FixPoint(__pX, __pY);

    // return
    return this->I(*__pX, *__pY);
}

int Map::Move(int __x, int __y, int __dX, int __dY) { return this->Move(&__x, &__y, __dX, __dY); }

Tile *Map::Get(int __i)
{
    // check arguments
    // - if provided starting point is out of map borders
    if (!this->HasPoint(__i))
        Log::Out("Map got point out of bounds.", true);

    // return
    return &this->mapTileArray[__i];
}

Tile *Map::Get(int __x, int __y) { return this->Get(this->I(__x, __y)); }
