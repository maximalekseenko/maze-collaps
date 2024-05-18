#ifndef __MAP_MAP_H
#define __MAP_MAP_H

#include "./tile.h"

/// @brief Object TODOTODOTODOT
class Map
{
private:
    /// @brief
    Tile *mapTileArray;

public:
    /// @brief Dimensional size of map.
    const int MX, MY, MI;

    /// @brief Display name of this map.
    const char *name;

public: // +++ constructors and destructors +++ //
    /// @brief Instance of map with defined sizes.
    /// @param __x Size of map on x axis.
    /// @param __y Size of map on y axis.
    /// @param __name Display name of this map.
    /// @throws Throws an error if size on ether axis is less of equal 0.
    Map(int __x, int __y, const char *__name);

    ~Map();

public: // Access and Manipulate functions
    /// @brief
    /// @param __x
    /// @param __y
    void FixPoint(int *__x, int *__y);

    /// @brief
    /// @param __i
    /// @return
    bool HasPoint(int __i);

    /// @brief
    /// @param __x
    /// @param __y
    /// @return
    bool HasPoint(int __x, int __y);

    /// @brief Gets position on x axis of a point at __i.
    /// @param __i Index of a point.
    /// @return Position of a point on x axis.
    /// @throws Throws an error if provided point is out of map borders.
    int X(int __i);

    /// @brief Gets position on y axis of a point at __i.
    /// @param __i Index of a point.
    /// @return Position of a point on y axis.
    /// @throws Throws an error if provided point is out of map borders.
    int Y(int __i);

    /// @brief Gets index of a point at position.
    /// @param __x Position of a point on x axis.
    /// @param __y Position of a point on y axis.
    /// @return Index of a point.
    /// @note Accounts for map's recursive nature
    int I(int __x, int __y);

    /// @brief Moves a point in direction.
    /// @param __px Pointer on position of a point on x axis.
    /// @param __py Pointer on position of a point on y axis.
    /// @param __dX Delta on x axis between starting point and end point.
    /// @param __dY Delta on y axis between starting point and end point.
    /// @note Accounts for map's recursive nature.
    /// @throws Throws an error if provided starting point is out of map borders.
    int Move(int *__px, int *__py, int __dX, int __dY);

    /// @brief Moves a point in direction.
    /// @param __x Position of a point on x axis.
    /// @param __y Position of a point on y axis.
    /// @param __dX Delta on x axis between starting point and end point.
    /// @param __dY Delta on y axis between starting point and end point.
    /// @note Accounts for map's recursive nature.
    /// @return New index of a point.
    /// @throws Throws an error if provided starting point is out of map borders.
    /// @overload Move(int*, int*, int, int).
    int Move(int __x, int __y, int __dX, int __dY);

    /// @brief Moves a point in direction.
    /// @param __pI Pointer on index of a point to move.
    /// @param __dX Delta on x axis between starting point and end point.
    /// @param __dY Delta on y axis between starting point and end point.
    /// @note Accounts for map's recursive nature.
    /// @throws Throws an error if provided starting point is out of map borders.
    /// @overload Move(int, int, int, int).
    int Move(int *__pI, int __dX, int __dY);

    /// @brief Moves a point in direction.
    /// @param __i Index of a point to move.
    /// @param __dX Delta on x axis between starting point and end point.
    /// @param __dY Delta on y axis between starting point and end point.
    /// @return New index of a point.
    /// @note Accounts for map's recursive nature.
    /// @throws Throws an error if provided starting point is out of map borders.
    /// @overload Move(int*, int, int).
    int Move(int __i, int __dX, int __dY);

    /// @brief Gets a tile at point.
    /// @param __i Index of a point.
    /// @return Pointer to a `Tile` at a point.
    /// @throws Throws an error if provided point is out of map borders.
    Tile *Get(int __i);

    /// @brief Gets a tile at point.
    /// @param __x Position of a point on x axis.
    /// @param __y Position of a point on y axis.
    /// @return Pointer to a `Tile` at a point.
    /// @throws Throws an error if provided point is out of map borders.
    /// @overload Get(int).
    Tile *Get(int __x, int __y);
};

#endif // __MAP_MAP_H